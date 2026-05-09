cmake_minimum_required(VERSION 3.20)
project(LuminescenceJNI)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ── Compiler hardening & optimisation flags ──────────────────────────────────
if(MSVC)
    add_compile_options(/O2 /GL /arch:AVX2 /W4)
    add_link_options(/LTCG)
else()
    add_compile_options(-O3 -march=native -flto -fvisibility=hidden -Wall)
    add_link_options(-flto)
endif()

# ── Locate JNI ───────────────────────────────────────────────────────────────
find_package(JNI REQUIRED)

# ── Paths passed in from Gradle ──────────────────────────────────────────────
# JNI_HEADER_DIR  — generated headers from javac -h
# ULTRALIGHT_SDK  — root of the user's Ultralight SDK
# PLATFORM        — e.g. windows-x64

set(UL_INCLUDE "${ULTRALIGHT_SDK}/include")
set(UL_LIB_DIR "${ULTRALIGHT_SDK}/lib")   # adjust to SDK layout

# ── Sources ──────────────────────────────────────────────────────────────────
file(GLOB_RECURSE SOURCES "*.cpp" "*.h")

add_library(LuminescenceJNI SHARED ${SOURCES}
        JNIBridge.cpp)

target_include_directories(LuminescenceJNI PRIVATE
        ${CMAKE_SOURCE_DIR}
        ${JNI_INCLUDE_DIRS}
        ${JNI_HEADER_DIR}
        ${UL_INCLUDE}
        include
)

# (Ayydxn) Thank you, C++ committee for deprecating this stuff and then proceeding to not introduce a modern, cross-platform replacement :D
target_compile_definitions(LuminescenceJNI PRIVATE
        _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
)

# Link against Ultralight (user-provided)
target_link_directories(LuminescenceJNI PRIVATE ${UL_LIB_DIR})
target_link_libraries(LuminescenceJNI PRIVATE
        Ultralight
        UltralightCore
        WebCore
        AppCore
)

# Export only JNI symbols, keep everything else hidden
set_target_properties(LuminescenceJNI PROPERTIES
        CXX_VISIBILITY_PRESET hidden
        VISIBILITY_INLINES_HIDDEN ON
)

# ── Tracy profiler (development only, off by default) ────────────────────────
# Enable with: cmake ... -DLUMINESCENCE_TRACY=ON
# Never enable in distributed or CI release builds — Tracy opens a network socket and adds overhead.
option(LUMINESCENCE_TRACY "Enable Tracy Profiling Instrumentation" OFF)

if(LUMINESCENCE_TRACY)
    message(STATUS "Tracy profiling enabled — Do NOT ship this build to end users!")

    include(FetchContent)
    FetchContent_Declare(
        tracy
        GIT_REPOSITORY https://github.com/wolfpld/tracy.git
        GIT_TAG        v0.13.1
        GIT_SHALLOW    TRUE
    )

    # Disable Tracy's own examples and documentation targets
    set(TRACY_ENABLE ON  CACHE BOOL "" FORCE)
    set(TRACY_ON_DEMAND ON  CACHE BOOL "" FORCE)  # only profile while the GUI is connected
    set(TRACY_NO_BROADCAST OFF CACHE BOOL "" FORCE)
    set(TRACY_NO_CODE_TRANSFER OFF CACHE BOOL "" FORCE)

    FetchContent_MakeAvailable(tracy)

    target_link_libraries(LuminescenceJNI PRIVATE TracyClient)
    target_compile_definitions(LuminescenceJNI PRIVATE TRACY_ENABLE)
endif()