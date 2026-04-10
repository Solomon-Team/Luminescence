//
// Created by Ayydxn on 4/8/2026.
//

#include "Adapters/LoggerCallbackAdapter.h"
#include "Core/JNIUtilities.h"

#include <Ultralight/CAPI/CAPI_Platform.h>

#include <memory>

static std::unique_ptr<Luminescence::CLoggerCallbackAdapter> GLoggerCallbackAdapter;

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
 * Method:    nulPlatformShutdown
 * Signature: ()V
 */
void JNICALL ULPlatformShutdown_Native(JNIEnv*, jclass)
{
    GLoggerCallbackAdapter.reset();
}

static constexpr JNINativeMethod PlatformMethods[] =
{
    JNI_METHOD("nulPlatformSetLogger", "(Lme/ayydxn/luminescence/platform/ULLogger;)V", ULPlatformSetLogger_Native),
    JNI_METHOD("nulPlatformShutdown", "()V", ULPlatformShutdown_Native),
};

bool Luminescence::RegisterPlatformMethods(JNIEnv* Environment)
{
    return RegisterNativeMethods(Environment, "me/ayydxn/luminescence/platform/ULPlatform$NativeMethods", JNI_METHODS_AND_COUNT(PlatformMethods));
}
