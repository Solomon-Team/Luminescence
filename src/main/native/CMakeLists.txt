cmake_minimum_required(VERSION 3.20)
project(LuminescenceJNI)

set(CMAKE_CXX_STANDARD 17)
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
        JNIBridge.cpp
        JNIBridge.cpp)

target_include_directories(LuminescenceJNI PRIVATE
        ${JNI_INCLUDE_DIRS}
        ${JNI_HEADER_DIR}
        ${UL_INCLUDE}
        include   # project-local helpers
)

# Link against Ultralight (user-provided)
target_link_directories(LuminescenceJNI PRIVATE ${UL_LIB_DIR})
target_link_libraries(LuminescenceJNI PRIVATE
        Ultralight
        UltralightCore
        WebCore
        AppCore          # remove if not using AppCore
)

# Export only JNI symbols, keep everything else hidden
set_target_properties(LuminescenceJNI PROPERTIES
        CXX_VISIBILITY_PRESET hidden
        VISIBILITY_INLINES_HIDDEN ON
)