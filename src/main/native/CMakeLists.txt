cmake_minimum_required(VERSION 3.20)
project(luminescence_jni)

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
file(GLOB_RECURSE SOURCES "*.cpp")

add_library(luminescence_jni SHARED ${SOURCES})

target_include_directories(luminescence_jni PRIVATE
        ${JNI_INCLUDE_DIRS}
        ${JNI_HEADER_DIR}
        ${UL_INCLUDE}
        include   # project-local helpers
)

# Link against Ultralight (user-provided)
target_link_directories(luminescence_jni PRIVATE ${UL_LIB_DIR})
target_link_libraries(luminescence_jni PRIVATE
        Ultralight
        UltralightCore
        WebCore
        AppCore          # remove if not using AppCore
)

# Export only JNI symbols, keep everything else hidden
set_target_properties(luminescence_jni PROPERTIES
        CXX_VISIBILITY_PRESET hidden
        VISIBILITY_INLINES_HIDDEN ON
)