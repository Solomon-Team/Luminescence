#include "Core/JNIUtilities.h"

#include <Ultralight/CAPI/CAPI_View.h>

/*
 * Class:     me_ayydxn_luminescence_view_ULViewConfig_NativeMethods
 * Method:    nulCreateViewConfig
 * Signature: ()J
 */
jlong JNICALL ULCreateViewConfig_Native(JNIEnv*, jclass)
{
    return reinterpret_cast<jlong>(ulCreateViewConfig());
}

/*
 * Class:     me_ayydxn_luminescence_view_ULViewConfig_NativeMethods
 * Method:    nulDestroyViewConfig
 * Signature: (J)V
 */
void JNICALL ULDestroyViewConfig_Native(JNIEnv*, jclass, jlong ViewConfigHandle)
{
    ulDestroyViewConfig(reinterpret_cast<ULViewConfig>(ViewConfigHandle));
}

/*
 * Class:     me_ayydxn_luminescence_view_ULViewConfig_NativeMethods
 * Method:    nulViewConfigSetDisplayID
 * Signature: (JI)V
 */
void JNICALL ULViewConfigSetDisplayID_Native(JNIEnv*, jclass, jlong ViewConfigHandle, jint DisplayID)
{
    ulViewConfigSetDisplayId(reinterpret_cast<ULViewConfig>(ViewConfigHandle), static_cast<unsigned int>(DisplayID));
}

/*
 * Class:     me_ayydxn_luminescence_view_ULViewConfig_NativeMethods
 * Method:    nulViewConfigIsAccelerated
 * Signature: (JZ)V
 */
void JNICALL ULViewConfigIsAccelerated_Native(JNIEnv*, jclass, jlong ViewConfigHandle, jboolean bIsAccelerated)
{
    ulViewConfigSetIsAccelerated(reinterpret_cast<ULViewConfig>(ViewConfigHandle), static_cast<bool>(bIsAccelerated));
}

/*
 * Class:     me_ayydxn_luminescence_view_ULViewConfig_NativeMethods
 * Method:    nulViewConfigSetIsTransparent
 * Signature: (JZ)V
 */
void JNICALL ULViewConfigSetIsTransparent_Native(JNIEnv*, jclass, jlong ViewConfigHandle, jboolean bIsTransparent)
{
    ulViewConfigSetIsTransparent(reinterpret_cast<ULViewConfig>(ViewConfigHandle), static_cast<bool>(bIsTransparent));
}

/*
 * Class:     me_ayydxn_luminescence_view_ULViewConfig_NativeMethods
 * Method:    nulViewConfigSetInitialDeviceScale
 * Signature: (JD)V
 */
void JNICALL ULViewConfigSetInitialDeviceScale_Native(JNIEnv*, jclass, jlong ViewConfigHandle, jdouble InitialDeviceScale)
{
    ulViewConfigSetInitialDeviceScale(reinterpret_cast<ULViewConfig>(ViewConfigHandle), InitialDeviceScale);
}

/*
 * Class:     me_ayydxn_luminescence_view_ULViewConfig_NativeMethods
 * Method:    nulViewConfigSetInitialFocus
 * Signature: (JZ)V
 */
void JNICALL ULViewConfigSetInitialFocus_Native(JNIEnv*, jclass, jlong ViewConfigHandle, jboolean bIsFocused)
{
    ulViewConfigSetInitialFocus(reinterpret_cast<ULViewConfig>(ViewConfigHandle), static_cast<bool>(bIsFocused));
}

/*
 * Class:     me_ayydxn_luminescence_view_ULViewConfig_NativeMethods
 * Method:    nulViewConfigSetEnableImages
 * Signature: (JZ)V
 */
void JNICALL ULViewConfigSetEnableImages_Native(JNIEnv*, jclass, jlong ViewConfigHandle, jboolean bEnableImages)
{
    ulViewConfigSetEnableImages(reinterpret_cast<ULViewConfig>(ViewConfigHandle), static_cast<bool>(bEnableImages));
}

/*
 * Class:     me_ayydxn_luminescence_view_ULViewConfig_NativeMethods
 * Method:    nulViewConfigSetEnableJavaScript
 * Signature: (JZ)V
 */
void JNICALL ULViewConfigSetEnableJavaScript_Native(JNIEnv*, jclass, jlong ViewConfigHandle, jboolean bEnableJavaScript)
{
    ulViewConfigSetEnableJavaScript(reinterpret_cast<ULViewConfig>(ViewConfigHandle), static_cast<bool>(bEnableJavaScript));
}

/*
 * Class:     me_ayydxn_luminescence_view_ULViewConfig_NativeMethods
 * Method:    nulViewConfigSetFontFamilyStandard
 * Signature: (JLjava/lang/String;)V
 */
void JNICALL ULViewConfigSetFontFamilyStandard_Native(JNIEnv* Environment, jclass, jlong ViewConfigHandle, jstring FontName)
{
    const auto ViewConfig = reinterpret_cast<ULViewConfig>(ViewConfigHandle);
    const auto FontNameChars = Environment->GetStringUTFChars(FontName, nullptr);
    const auto FontNameString = ulCreateString(FontNameChars);
    
    ulViewConfigSetFontFamilyStandard(ViewConfig, FontNameString);
    
    ulDestroyString(FontNameString);
    Environment->ReleaseStringUTFChars(FontName, FontNameChars);
}

/*
 * Class:     me_ayydxn_luminescence_view_ULViewConfig_NativeMethods
 * Method:    nulViewConfigSetFontFamilyFixed
 * Signature: (JLjava/lang/String;)V
 */
void JNICALL ULViewConfigSetFontFamilyFixed_Native(JNIEnv* Environment, jclass, jlong ViewConfigHandle, jstring FontName)
{
    const auto ViewConfig = reinterpret_cast<ULViewConfig>(ViewConfigHandle);
    const auto FontNameChars = Environment->GetStringUTFChars(FontName, nullptr);
    const auto FontNameString = ulCreateString(FontNameChars);
    
    ulViewConfigSetFontFamilyFixed(ViewConfig, FontNameString);
    
    ulDestroyString(FontNameString);
    Environment->ReleaseStringUTFChars(FontName, FontNameChars);
}

/*
 * Class:     me_ayydxn_luminescence_view_ULViewConfig_NativeMethods
 * Method:    nulViewConfigSetFontFamilySansSerif
 * Signature: (JLjava/lang/String;)V
 */
void JNICALL ULViewConfigSetFontFamilySansSerif_Native(JNIEnv* Environment, jclass, jlong ViewConfigHandle, jstring FontName)
{
    const auto ViewConfig = reinterpret_cast<ULViewConfig>(ViewConfigHandle);
    const auto FontNameChars = Environment->GetStringUTFChars(FontName, nullptr);
    const auto FontNameString = ulCreateString(FontNameChars);
    
    ulViewConfigSetFontFamilySansSerif(ViewConfig, FontNameString);
    
    ulDestroyString(FontNameString);
    Environment->ReleaseStringUTFChars(FontName, FontNameChars);
}

/*
 * Class:     me_ayydxn_luminescence_view_ULViewConfig_NativeMethods
 * Method:    nulViewConfigSetUserAgent
 * Signature: (JLjava/lang/String;)V
 */
void JNICALL ULViewConfigSetUserAgent_Native(JNIEnv* Environment, jclass, jlong ViewConfigHandle, jstring UserAgent)
{
    const auto ViewConfig = reinterpret_cast<ULViewConfig>(ViewConfigHandle);
    const auto UserAgentChars = Environment->GetStringUTFChars(UserAgent, nullptr);
    const auto UserAgentString = ulCreateString(UserAgentChars);
    
    ulViewConfigSetUserAgent(ViewConfig, UserAgentString);
    
    ulDestroyString(UserAgentString);
    Environment->ReleaseStringUTFChars(UserAgent, UserAgentChars);
}

static constexpr JNINativeMethod ViewConfigMethods[] = {
    JNI_METHOD("nulCreateViewConfig", "()J", ULCreateViewConfig_Native),
    JNI_METHOD("nulDestroyViewConfig", "(J)V", ULDestroyViewConfig_Native),
    JNI_METHOD("nulViewConfigSetDisplayID", "(JI)V", ULViewConfigSetDisplayID_Native),
    JNI_METHOD("nulViewConfigIsAccelerated", "(JZ)V", ULViewConfigIsAccelerated_Native),
    JNI_METHOD("nulViewConfigSetIsTransparent", "(JZ)V", ULViewConfigSetIsTransparent_Native),
    JNI_METHOD("nulViewConfigSetInitialDeviceScale", "(JD)V", ULViewConfigSetInitialDeviceScale_Native),
    JNI_METHOD("nulViewConfigSetInitialFocus", "(JZ)V", ULViewConfigSetInitialFocus_Native),
    JNI_METHOD("nulViewConfigSetEnableImages", "(JZ)V", ULViewConfigSetEnableImages_Native),
    JNI_METHOD("nulViewConfigSetEnableJavaScript", "(JZ)V", ULViewConfigSetEnableJavaScript_Native),
    JNI_METHOD("nulViewConfigSetFontFamilyStandard", "(JLjava/lang/String;)V", ULViewConfigSetFontFamilyStandard_Native),
    JNI_METHOD("nulViewConfigSetFontFamilyFixed", "(JLjava/lang/String;)V", ULViewConfigSetFontFamilyFixed_Native),
    JNI_METHOD("nulViewConfigSetFontFamilySansSerif", "(JLjava/lang/String;)V", ULViewConfigSetFontFamilySansSerif_Native),
    JNI_METHOD("nulViewConfigSetUserAgent", "(JLjava/lang/String;)V", ULViewConfigSetUserAgent_Native)
};

bool Luminescence::RegisterViewConfigMethods(JNIEnv* Environment)
{
    return RegisterNativeMethods(Environment, "me/ayydxn/luminescence/view/ULViewConfig$NativeMethods", JNI_METHODS_AND_COUNT(ViewConfigMethods));
}
