//
// Created by Ayydxn on 4/10/2026.
//

#include "Core/JNIUtilities.h"

#include <Ultralight/CAPI/CAPI_GamepadEvent.h>

/*
 * Class:     me_ayydxn_luminescence_events_ULGamepadEvent_NativeMethods
 * Method:    nulCreateGamepadEvent
 * Signature: (II)J
 */
JNIEXPORT jlong JNICALL ULCreateGamepadEvent_Native(JNIEnv *, jclass, jint Index, jint EventTypeOrdinal)
{
    return reinterpret_cast<jlong>(ulCreateGamepadEvent(static_cast<unsigned int>(Index), static_cast<ULGamepadEventType>(EventTypeOrdinal)));
}

/*
 * Class:     me_ayydxn_luminescence_events_ULGamepadEvent_NativeMethods
 * Method:    nulDestroyGamepadEvent
 * Signature: (J)V
 */
JNIEXPORT void JNICALL ULDestroyGamepadEvent_Native(JNIEnv*, jclass, jlong GamepadEventHandle)
{
    ulDestroyGamepadEvent(reinterpret_cast<ULGamepadEvent>(GamepadEventHandle));
}

/*
 * Class:     me_ayydxn_luminescence_events_ULGamepadEvent_NativeMethods
 * Method:    nulCreateGamepadAxisEvent
 * Signature: (IID)J
 */
JNIEXPORT jlong JNICALL ULCreateGamepadAxisEvent_Native(JNIEnv*, jclass, jint Index, jint AxisIndex, jdouble Value)
{
    return reinterpret_cast<jlong>(ulCreateGamepadAxisEvent(static_cast<unsigned int>(Index), static_cast<unsigned int>(AxisIndex), Value));
}

/*
 * Class:     me_ayydxn_luminescence_events_ULGamepadEvent_NativeMethods
 * Method:    nulDestroyGamepadAxisEvent
 * Signature: (J)V
 */
JNIEXPORT void JNICALL ULDestroyGamepadAxisEvent_Native(JNIEnv*, jclass, jlong GamepadAxisEventHandle)
{
    ulDestroyGamepadAxisEvent(reinterpret_cast<ULGamepadAxisEvent>(GamepadAxisEventHandle));
}

/*
 * Class:     me_ayydxn_luminescence_events_ULGamepadEvent_NativeMethods
 * Method:    nulCreateGamepadButtonEvent
 * Signature: (IID)J
 */
JNIEXPORT jlong JNICALL ULCreateGamepadButtonEvent_Native(JNIEnv*, jclass, jint Index, jint ButtonIndex, jdouble Value)
{
    return reinterpret_cast<jlong>(ulCreateGamepadButtonEvent(static_cast<unsigned int>(Index), static_cast<unsigned int>(ButtonIndex), Value));
}

/*
 * Class:     me_ayydxn_luminescence_events_ULGamepadEvent_NativeMethods
 * Method:    nulDestroyGamepadButtonEvent
 * Signature: (J)V
 */
JNIEXPORT void JNICALL ULDestroyGamepadButtonEvent_Native(JNIEnv *, jclass, jlong GamepadButtonEventHandle)
{
    ulDestroyGamepadButtonEvent(reinterpret_cast<ULGamepadButtonEvent>(GamepadButtonEventHandle)); 
}

static constexpr JNINativeMethod GamepadEventMethods[] =
{
    JNI_METHOD("nulCreateGamepadEvent", "(II)J", ULCreateGamepadEvent_Native),
    JNI_METHOD("nulDestroyGamepadEvent", "(J)V", ULDestroyGamepadEvent_Native),
    JNI_METHOD("nulCreateGamepadAxisEvent", "(IID)J", ULCreateGamepadAxisEvent_Native),
    JNI_METHOD("nulDestroyGamepadAxisEvent", "(J)V", ULDestroyGamepadAxisEvent_Native),
    JNI_METHOD("nulCreateGamepadButtonEvent", "(IID)J", ULCreateGamepadButtonEvent_Native),
    JNI_METHOD("nulDestroyGamepadButtonEvent", "(J)V", ULDestroyGamepadButtonEvent_Native)
};

bool Luminescence::RegisterGamepadEventMethods(JNIEnv* Environment)
{
    return RegisterNativeMethods(Environment, "me/ayydxn/luminescence/events/ULGamepadEvent$NativeMethods", JNI_METHODS_AND_COUNT(GamepadEventMethods));
}
