import org.gradle.nativeplatform.platform.internal.DefaultNativePlatform
import java.util.*

plugins {
    id("java")
    id("c")
}

group = rootProject.property("maven_group").toString()
version = rootProject.property("version").toString()

base.archivesName.set(rootProject.property("archives_base_name").toString())

val localProperties = Properties().apply {
    val localPropertiesFile = rootProject.file("local.properties")

    if (localPropertiesFile.exists())
        load(localPropertiesFile.inputStream())
}

val ultralightSdkLocation: String =
    (localProperties.getProperty("ultralight_sdk_location") ?: findProperty("ultralight_sdk_location") as String? ?: "").also {
        if (it.isBlank()) {
            logger.warn(
                "\n[ultralight-jni] WARNING: 'ultralight_sdk_location' is not set.\n" +
                        "  Native compilation will be skipped.\n" +
                        "  Set it in local.properties or pass -Pultralight_sdk_location=<path>.\n" +
                        "  See docs/NATIVE_SETUP.md for instructions.\n"
            )
        }
    }

// Set tracy_enabled=true in local.properties or pass -Ptracy_enabled=true to enable Tracy profiling.
// Never enable in CI release builds — Tracy opens a network socket and adds overhead.
val tracyEnabled: Boolean = (localProperties.getProperty("tracy_enabled") ?: findProperty("tracy_enabled") as String? ?: "false")
        .trim()
        .equals("true", ignoreCase = true)

val platform: String = (findProperty("platform") as String?) ?: when {
    DefaultNativePlatform.getCurrentOperatingSystem().isWindows -> "windows-x64"
    DefaultNativePlatform.getCurrentOperatingSystem().isMacOsX -> "macos-x64"
    else -> "linux-x64"
}

val lwjglNatives = Pair(
    System.getProperty("os.name")!!,
    System.getProperty("os.arch")!!
).let { (name, arch) ->
    when {
        "FreeBSD" == name ->
            "natives-freebsd"
        arrayOf("Linux", "SunOS", "Unit").any { name.startsWith(it) } ->
            if (arrayOf("arm", "aarch64").any { arch.startsWith(it) })
                "natives-linux${if (arch.contains("64") || arch.startsWith("armv8")) "-arm64" else "-arm32"}"
            else if (arch.startsWith("ppc"))
                "natives-linux-ppc64le"
            else if (arch.startsWith("riscv"))
                "natives-linux-riscv64"
            else
                "natives-linux"
        arrayOf("Mac OS X", "Darwin").any { name.startsWith(it) }     ->
            "natives-macos${if (arch.startsWith("aarch64")) "-arm64" else ""}"
        arrayOf("Windows").any { name.startsWith(it) }                ->
            if (arch.contains("64"))
                "natives-windows${if (arch.startsWith("aarch64")) "-arm64" else ""}"
            else
                "natives-windows-x86"
        else                                                                            ->
            throw Error("Unrecognized or unsupported platform. Please set \"lwjglNatives\" manually")
    }
}

repositories {
    mavenCentral()
}

dependencies {
    implementation("com.ibm.icu:icu4j:${rootProject.property("icu4j_version").toString()}")

    compileOnly("org.jetbrains:annotations:${rootProject.property("jetbrains_annotations_version").toString()}")

    /* -- Tests -- */
    testImplementation(platform("org.junit:junit-bom:${rootProject.property("junit_version").toString()}"))
    testImplementation("org.junit.jupiter:junit-jupiter")
    testRuntimeOnly("org.junit.platform:junit-platform-launcher")

    testImplementation("org.mockito:mockito-core:${rootProject.property("mockito_version").toString()}")

    testImplementation("commons-io:commons-io:2.22.0")
    testImplementation("com.google.guava:guava:33.6.0-jre")

    testImplementation(platform("org.lwjgl:lwjgl-bom:${rootProject.property("lwjgl_version").toString()}"))
    testImplementation("org.lwjgl:lwjgl")
    testImplementation("org.lwjgl:lwjgl-glfw")
    testImplementation("org.lwjgl:lwjgl-opengl")
    testImplementation("org.lwjgl", "lwjgl", classifier = lwjglNatives)
    testImplementation("org.lwjgl", "lwjgl-glfw", classifier = lwjglNatives)
    testImplementation("org.lwjgl", "lwjgl-opengl", classifier = lwjglNatives)
}

val generateJniHeaders by tasks.registering(JavaCompile::class) {
    group = "jni"
    description = "Compiles Java sources and emits JNI .h headers via the -h flag"

    source = sourceSets.main.get().java
    classpath = sourceSets.main.get().compileClasspath
    destinationDirectory.set(layout.buildDirectory.dir("jni-classes"))

    options.compilerArgs.addAll(
        listOf(
            "-h", layout.buildDirectory.dir("jni-headers").get().asFile.absolutePath,
            "-implicit:class"  // still emit .class files for javac cache
        )
    )
}

val cmakeBuildDir = layout.buildDirectory.dir("cmake-build").get().asFile

val cmakeConfigure by tasks.registering(Exec::class) {
    group = "jni"
    description = "Runs CMake to configure the native build"

    dependsOn(generateJniHeaders)

    onlyIf { ultralightSdkLocation.isNotBlank() }

    doFirst { cmakeBuildDir.mkdirs() }

    workingDir = cmakeBuildDir
    commandLine(
        "cmake",
        file("src/main/native").absolutePath,
        "-DJNI_HEADER_DIR=${layout.buildDirectory.dir("jni-headers").get().asFile}",
        "-DULTRALIGHT_SDK=${ultralightSdkLocation}",
        "-DPLATFORM=${platform}",
        "-DCMAKE_BUILD_TYPE=Release",
        "-DLUMINESCENCE_TRACY=${if (tracyEnabled) "ON" else "OFF"}",
        "-G", if (DefaultNativePlatform.getCurrentOperatingSystem().isWindows) "Visual Studio 18 2026" else "Unix Makefiles"
    )
}

val compileNative by tasks.registering(Exec::class) {
    group = "jni"
    description = "Compiles native code via CMake --build"
    dependsOn(cmakeConfigure)

    onlyIf { ultralightSdkLocation.isNotBlank() }

    workingDir = cmakeBuildDir
    commandLine("cmake", "--build", ".", "--config", "Release", "--parallel")

    // Copy the produced shared lib into resources so it's on the classpath
    doLast {
        val libName = when (platform) {
            "windows-x64" -> "LuminescenceJNI.dll"
            "macos-x64",
            "macos-arm64" -> "libLuminescenceJNI.dylib"

            else -> "libLuminescenceJNI.so"
        }

        val produced = file("$cmakeBuildDir/Release/$libName")
        if (produced.exists()) {
            copy {
                from(produced)
                into(layout.buildDirectory.dir("resources/main/natives/$platform"))
            }
        }
    }
}

tasks.register<JavaExec>("runDemoApp") {
    group = "demos"
    description = "Runs the JNI test application with the correct library path."

    // Ensure the native library is built before running
    dependsOn("cmakeConfigure")

    classpath = sourceSets["test"].runtimeClasspath
    mainClass.set("me.ayydxn.luminescence.demo.DemoApp")

    // Automatically set the PATH so Windows finds luminescence_jni.dll and Ultralight.dll
    val nativeOutDir = file("build/cmake-build/Release").absolutePath
    val ultralightBin = file("$ultralightSdkLocation/bin").absolutePath

    systemProperty("java.library.path", "$nativeOutDir;$ultralightBin")

    // Also add to environment PATH for Windows-level dependency resolution
    if (DefaultNativePlatform.getCurrentOperatingSystem().isWindows)
    {
        environment("PATH", "${environment["PATH"]};$nativeOutDir;$ultralightBin")
    }
    else if (DefaultNativePlatform.getCurrentOperatingSystem().isMacOsX)
    {
        environment("DYLD_LIBRARY_PATH", "$nativeOutDir:$ultralightBin")
    }
    else if (DefaultNativePlatform.getCurrentOperatingSystem().isLinux)
    {
        environment("LD_LIBRARY_PATH", "${environment["LD_LIBRARY_PATH"]}:$nativeOutDir:$ultralightBin")
    }
}

tasks.register<JavaExec>("runLWJGLDemoApp") {
    group = "demos"
    description = "Runs the JNI test application with the correct library path."

    // Ensure the native library is built before running
    dependsOn("cmakeConfigure")

    classpath = sourceSets["test"].runtimeClasspath
    mainClass.set("me.ayydxn.luminescence.demo.LWJGLDemoApp")

    // Automatically set the PATH so Windows finds luminescence_jni.dll and Ultralight.dll
    val nativeOutDir = file("build/cmake-build/Release").absolutePath
    val ultralightBin = file("$ultralightSdkLocation/bin").absolutePath

    systemProperty("java.library.path", "$nativeOutDir;$ultralightBin")

    // Also add to environment PATH for Windows-level dependency resolution
    if (DefaultNativePlatform.getCurrentOperatingSystem().isWindows)
    {
        environment("PATH", "${environment["PATH"]};$nativeOutDir;$ultralightBin")
    }
    else if (DefaultNativePlatform.getCurrentOperatingSystem().isMacOsX)
    {
        environment("DYLD_LIBRARY_PATH", "$nativeOutDir:$ultralightBin")
    }
    else if (DefaultNativePlatform.getCurrentOperatingSystem().isLinux)
    {
        environment("LD_LIBRARY_PATH", "${environment["LD_LIBRARY_PATH"]}:$nativeOutDir:$ultralightBin")
    }
}

tasks.withType<JavaCompile> {
    options.encoding = "UTF-8"
    options.release = 17
}

tasks.withType<Test> {
    useJUnitPlatform()

    val nativeOutDir = file("build/cmake-build/Release").absolutePath
    val ultralightBin = file("$ultralightSdkLocation/bin").absolutePath

    systemProperty("java.library.path", "$nativeOutDir;$ultralightBin")

    if (DefaultNativePlatform.getCurrentOperatingSystem().isWindows)
    {
        environment("PATH", "${environment["PATH"]};$nativeOutDir;$ultralightBin")
    }
    else if (DefaultNativePlatform.getCurrentOperatingSystem().isMacOsX)
    {
        environment("DYLD_LIBRARY_PATH", "$nativeOutDir:$ultralightBin")
    }
    else if (DefaultNativePlatform.getCurrentOperatingSystem().isLinux)
    {
        environment("LD_LIBRARY_PATH", "${environment["LD_LIBRARY_PATH"]}:$nativeOutDir:$ultralightBin")
    }

    failOnNoDiscoveredTests = false

    testLogging {
        events("passed", "skipped", "failed")
    }
}


// Wire native compilation into the normal build
tasks.named("processResources") { dependsOn(compileNative) }

java {
    // Loom will automatically attach sourcesJar to a RemapSourcesJar task and to the "build" task
    // if it is present.
    // If you remove this line, sources will not be generated.
    withSourcesJar()

    sourceCompatibility = JavaVersion.VERSION_17
    targetCompatibility = JavaVersion.VERSION_17
}