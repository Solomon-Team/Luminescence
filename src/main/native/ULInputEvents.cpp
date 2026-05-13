#include "Core/JNIUtilities.h"
#include "Core/Profiling.h"

#include <Ultralight/CAPI/CAPI_KeyEvent.h>
#include <Ultralight/CAPI/CAPI_MouseEvent.h>
#include <Ultralight/CAPI/CAPI_ScrollEvent.h>

/*
 * Class:     me_ayydxn_luminescence_events_ULKeyEvent_NativeMethods
 * Method:    nulCreateKeyEvent
 * Signature: (IIIILjava/lang/String;Ljava/lang/String;ZZZ)J
 */
jlong JNICALL ULCreateKeyEvent_Native(JNIEnv* Environment, jclass, jint EventTypeOrdinal, jint Modifiers, jint VirtualKeyCode, jint NativeKeyCode, jstring Text,
    jstring UnmodifiedText, jboolean bIsKeyPad, jboolean bIsAutoRepeat, jboolean bIsSystemKey)
{
    ZoneScoped
    
    const auto TextChars = Environment->GetStringUTFChars(Text, nullptr);
    const auto UnmodifiedTextChars = Environment->GetStringUTFChars(UnmodifiedText, nullptr);
    
    const auto TextString = ulCreateString(TextChars);
    const auto UnmodifiedTextString = ulCreateString(UnmodifiedTextChars);
    
    const auto Handle = reinterpret_cast<jlong>(ulCreateKeyEvent(
        static_cast<ULKeyEventType>(EventTypeOrdinal),
        static_cast<unsigned int>(Modifiers),
        VirtualKeyCode,
        NativeKeyCode,
        TextString,
        UnmodifiedTextString,
        static_cast<bool>(bIsKeyPad),
        static_cast<bool>(bIsAutoRepeat),
        static_cast<bool>(bIsSystemKey)
    ));
    
    ulDestroyString(UnmodifiedTextString);
    ulDestroyString(TextString);
    
    Environment->ReleaseStringUTFChars(UnmodifiedText, UnmodifiedTextChars);
    Environment->ReleaseStringUTFChars(Text, TextChars);
    
    return Handle;
}

/*
 * Class:     me_ayydxn_luminescence_events_ULKeyEvent_NativeMethods
 * Method:    nulDestroyKeyEvent
 * Signature: (J)V
 */
void JNICALL ULDestroyKeyEvent_Native(JNIEnv*, jclass, jlong KeyEventHandle)
{
    ZoneScoped
    
    ulDestroyKeyEvent(reinterpret_cast<ULKeyEvent>(KeyEventHandle));
}

/*
 * Class:     me_ayydxn_luminescence_events_ULMouseEvent_NativeMethods
 * Method:    nulCreateMouseEvent
 * Signature: (IIII)J
 */
jlong JNICALL ULCreateMouseEvent_Native(JNIEnv*, jclass, jint EventTypeOrdinal, jint X, jint Y, jint MouseButtonOrdinal)
{
    ZoneScoped
    
    return reinterpret_cast<jlong>(ulCreateMouseEvent(static_cast<ULMouseEventType>(EventTypeOrdinal), X, Y, static_cast<ULMouseButton>(MouseButtonOrdinal)));
}

/*
 * Class:     me_ayydxn_luminescence_events_ULMouseEvent_NativeMethods
 * Method:    nulDestroyMouseEvent
 * Signature: (J)V
 */
void JNICALL ULDestroyMouseEvent_Native(JNIEnv*, jclass, jlong MouseEventHandle)
{
    ZoneScoped
    
    ulDestroyMouseEvent(reinterpret_cast<ULMouseEvent>(MouseEventHandle));
}

/*
 * Class:     me_ayydxn_luminescence_events_ULScrollEvent_NativeMethods
 * Method:    nulCreateScrollEvent
 * Signature: (III)J
 */
jlong JNICALL ULCreateScrollEvent_Native(JNIEnv*, jclass, jint EventTypeOrdinal, jint DeltaX, jint DeltaY)
{
    ZoneScoped
    
    return reinterpret_cast<jlong>(ulCreateScrollEvent(static_cast<ULScrollEventType>(EventTypeOrdinal), DeltaX, DeltaY));
}

/*
 * Class:     me_ayydxn_luminescence_events_ULScrollEvent_NativeMethods
 * Method:    nulDestroyScrollEvent
 * Signature: (J)V
 */
void JNICALL ULDestroyScrollEvent_Native(JNIEnv*, jclass, jlong ScrollEventHandle)
{
    ZoneScoped
    
    ulDestroyScrollEvent(reinterpret_cast<ULScrollEvent>(ScrollEventHandle));
}

static constexpr JNINativeMethod KeyEventMethods[] = {
    JNI_METHOD("nulCreateKeyEvent", "(IIIILjava/lang/String;Ljava/lang/String;ZZZ)J", ULCreateKeyEvent_Native),
    JNI_METHOD("nulDestroyKeyEvent", "(J)V", ULDestroyKeyEvent_Native)
};

static constexpr JNINativeMethod MouseEventMethods[] = {
    JNI_METHOD("nulCreateMouseEvent", "(IIII)J", ULCreateMouseEvent_Native),
    JNI_METHOD("nulDestroyMouseEvent", "(J)V", ULDestroyMouseEvent_Native)
};

static constexpr JNINativeMethod ScrollEventMethods[] = {
    JNI_METHOD("nulCreateScrollEvent", "(III)J", ULCreateScrollEvent_Native),
    JNI_METHOD("nulDestroyScrollEvent", "(J)V", ULDestroyScrollEvent_Native)
};

bool Luminescence::RegisterInputEventMethods(JNIEnv* Environment) {
    const bool bKeyEventMethodsRegistration = RegisterNativeMethods(Environment, "me/ayydxn/luminescence/events/ULKeyEvent$NativeMethods",
        JNI_METHODS_AND_COUNT(KeyEventMethods));

    const bool bMouseEventMethodsRegistration = RegisterNativeMethods(Environment, "me/ayydxn/luminescence/events/ULMouseEvent$NativeMethods", 
        JNI_METHODS_AND_COUNT(MouseEventMethods));

    const bool bScrollEventMethodsRegistration = RegisterNativeMethods(Environment, "me/ayydxn/luminescence/events/ULScrollEvent$NativeMethods", 
        JNI_METHODS_AND_COUNT(ScrollEventMethods));

    return bKeyEventMethodsRegistration && bMouseEventMethodsRegistration && bScrollEventMethodsRegistration;
}