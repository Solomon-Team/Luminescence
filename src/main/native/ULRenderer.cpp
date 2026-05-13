//
// Created by Ayydxn on 4/10/2026.
//

#include "Core/JNIUtilities.h"
#include "Core/Profiling.h"

#include <Ultralight/CAPI/CAPI_Renderer.h>

/*
 * Class:     me_ayydxn_luminescence_renderer_ULRenderer_NativeMethods
 * Method:    nulCreateRenderer
 * Signature: (J)J
 */
jlong JNICALL ULCreateRenderer_Native(JNIEnv*, jclass, jlong ConfigHandle)
{
    ZoneScoped
    
    return reinterpret_cast<jlong>(ulCreateRenderer(reinterpret_cast<ULConfig>(ConfigHandle)));
}

/*
 * Class:     me_ayydxn_luminescence_renderer_ULRenderer_NativeMethods
 * Method:    nulDestroyRenderer
 * Signature: (J)V
 */
void JNICALL ULDestroyRenderer_Native(JNIEnv*, jclass, jlong RendererHandle)
{
    ZoneScoped
    
    ulDestroyRenderer(reinterpret_cast<ULRenderer>(RendererHandle));
}

/*
 * Class:     me_ayydxn_luminescence_renderer_ULRenderer_NativeMethods
 * Method:    nulUpdate
 * Signature: (J)V
 */
void JNICALL ULUpdate_Native(JNIEnv*, jclass, jlong RendererHandle)
{
    ZoneScoped
    
    ulUpdate(reinterpret_cast<ULRenderer>(RendererHandle));
}

/*
 * Class:     me_ayydxn_luminescence_renderer_ULRenderer_NativeMethods
 * Method:    nulRefreshDisplay
 * Signature: (JI)V
 */
void JNICALL ULRefreshDisplay_Native(JNIEnv*, jclass, jlong RendererHandle, jint DisplayID)
{
    ZoneScoped
    
    ulRefreshDisplay(reinterpret_cast<ULRenderer>(RendererHandle), static_cast<unsigned int>(DisplayID));
}

/*
 * Class:     me_ayydxn_luminescence_renderer_ULRenderer_NativeMethods
 * Method:    nulRender
 * Signature: (J)V
 */
void JNICALL ULRender_Native(JNIEnv*, jclass, jlong RendererHandle)
{
    ZoneScoped
    
    ulRender(reinterpret_cast<ULRenderer>(RendererHandle));
}

/*
 * Class:     me_ayydxn_luminescence_renderer_ULRenderer_NativeMethods
 * Method:    nulPurgeMemory
 * Signature: (J)V
 */
void JNICALL ULPurgeMemory_Native(JNIEnv*, jclass, jlong RendererHandle)
{
    ZoneScoped
    
    ulPurgeMemory(reinterpret_cast<ULRenderer>(RendererHandle));
}

/*
 * Class:     me_ayydxn_luminescence_renderer_ULRenderer_NativeMethods
 * Method:    nulLogMemoryUsage
 * Signature: (J)V
 */
void JNICALL ULLogMemoryUsage_Native(JNIEnv*, jclass, jlong RendererHandle)
{
    ZoneScoped
    
    ulLogMemoryUsage(reinterpret_cast<ULRenderer>(RendererHandle));
}

/*
 * Class:     me_ayydxn_luminescence_renderer_ULRenderer_NativeMethods
 * Method:    nulStartRemoteInspectorServer
 * Signature: (JLjava/lang/String;I)Z
 */
jboolean JNICALL ULStartRemoteInspectorServer_Native(JNIEnv* Environment, jclass, jlong RendererHandle, jstring Address, jint Port)
{
    ZoneScoped
    
    const auto Renderer = reinterpret_cast<ULRenderer>(RendererHandle);
    const auto AddressCString = Environment->GetStringUTFChars(Address, nullptr);
    
    const bool Result = ulStartRemoteInspectorServer(Renderer, AddressCString, static_cast<unsigned short>(Port));
    
    Environment->ReleaseStringUTFChars(Address, AddressCString);
    
    return Result;
}

/*
 * Class:     me_ayydxn_luminescence_renderer_ULRenderer_NativeMethods
 * Method:    nulSetGamepadDetails
 * Signature: (JILjava/lang/String;II)V
 */
void JNICALL ULSetGamepadDetails_Native(JNIEnv* Environment, jclass, jlong RendererHandle, jint Index, jstring ID, jint AxisCount, jint ButtonCount)
{
    ZoneScoped
    
    const auto Renderer = reinterpret_cast<ULRenderer>(RendererHandle);
    const auto IDCString = Environment->GetStringUTFChars(ID, nullptr);
    const auto IDString = ulCreateString(IDCString);
    
    ulSetGamepadDetails(Renderer, static_cast<unsigned int>(Index), IDString, static_cast<unsigned int>(AxisCount), static_cast<unsigned int>(ButtonCount));
    
    ulDestroyString(IDString);
    Environment->ReleaseStringUTFChars(ID, IDCString);
}

/*
 * Class:     me_ayydxn_luminescence_renderer_ULRenderer_NativeMethods
 * Method:    nulFireGamepadEvent
 * Signature: (JJ)V
 */
void JNICALL ULFireGamepadEvent_Native(JNIEnv*, jclass, jlong RendererHandle, jlong GamepadEventHandle)
{
    ZoneScoped
    
    const auto Renderer = reinterpret_cast<ULRenderer>(RendererHandle);
    
    ulFireGamepadEvent(Renderer, reinterpret_cast<ULGamepadEvent>(GamepadEventHandle));
}

/*
 * Class:     me_ayydxn_luminescence_renderer_ULRenderer_NativeMethods
 * Method:    nulFireGamepadAxisEvent
 * Signature: (JJ)V
 */
void JNICALL ULFireGamepadAxisEvent_Native(JNIEnv*, jclass, jlong RendererHandle, jlong GamepadAxisEventHandle)
{
    ZoneScoped
    
    const auto Renderer = reinterpret_cast<ULRenderer>(RendererHandle);
    
    ulFireGamepadAxisEvent(Renderer, reinterpret_cast<ULGamepadAxisEvent>(GamepadAxisEventHandle));
}

/*
 * Class:     me_ayydxn_luminescence_renderer_ULRenderer_NativeMethods
 * Method:    nulFireGamepadButtonEvent
 * Signature: (JJ)V
 */
void JNICALL ULFireGamepadButtonEvent_Native(JNIEnv*, jclass, jlong RendererHandle, jlong GamepadButtonEventHandle)
{
    ZoneScoped
    
    const auto Renderer = reinterpret_cast<ULRenderer>(RendererHandle);
    
    ulFireGamepadButtonEvent(Renderer, reinterpret_cast<ULGamepadButtonEvent>(GamepadButtonEventHandle));
}

static constexpr JNINativeMethod RendererMethods[] =
{
    JNI_METHOD("nulCreateRenderer", "(J)J", ULCreateRenderer_Native),
    JNI_METHOD("nulDestroyRenderer", "(J)V", ULDestroyRenderer_Native),
    JNI_METHOD("nulUpdate", "(J)V", ULUpdate_Native),
    JNI_METHOD("nulRefreshDisplay", "(JI)V", ULRefreshDisplay_Native),
    JNI_METHOD("nulRender", "(J)V", ULRender_Native),
    JNI_METHOD("nulPurgeMemory", "(J)V", ULPurgeMemory_Native),
    JNI_METHOD("nulLogMemoryUsage", "(J)V", ULLogMemoryUsage_Native),
    JNI_METHOD("nulStartRemoteInspectorServer", "(JLjava/lang/String;I)Z", ULStartRemoteInspectorServer_Native),
    JNI_METHOD("nulSetGamepadDetails", "(JILjava/lang/String;II)V", ULSetGamepadDetails_Native),
    JNI_METHOD("nulFireGamepadEvent", "(JJ)V", ULFireGamepadEvent_Native),
    JNI_METHOD("nulFireGamepadAxisEvent", "(JJ)V", ULFireGamepadAxisEvent_Native),
    JNI_METHOD("nulFireGamepadButtonEvent", "(JJ)V", ULFireGamepadButtonEvent_Native)
};

bool Luminescence::RegisterRendererMethods(JNIEnv* Environment)
{
    return RegisterNativeMethods(Environment, "me/ayydxn/luminescence/renderer/ULRenderer$NativeMethods", JNI_METHODS_AND_COUNT(RendererMethods));
}
