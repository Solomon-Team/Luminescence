<div align="center">

# 🔨 Building Luminescence

**Everything you need to compile Luminescence from source, run the test suite, and profile performance with Tracy.**

</div>

---

## 📋 Table of Contents

1. [Prerequisites](#1--prerequisites)
2. [Repository Layout](#2--repository-layout)
3. [Obtaining the Ultralight SDK](#3--obtaining-the-ultralight-sdk)
4. [Configuring local.properties](#4--configuring-localproperties)
5. [Building](#5--building)
6. [Running the Tests](#6--running-the-tests)
7. [Running the Demo Applications](#7--running-the-demo-applications)
8. [Tracy Profiling](#8--tracy-profiling)
9. [Platform Notes](#9--platform-notes)

---

## 1. 🧰 Prerequisites

The following tools must be installed and available on your `PATH` before you do anything else.

### All Platforms

| Tool | Minimum Version | Notes |
|---|---|---|
| JDK | 17 | Zulu, Temurin, or Oracle all work. `JAVA_HOME` must be set. |
| CMake | 3.20 | Used to configure and drive the C++ build. |
| Git | Any recent | Required by CMake's `FetchContent` when Tracy is enabled. |

### 🪟 Windows

| Tool               | Notes                                                                                                                                                                                                                                                                        |
|--------------------|------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| Visual Studio 2026 | The **Desktop development with C++** workload is required. CMake targets the `"Visual Studio 18 2026"` generator — earlier versions will not work without editing `build.gradle.kts`. Installing only Build Tools for Visual Studio will suffice. The full IDE isn't needed. |
| 7-Zip              | Only needed if you obtain the Ultralight SDK as a `.7z` archive.                                                                                                                                                                                                             |

### 🍎 macOS

| Tool                     | Notes                                                               |
|--------------------------|---------------------------------------------------------------------|
| Xcode Command Line Tools | Install with `xcode-select --install`. Provides `clang` and `make`. |

### 🐧 Linux

| Tool                   | Notes                                                                                                       |
|------------------------|-------------------------------------------------------------------------------------------------------------|
| GCC ≥ 13 or Clang ≥ 17 | Must support C++20. The build uses `-O3 -march=native -flto`, so a reasonably modern toolchain is expected. |
| `build-essential`      | Provides `make`, `gcc`, `g++`. Install via `sudo apt install build-essential` on Debian/Ubuntu.             |

Verify your setup before proceeding:

```bash
java -version       # Should print 17 or higher
cmake --version     # Should print 3.20 or higher
git --version
```

---

## 2. 🗂️ Repository Layout

Understanding where things live will save you time when something goes wrong.

```
Luminescence/
├── build.gradle.kts          # Gradle build — all tasks defined here
├── gradle.properties         # Version numbers and Maven coordinates
├── local.properties          # Your machine-local config (never committed)
├── settings.gradle.kts
│
├── src/main/
│   ├── java/                 # Public Java API (me.ayydxn.luminescence.*)
│   └── native/               # C++ JNI bridge
│       ├── Adapters/         # Callback adapter implementations
│       ├── Cache/            # JNI class/method ID caches
│       ├── Core/             # ICallbackAdapter, JNI utilities, ScopedLocalRef
│       ├── JavaScript/       # JSC interop (JSValue, JSObject, JSFunction, etc.)
│       ├── CMakeLists.txt    # Native build definition
│       └── JNIBridge.cpp     # JNI_OnLoad / JNI_OnUnload entry points
│
├── src/test/
│   ├── java/                 # Unit + integration tests, demo apps
│   └── resources/ultralight/ # Test resources: HTML, shaders, ICU data
│
└── build/                    # Generated — never commit this
    ├── jni-headers/          # .h files emitted by generateJniHeaders
    ├── cmake-build/          # CMake build directory
    └── resources/main/       # Processed resources including the built .dll/.so
```

> [**TIP**]
> The `build/` directory is entirely generated. Delete it freely if something seems stuck — `./gradlew clean` does this for you.

---

## 3. 📦 Obtaining the Ultralight SDK

Luminescence does not bundle the Ultralight runtime binaries due to Ultralight's licensing terms. You must obtain the SDK and provide these binaries yourself. Additionally, it must match the version that Luminescence's bindings were built against (currently **1.4.0**). Mismatched versions will compile but fail at runtime with `UnsatisfiedLinkError` or similar.

Download the SDK for your platform from the [Ultralight releases page](https://github.com/ultralight-ux/Ultralight/releases) or your internal mirror. The SDK archive extracts to a directory with this layout:

```
ultralight-sdk/
├── include/          # C API headers (Ultralight/*.h, JavaScriptCore/*.h, etc.)
├── lib/              # Import libraries (.lib on Windows, .a/.so on Linux/macOS)
└── bin/              # Runtime DLLs / dylibs (Ultralight.dll, WebCore.dll, etc.)
```

The path to this extracted directory is what you will set as `ultralight_sdk_location` in the next step. The `include/` and `lib/` subdirectories are used at compile time; the `bin/` directory is added to the library search path at test and demo runtime.

> [**NOTE**]
> If you are having issues with the JVM not finding Ultralight's binaries at test or demo time, even with the appropriate configuration, try placing the binaries in the root of the working directory. In a development environment, this will be the project root.
---

## 4. ⚙️ Configuring local.properties

Create a file named `local.properties` in the project root. This file is already listed in `.gitignore` and **must never be committed.**

```properties
# Required — absolute path to your extracted Ultralight SDK
ultralight_sdk_location=C:/Development/SDKs/Ultralight

# Optional — Enables Tracy profiling instrumentation (see Section 8)
# WARNING: Never ship a build with this enabled. Default is false.
tracy_enabled=false
```

> [**NOTE**]
> Use forward slashes on all platforms, including Windows. Gradle handles the conversion.

### Alternative: Gradle Project Properties

If you prefer not to use `local.properties` — for example, in a CI environment — you can pass properties directly on the command line:

```bash
./gradlew compileNative -Pultralight_sdk_location=/path/to/sdk
```

Command-line properties take precedence over `local.properties`.

---

## 5. 🏗️ Building

All build tasks are Gradle tasks. Use the Gradle wrapper (`./gradlew` on Unix, `gradlew.bat` on Windows) rather than a system-installed Gradle to ensure version consistency.

### Step 1 — Generate JNI Headers

```bash
./gradlew generateJniHeaders
```

Compiles the Java sources and emits the C JNI header files that the native code implements. Output goes to `build/jni-headers/`. This task runs automatically as a dependency of `compileNative`, but you can run it standalone to verify your Java sources compile cleanly before touching C++.

In practice, this is only used for ensuring that native C++ functions have the correct signatures as Luminescence uses the `RegisterNatives` approach over automatic name-based lookup. If you change a Java method signature that is called from C++, you must regenerate the headers and update the corresponding C++ function signature.

### Step 2 — Configure the Native Build

```bash
./gradlew cmakeConfigure
```

Invokes CMake to configure the native build. Under the hood this runs something like:

```bash
cmake src/main/native \
  -DJNI_HEADER_DIR=build/jni-headers \
  -DULTRALIGHT_SDK=/path/to/sdk \
  -DPLATFORM=windows-x64 \
  -DCMAKE_BUILD_TYPE=Release \
  -DLUMINESCENCE_TRACY=OFF \
  -G "Visual Studio 18 2026"  # Windows
  -G "Unix Makefiles"          # Linux / macOS
```

CMake output is written to `build/cmake-build/`. If this step fails, the error message from CMake will tell you what is missing — most commonly a wrong SDK path or a missing compiler.

### Step 3 — Compile

```bash
./gradlew compileNative
```

Runs `cmake --build` and copies the resulting shared library into `build/resources/main/natives/<platform>/` so it is on the classpath for tests and demos.

Output filenames by platform:

| Platform      | Output File                |
|---------------|----------------------------|
| `windows-x64` | `LuminescenceJNI.dll`      |
| `linux-x64`   | `libLuminescenceJNI.so`    |
| `macos-x64`   | `libLuminescenceJNI.dylib` |
| `macos-arm64` | `libLuminescenceJNI.dylib` |

### ⚡ Full Build (Recommended)

```bash
./gradlew build
```

Runs the full task chain in the correct dependency order:

```
generateJniHeaders → cmakeConfigure → compileNative → processResources → compileJava → test → jar
```

The final JAR is written to `build/libs/`.

### Overriding the Platform

By default, the platform is detected from the current OS. You can override it explicitly:

```bash
./gradlew compileNative -Pplatform=linux-x64
```

Valid values: `windows-x64`, `linux-x64`, `linux-arm64`, `macos-x64`, `macos-arm64`.

> [**WARNING**]
> Cross-compilation is not supported — the CMake build always compiles for the host machine. Passing a mismatched platform string will produce a binary that will not load on the host.

---

## 6. 🧪 Running the Tests

```bash
./gradlew test
```

The test runner automatically sets `java.library.path` and the platform-native library search path (`PATH` / `LD_LIBRARY_PATH` / `DYLD_LIBRARY_PATH`) to include both the built JNI library and the Ultralight SDK's `bin/` directory. You do not need to configure these manually.

Test results are written to:
- `build/test-results/test/` — JUnit XML (for CI)
- `build/reports/tests/test/index.html` — Human-readable HTML report

### Running a Specific Test Class

```bash
./gradlew test --tests "me.ayydxn.luminescence.javascript.JSFunctionTest"
```

### Running a Specific Test Package

```bash
./gradlew test --tests "me.ayydxn.luminescence.javascript.*"
```

### What the Tests Require

The test suite loads the real Ultralight runtime. Tests will fail with `UnsatisfiedLinkError` if:

- `compileNative` has not been run (no JNI library on the classpath)
- `ultralight_sdk_location` is not set (the SDK's runtime binaries cannot be found)
- The Ultralight SDK version does not match what the bindings were built against (currently **1.4.0**)

---

## 7. 🎮 Running the Demo Applications

Two demo applications are included under `src/test/java/.../demo/`. Both require a working LWJGL environment and a connected display — they cannot run headlessly.

### AppCore Demo

A minimal application which creates a window and renders a web view inside it using Ultralight's AppCore API. This is currently not fully implemented and only prints Ultralight and WebKit versioning info.

```bash
./gradlew runDemoApp
```

### CPU Rendering Demo

Renders a web view using Ultralight's software rasterizer, displayed via OpenGL using LWJGL. This is the primary interop demo and loads `src/test/resources/ultralight/html/advanced_demo.html`.

```bash
./gradlew runLWJGLDemoApp
```

### GPU Rendering Demo

Uses Ultralight's GPU acceleration path with a custom `ULGPUDriver` implementation. This is run the same way that the CPU demo is, but the `GPU_DEMO` boolean within the `LWJGLDemoApp` class must be set to `true` before compiling and running. 

Both tasks depend on `cmakeConfigure` and set the library path automatically.

> [**TIP**]
> If a window appears but is blank, see [Troubleshooting](#10--troubleshooting).

---

## 8. 📊 Tracy Profiling

[Tracy](https://github.com/wolfpld/tracy) is a real-time C++ frame profiler. Luminescence includes optional Tracy instrumentation gated behind a build flag that is **off by default.**

> [**CAUTION**]
> A Tracy-enabled build opens a TCP socket on startup and buffers profiling data continuously. **Never ship or distribute a Tracy-enabled build.** This flag is strictly for local development.

### Enabling Tracy

Add to `local.properties`:

```properties
tracy_enabled=true
```

Or pass it on the command line for a one-off profiling session:

```bash
./gradlew compileNative -Ptracy_enabled=true
```

When enabled, Gradle passes `-DLUMINESCENCE_TRACY=ON` to CMake. CMake then uses `FetchContent` to download Tracy at configure time (requires internet access and Git) and links `TracyClient` into `LuminescenceJNI`.

### Connecting the Tracy GUI

1. Download the Tracy profiler GUI from the [Tracy releases page](https://github.com/wolfpld/tracy/releases) for your platform. (Ensure to download the same version that Luminescence uses which is currently 0.13.1)
2. Build and run Luminescence with Tracy enabled.
3. Open the Tracy GUI — it will detect the running process automatically on `localhost` and connect.

Because `TRACY_ON_DEMAND` is enabled, Tracy only begins collecting data **after the GUI connects**. Startup behavior is not captured by default, but steady-state profiling (render loop, callback dispatch, string conversions) works without any pre-launch setup.

---

## 9. 🖥️ Platform Notes

### 🪟 Windows

- The CMake generator is hardcoded to `"Visual Studio 18 2026"`. If you need to use an earlier version, edit the `cmakeConfigure` task in `build.gradle.kts`.
- `/arch:AVX2` is passed to the compiler. If your machine does not support AVX2, remove this flag from `CMakeLists.txt`.
- The Ultralight SDK's `bin/` directory is added to `PATH` at test runtime so that `Ultralight.dll`, `WebCore.dll`, `UltralightCore.dll`, and `AppCore.dll` are found without any manual configuration.

### 🍎 macOS

- Both `macos-x64` (Intel) and `macos-arm64` (Apple Silicon) are supported. The platform is detected automatically from `os.arch`.
- The build works without changes on M1/M2/M3 machines.
- Ultralight's runtime dylibs must be in `DYLD_LIBRARY_PATH` at test/demo runtime — the Gradle tasks set this automatically from `ultralight_sdk_location`.
- macOS may quarantine downloaded dylibs. If you encounter `Library not loaded` errors at runtime, clear the quarantine attribute:

```bash
xattr -r -d com.apple.quarantine /path/to/ultralight-sdk/bin/
```

### 🐧 Linux

- GCC 13+ or Clang 17+ is required for full C++20 support. The default `g++` on Ubuntu 22.04 is GCC 11 — install a newer version via the `ubuntu-toolchain-r/test` PPA or use Clang from [llvm.org](https://llvm.org).
- `-march=native` optimises for your specific CPU and produces a non-portable binary. This is intentional for development. For a distributable build, replace it with an appropriate baseline such as `-march=x86-64-v2`.
- If Tracy is enabled, ensure `libstdc++` is version 11 or later, as Tracy uses `<bit>` header features.