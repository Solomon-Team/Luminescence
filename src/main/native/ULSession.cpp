#include "Core/JNIUtilities.h"
#include "Core/Profiling.h"

#include <Ultralight/CAPI/CAPI_Session.h>

/*
 * Class:     me_ayydxn_luminescence_renderer_ULSession_NativeMethods
 * Method:    nulCreateSession
 * Signature: (JZLjava/lang/String;)J
 */
jlong JNICALL ULCreateSession_Native(JNIEnv* Environment, jclass, jlong RendererHandle, jboolean bIsPersistent, jstring Name)
{
    ZoneScoped
    
    const auto Renderer = reinterpret_cast<ULRenderer>(RendererHandle);
    const ULString SessionName = Luminescence::JavaStringToULString(Environment, Name);
    
    return reinterpret_cast<jlong>(ulCreateSession(Renderer, bIsPersistent, SessionName));
}

/*
 * Class:     me_ayydxn_luminescence_renderer_ULSession_NativeMethods
 * Method:    nulDestroySession
 * Signature: (J)V
 */
void JNICALL ULDestroySession_Native(JNIEnv*, jclass, jlong handle)
{
    ZoneScoped
    
    ulDestroySession(reinterpret_cast<ULSession>(handle));
}

/*
 * Class:     me_ayydxn_luminescence_renderer_ULSession_NativeMethods
 * Method:    nulDefaultSession
 * Signature: (J)J
 */
jlong JNICALL ULDefaultSession_Native(JNIEnv*, jclass, jlong RendererHandle)
{
    ZoneScoped
    
    return reinterpret_cast<jlong>(ulDefaultSession(reinterpret_cast<ULRenderer>(RendererHandle)));
}

/*
 * Class:     me_ayydxn_luminescence_renderer_ULSession_NativeMethods
 * Method:    nulSessionIsPersistent
 * Signature: (J)Z
 */
jboolean JNICALL ULSessionIsPersistent_Native(JNIEnv*, jclass, jlong SessionHandle)
{
    ZoneScoped
    
    return ulSessionIsPersistent(reinterpret_cast<ULSession>(SessionHandle));
}

/*
 * Class:     me_ayydxn_luminescence_renderer_ULSession_NativeMethods
 * Method:    nulSessionGetName
 * Signature: (J)Ljava/lang/String;
 */
jstring JNICALL ULSessionGetName_Native(JNIEnv* Environment, jclass, jlong SessionHandle)
{
    ZoneScoped
    
    const ULString Name = ulSessionGetName(reinterpret_cast<ULSession>(SessionHandle));
    
    return Luminescence::ULStringToJavaString(Environment, Name);
}

/*
 * Class:     me_ayydxn_luminescence_renderer_ULSession_NativeMethods
 * Method:    nulSessionGetID
 * Signature: (J)J
 */
jlong JNICALL ULSessionGetID_Native(JNIEnv*, jclass, jlong SessionHandle)
{
    ZoneScoped
    
    return static_cast<jlong>(ulSessionGetId(reinterpret_cast<ULSession>(SessionHandle)));
}

/*
 * Class:     me_ayydxn_luminescence_renderer_ULSession_NativeMethods
 * Method:    nulSessionGetDiskPath
 * Signature: (J)Ljava/lang/String;
 */
jstring JNICALL ULSessionGetDiskPath_Native(JNIEnv* Environment, jclass, jlong SessionHandle)
{
    ZoneScoped
    
    const ULString Path = ulSessionGetDiskPath(reinterpret_cast<ULSession>(SessionHandle));
    
    return Luminescence::ULStringToJavaString(Environment, Path);
}

/* -- Session Method Array -- */

static constexpr JNINativeMethod SessionMethods[] = {
    JNI_METHOD("nulCreateSession", "(JZLjava/lang/String;)J", ULCreateSession_Native),
    JNI_METHOD("nulDestroySession", "(J)V", ULDestroySession_Native),
    JNI_METHOD("nulDefaultSession", "(J)J", ULDefaultSession_Native),
    JNI_METHOD("nulSessionIsPersistent", "(J)Z", ULSessionIsPersistent_Native),
    JNI_METHOD("nulSessionGetName", "(J)Ljava/lang/String;", ULSessionGetName_Native),
    JNI_METHOD("nulSessionGetID", "(J)J", ULSessionGetID_Native),
    JNI_METHOD("nulSessionGetDiskPath", "(J)Ljava/lang/String;", ULSessionGetDiskPath_Native)
};

bool Luminescence::RegisterSessionMethods(JNIEnv* Environment)
{
    return RegisterNativeMethods(Environment, "me/ayydxn/luminescence/renderer/ULSession$NativeMethods", JNI_METHODS_AND_COUNT(SessionMethods));
}
