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

val platform: String = (findProperty("platform") as String?) ?: when {
    DefaultNativePlatform.getCurrentOperatingSystem().isWindows -> "windows-x64"
    DefaultNativePlatform.getCurrentOperatingSystem().isMacOsX -> "macos-x64"
    else -> "linux-x64"
}

repositories {
    mavenCentral()
}

dependencies {
    compileOnly("org.jetbrains:annotations:${rootProject.property("jetbrains_annotations_version").toString()}")

    implementation("com.ibm.icu:icu4j:${rootProject.property("icu4j_version").toString()}")

    testImplementation(platform("org.junit:junit-bom:${rootProject.property("junit_version").toString()}"))
    testImplementation("org.junit.jupiter:junit-jupiter")
    testRuntimeOnly("org.junit.platform:junit-platform-launcher")
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