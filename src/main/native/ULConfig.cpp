//
// Created by Ayydxn on 4/6/2026.
//

#include "JNIUtilities.h"

#include <Ultralight/CAPI/CAPI_Config.h>

/*
 * Class:     me_ayydxn_luminescence_config_Config_NativeMethods
 * Method:    nulCreateConfig
 * Signature: ()J
 */
jlong JNICALL ULCreateConfig_Native(JNIEnv*, jclass)
{
    return reinterpret_cast<jlong>(ulCreateConfig());
}

/*
 * Class:     me_ayydxn_luminescence_config_Config_NativeMethods
 * Method:    nulDestroyConfig
 * Signature: (J)V
 */
void JNICALL ULDestroyConfig_Native(JNIEnv*, jclass, jlong Config)
{
    ulDestroyConfig(reinterpret_cast<ULConfig>(Config));
}

/*
 * Class:     me_ayydxn_luminescence_config_Config_NativeMethods
 * Method:    nulConfigSetCachePath
 * Signature: (JJ)V
 */
void JNICALL ULConfigSetCachePath_Native(JNIEnv*, jclass, jlong ConfigHandle, jlong CachePathStringHandle)
{
    const auto Config = reinterpret_cast<ULConfig>(ConfigHandle);
    const auto CachePathString = reinterpret_cast<ULString>(CachePathStringHandle);

    ulConfigSetCachePath(Config, CachePathString);
}

/*
 * Class:     me_ayydxn_luminescence_config_Config_NativeMethods
 * Method:    nulConfigSetResourcePathPrefix
 * Signature: (JJ)V
 */
void JNICALL ULConfigSetResourcePathPrefix_Native(JNIEnv*, jclass, jlong ConfigHandle, jlong ResourcePathPrefixULString)
{
    const auto Config = reinterpret_cast<ULConfig>(ConfigHandle);
    const auto ResourcePathPrefix = reinterpret_cast<ULString>(ResourcePathPrefixULString);

    ulConfigSetResourcePathPrefix(Config, ResourcePathPrefix);
}

/*
 * Class:     me_ayydxn_luminescence_config_Config_NativeMethods
 * Method:    nulConfigSetFaceWinding
 * Signature: (JI)V
 */
void JNICALL ULConfigSetFaceWinding_Native(JNIEnv*, jclass, jlong ConfigHandle, jint FaceWindingValue)
{
    const auto Config = reinterpret_cast<ULConfig>(ConfigHandle);
    const auto FaceWinding = static_cast<ULFaceWinding>(FaceWindingValue);

    ulConfigSetFaceWinding(Config, FaceWinding);
}

/*
 * Class:     me_ayydxn_luminescence_config_Config_NativeMethods
 * Method:    nulConfigSetFontHinting
 * Signature: (JI)V
 */
void JNICALL ULConfigSetFontHinting_Native(JNIEnv*, jclass, jlong ConfigHandle, jint FontHinting)
{
    const auto Config = reinterpret_cast<ULConfig>(ConfigHandle);

    ulConfigSetFontHinting(Config, static_cast<ULFontHinting>(FontHinting));
}

/*
 * Class:     me_ayydxn_luminescence_config_Config_NativeMethods
 * Method:    nulConfigSetFontGamma
 * Signature: (JD)V
 */
void JNICALL ULConfigSetFontGamma_Native(JNIEnv*, jclass, jlong ConfigHandle, jdouble FontGamma)
{
    const auto Config = reinterpret_cast<ULConfig>(ConfigHandle);

    ulConfigSetFontGamma(Config, FontGamma);
}

/*
 * Class:     me_ayydxn_luminescence_config_Config_NativeMethods
 * Method:    nulConfigSetUserStylesheet
 * Signature: (JJ)V
 */
void JNICALL ULConfigSetUserStylesheet_Native(JNIEnv*, jclass, jlong ConfigHandle, jlong UserStylesheetULString)
{
    const auto Config = reinterpret_cast<ULConfig>(ConfigHandle);
    const auto UserStylesheetString = reinterpret_cast<ULString>(UserStylesheetULString);

    ulConfigSetUserStylesheet(Config, UserStylesheetString);
}

/*
 * Class:     me_ayydxn_luminescence_config_Config_NativeMethods
 * Method:    nulConfigSetForceRepaint
 * Signature: (JZ)V
 */
void JNICALL ULConfigSetForceRepaint_Native(JNIEnv*, jclass, jlong ConfigHandle, jboolean bEnableForceRepaint)
{
    const auto Config = reinterpret_cast<ULConfig>(ConfigHandle);
    
    ulConfigSetForceRepaint(Config, static_cast<bool>(bEnableForceRepaint));
}

/*
 * Class:     me_ayydxn_luminescence_config_Config_NativeMethods
 * Method:    nulConfigSetAnimationTimerDelay
 * Signature: (JD)V
 */
void JNICALL ULConfigSetAnimationTimerDelay_Native(JNIEnv*, jclass, jlong ConfigHandle, jdouble AnimationTimerDelay)
{
    const auto Config = reinterpret_cast<ULConfig>(ConfigHandle);
    
    ulConfigSetAnimationTimerDelay(Config, AnimationTimerDelay);
}

/*
 * Class:     me_ayydxn_luminescence_config_Config_NativeMethods
 * Method:    nulConfigSetScrollTimerDelay
 * Signature: (JD)V
 */
void JNICALL ULConfigSetScrollTimerDelay_Native(JNIEnv*, jclass, jlong ConfigHandle, jdouble ScrollTimerDelay)
{
    const auto Config = reinterpret_cast<ULConfig>(ConfigHandle);
    
    ulConfigSetScrollTimerDelay(Config, ScrollTimerDelay);
}

/*
 * Class:     me_ayydxn_luminescence_config_Config_NativeMethods
 * Method:    nulConfigSetRecycleDelay
 * Signature: (JD)V
 */
void JNICALL ULConfigSetRecycleDelay_Native(JNIEnv*, jclass, jlong ConfigHandle, jdouble RecycleDelay)
{
    const auto Config = reinterpret_cast<ULConfig>(ConfigHandle);
    
    ulConfigSetRecycleDelay(Config, RecycleDelay);
}

/*
 * Class:     me_ayydxn_luminescence_config_Config_NativeMethods
 * Method:    nulConfigSetMemoryCacheSize
 * Signature: (JI)V
 */
void JNICALL ULConfigSetMemoryCacheSize_Native(JNIEnv*, jclass, jlong ConfigHandle, jint MemoryCacheSize)
{
    const auto Config = reinterpret_cast<ULConfig>(ConfigHandle);
    
    ulConfigSetMemoryCacheSize(Config, static_cast<unsigned int>(MemoryCacheSize));
}

/*
 * Class:     me_ayydxn_luminescence_config_Config_NativeMethods
 * Method:    nulConfigSetPageCacheSize
 * Signature: (JI)V
 */
void JNICALL ULConfigSetPageCacheSize_Native(JNIEnv*, jclass, jlong ConfigHandle, jint PageCacheSize)
{
    const auto Config = reinterpret_cast<ULConfig>(ConfigHandle);
    
    ulConfigSetPageCacheSize(Config, static_cast<unsigned int>(PageCacheSize));
}

/*
 * Class:     me_ayydxn_luminescence_config_Config_NativeMethods
 * Method:    nulConfigSetOverrideRAMSize
 * Signature: (JI)V
 */
void JNICALL ULConfigSetOverrideRAMSize_Native(JNIEnv*, jclass, jlong ConfigHandle, jint RamSize)
{
    const auto Config = reinterpret_cast<ULConfig>(ConfigHandle);
    
    ulConfigSetOverrideRAMSize(Config, static_cast<unsigned int>(RamSize));
}

/*
 * Class:     me_ayydxn_luminescence_config_Config_NativeMethods
 * Method:    nulConfigSetMinLargeHeapSize
 * Signature: (JI)V
 */
void JNICALL ULConfigSetMinLargeHeapSize_Native(JNIEnv*, jclass, jlong ConfigHandle, jint MinLargeHeapSize)
{
    const auto Config = reinterpret_cast<ULConfig>(ConfigHandle);
    
    ulConfigSetMinLargeHeapSize(Config, static_cast<unsigned int>(MinLargeHeapSize));
}

/*
 * Class:     me_ayydxn_luminescence_config_Config_NativeMethods
 * Method:    nulConfigSetMinSmallHeapSize
 * Signature: (JI)V
 */
void JNICALL ULConfigSetMinSmallHeapSize_Native(JNIEnv*, jclass, jlong ConfigHandle, jint MinSmallHeapSize)
{
    const auto Config = reinterpret_cast<ULConfig>(ConfigHandle);
    
    ulConfigSetMinSmallHeapSize(Config, static_cast<unsigned int>(MinSmallHeapSize));
}

/*
 * Class:     me_ayydxn_luminescence_config_Config_NativeMethods
 * Method:    nulConfigSetNumRendererThreads
 * Signature: (JI)V
 */
void JNICALL ULConfigSetNumRendererThreads_Native(JNIEnv*, jclass, jlong ConfigHandle, jint NumberOfRendererThreads)
{
    const auto Config = reinterpret_cast<ULConfig>(ConfigHandle);
    
    ulConfigSetNumRendererThreads(Config, static_cast<unsigned int>(NumberOfRendererThreads));
}

/*
 * Class:     me_ayydxn_luminescence_config_Config_NativeMethods
 * Method:    nulConfigSetMaxUpdateTime
 * Signature: (JD)V
 */
void JNICALL ULConfigSetMaxUpdateTime_Native(JNIEnv*, jclass, jlong ConfigHandle, jdouble MaxUpdateTime)
{
    const auto Config = reinterpret_cast<ULConfig>(ConfigHandle);
    
    ulConfigSetMaxUpdateTime(Config, MaxUpdateTime);
}

/*
 * Class:     me_ayydxn_luminescence_config_Config_NativeMethods
 * Method:    nulConfigSetBitmapAlignment
 * Signature: (JI)V
 */
void JNICALL ULConfigSetBitmapAlignment_Native(JNIEnv*, jclass, jlong ConfigHandle, jint BitmapAlignment)
{
    const auto Config = reinterpret_cast<ULConfig>(ConfigHandle);
    
    ulConfigSetBitmapAlignment(Config, static_cast<unsigned int>(BitmapAlignment));
}

static constexpr JNINativeMethod ConfigMethods[] =
{
    JNI_METHOD("nulCreateConfig", "()J", ULCreateConfig_Native),
    JNI_METHOD("nulDestroyConfig", "(J)V", ULDestroyConfig_Native),
    JNI_METHOD("nulConfigSetCachePath", "(JJ)V", ULConfigSetCachePath_Native),
    JNI_METHOD("nulConfigSetResourcePathPrefix", "(JJ)V", ULConfigSetResourcePathPrefix_Native),
    JNI_METHOD("nulConfigSetFaceWinding", "(JI)V", ULConfigSetFaceWinding_Native),
    JNI_METHOD("nulConfigSetFontHinting", "(JI)V", ULConfigSetFontHinting_Native),
    JNI_METHOD("nulConfigSetFontGamma", "(JD)V", ULConfigSetFontGamma_Native),
    JNI_METHOD("nulConfigSetUserStylesheet", "(JJ)V", ULConfigSetUserStylesheet_Native),
    JNI_METHOD("nulConfigSetForceRepaint", "(JZ)V", ULConfigSetForceRepaint_Native),
    JNI_METHOD("nulConfigSetAnimationTimerDelay", "(JD)V", ULConfigSetAnimationTimerDelay_Native),
    JNI_METHOD("nulConfigSetScrollTimerDelay", "(JD)V", ULConfigSetScrollTimerDelay_Native),
    JNI_METHOD("nulConfigSetRecycleDelay", "(JD)V", ULConfigSetRecycleDelay_Native),
    JNI_METHOD("nulConfigSetMemoryCacheSize", "(JI)V", ULConfigSetMemoryCacheSize_Native),
    JNI_METHOD("nulConfigSetPageCacheSize", "(JI)V", ULConfigSetPageCacheSize_Native),
    JNI_METHOD("nulConfigSetOverrideRAMSize", "(JI)V", ULConfigSetOverrideRAMSize_Native),
    JNI_METHOD("nulConfigSetMinLargeHeapSize", "(JI)V", ULConfigSetMinLargeHeapSize_Native),
    JNI_METHOD("nulConfigSetMinSmallHeapSize", "(JI)V", ULConfigSetMinSmallHeapSize_Native),
    JNI_METHOD("nulConfigSetNumRendererThreads", "(JI)V", ULConfigSetNumRendererThreads_Native),
    JNI_METHOD("nulConfigSetMaxUpdateTime", "(JD)V", ULConfigSetMaxUpdateTime_Native),
    JNI_METHOD("nulConfigSetBitmapAlignment", "(JI)V", ULConfigSetBitmapAlignment_Native)
};

bool Luminescence::RegisterConfigMethods(JNIEnv* Environment)
{
    return RegisterNativeMethods(Environment, "me/ayydxn/luminescence/config/Config$NativeMethods",
                                 ConfigMethods, sizeof(ConfigMethods) / sizeof(ConfigMethods[0]));
}
