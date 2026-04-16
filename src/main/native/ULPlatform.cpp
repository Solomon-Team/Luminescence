//
// Created by Ayydxn on 4/8/2026.
//

#include "Adapters/ClipboardCallbackAdapter.h"
#include "Adapters/FileSystemCallbackAdapter.h"
#include "Adapters/FontLoaderCallbackAdapter.h"
#include "Adapters/GPUDriverCallbackAdapter.h"
#include "Adapters/LoggerCallbackAdapter.h"
#include "Core/JNIUtilities.h"

#include <Ultralight/CAPI/CAPI_Platform.h>

#include <memory>

static std::unique_ptr<Luminescence::CLoggerCallbackAdapter> GLoggerCallbackAdapter;
static std::unique_ptr<Luminescence::CFileSystemCallbackAdapter> GFileSystemCallbackAdapter;
static std::unique_ptr<Luminescence::CClipboardCallbackAdapter> GClipboardCallbackAdapter;
static std::unique_ptr<Luminescence::CFontLoaderCallbackAdapter> GFontLoaderCallbackAdapter;
static std::unique_ptr<Luminescence::CGPUDriverCallbackAdapter> GGPUDriverCallbackAdapter;

/*
 * Class:     me_ayydxn_luminescence_platform_ULPlatform_NativeMethods
 * Method:    nulPlatformSetLogger
 * Signature: (Lme/ayydxn/luminescence/platform/ULLogger;)V
 */
void JNICALL ULPlatformSetLogger_Native(JNIEnv* Environment, jclass, jobject LoggerJavaInstance)
{
    GLoggerCallbackAdapter = std::make_unique<Luminescence::CLoggerCallbackAdapter>(Environment, LoggerJavaInstance);

    ulPlatformSetLogger(GLoggerCallbackAdapter->MakeStruct());
}

/*
 * Class:     me_ayydxn_luminescence_platform_ULPlatform_NativeMethods
 * Method:    nulPlatformSetFileSystem
 * Signature: (Lme/ayydxn/luminescence/platform/ULFileSystem;)V
 */
void JNICALL ULPlatformSetFileSystem_Native(JNIEnv* Environment, jclass, jobject FileSystemJavaInstance)
{
    GFileSystemCallbackAdapter = std::make_unique<Luminescence::CFileSystemCallbackAdapter>(Environment, FileSystemJavaInstance);
    
    ulPlatformSetFileSystem(GFileSystemCallbackAdapter->MakeStruct());
}

/*
 * Class:     me_ayydxn_luminescence_platform_ULPlatform_NativeMethods
 * Method:    nulPlatformSetClipboard
 * Signature: (Lme/ayydxn/luminescence/platform/ULClipboard;)V
 */
void JNICALL ULPlatformSetClipboard_Native(JNIEnv* Environment, jclass, jobject ClipboardJavaInstance)
{
    GClipboardCallbackAdapter = std::make_unique<Luminescence::CClipboardCallbackAdapter>(Environment, ClipboardJavaInstance);
    
    ulPlatformSetClipboard(GClipboardCallbackAdapter->MakeStruct());
}

/*
 * Class:     me_ayydxn_luminescence_platform_ULPlatform_NativeMethods
 * Method:    nulPlatformSetFontLoader
 * Signature: (Lme/ayydxn/luminescence/platform/ULFontLoader;)V
 */
void JNICALL ULPlatformSetFontLoader_Native(JNIEnv* Environment, jclass, jobject FontLoaderJavaInstance)
{
    GFontLoaderCallbackAdapter = std::make_unique<Luminescence::CFontLoaderCallbackAdapter>(Environment, FontLoaderJavaInstance);
    
    ulPlatformSetFontLoader(GFontLoaderCallbackAdapter->MakeStruct());
}

/*
 * Class:     me_ayydxn_luminescence_platform_ULPlatform_NativeMethods
 * Method:    nulPlatformSetGPUDriver
 * Signature: (Lme/ayydxn/luminescence/platform/ULGPUDriver;)V
 */
void JNICALL ULPlatformSetGPUDriver_Native(JNIEnv* Environment, jclass, jobject GPUDriverJavaInstance)
{
    GGPUDriverCallbackAdapter = std::make_unique<Luminescence::CGPUDriverCallbackAdapter>(Environment, GPUDriverJavaInstance);

    ulPlatformSetGPUDriver(GGPUDriverCallbackAdapter->MakeStruct());
}

/*
 * Class:     me_ayydxn_luminescence_platform_ULPlatform_NativeMethods
 * Method:    nulPlatformShutdown
 * Signature: ()V
 */
void JNICALL ULPlatformShutdown_Native(JNIEnv*, jclass)
{
    GLoggerCallbackAdapter.reset();
    GFileSystemCallbackAdapter.reset();
    GClipboardCallbackAdapter.reset();
    GFontLoaderCallbackAdapter.reset();
    GGPUDriverCallbackAdapter.reset();
}

static constexpr JNINativeMethod PlatformMethods[] =
{
    JNI_METHOD("nulPlatformSetLogger", "(Lme/ayydxn/luminescence/platform/ULLogger;)V", ULPlatformSetLogger_Native),
    JNI_METHOD("nulPlatformSetFileSystem", "(Lme/ayydxn/luminescence/platform/ULFileSystem;)V", ULPlatformSetFileSystem_Native),
    JNI_METHOD("nulPlatformSetClipboard", "(Lme/ayydxn/luminescence/platform/ULClipboard;)V", ULPlatformSetClipboard_Native),
    JNI_METHOD("nulPlatformSetFontLoader", "(Lme/ayydxn/luminescence/platform/ULFontLoader;)V", ULPlatformSetFontLoader_Native),
    JNI_METHOD("nulPlatformSetGPUDriver", "(Lme/ayydxn/luminescence/platform/ULGPUDriver;)V", ULPlatformSetGPUDriver_Native),
    JNI_METHOD("nulPlatformShutdown", "()V", ULPlatformShutdown_Native),
};

bool Luminescence::RegisterPlatformMethods(JNIEnv* Environment)
{
    return RegisterNativeMethods(Environment, "me/ayydxn/luminescence/platform/ULPlatform$NativeMethods", JNI_METHODS_AND_COUNT(PlatformMethods));
}
