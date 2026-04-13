//
// Created by Ayydxn on 4/6/2026.
//

#include "Core/JNIUtilities.h"

#include <Ultralight/Defines.h>

/*
 * Class:     me_ayydxn_luminescence_Ultralight
 * Method:    nulVersionString
 * Signature: ()Ljava/lang/String;
 */
jstring JNICALL ULVersionString_Native(JNIEnv* Env, jclass)
{
	return Env->NewStringUTF(UltralightVersionString());
}

/*
 * Class:     me_ayydxn_luminescence_Ultralight
 * Method:    nulVersionMajor
 * Signature: ()I
 */
jint JNICALL ULVersionMajor_Native(JNIEnv*, jclass)
{
	return static_cast<jint>(UltralightVersionMajor());
}

/*
 * Class:     me_ayydxn_luminescence_Ultralight
 * Method:    nulVersionMinor
 * Signature: ()I
 */
jint JNICALL ULVersionMinor_Native(JNIEnv*, jclass)
{
	return static_cast<jint>(UltralightVersionMinor());
}

/*
 * Class:     me_ayydxn_luminescence_Ultralight
 * Method:    nulVersionPatch
 * Signature: ()I
 */
jint JNICALL ULVersionPatch_Native(JNIEnv*, jclass)
{
	return static_cast<jint>(UltralightVersionPatch());
}

/*
 * Class:     me_ayydxn_luminescence_Ultralight
 * Method:    nulWebKitVersionString
 * Signature: ()Ljava/lang/String;
 */
jstring JNICALL ULWebKitVersionString_Native(JNIEnv* Env, jclass)
{
	return Env->NewStringUTF(WebKitVersionString());
}

/*
 * Class:     me_ayydxn_luminescence_Ultralight
 * Method:    nulWebKitVersionMajor
 * Signature: ()I
 */
jint JNICALL ULWebKitVersionMajor_Native(JNIEnv*, jclass)
{
	return static_cast<jint>(WebKitVersionMajor());
}

/*
 * Class:     me_ayydxn_luminescence_Ultralight
 * Method:    nulWebKitVersionMinor
 * Signature: ()I
 */
jint JNICALL ULWebKitVersionMinor_Native(JNIEnv*, jclass)
{
	return static_cast<jint>(WebKitVersionMinor());
}

/*
 * Class:     me_ayydxn_luminescence_Ultralight
 * Method:    nulWebKitVersionTiny
 * Signature: ()I
 */
jint JNICALL ULWebKitVersionTiny_Native(JNIEnv*, jclass)
{
	return static_cast<jint>(WebKitVersionTiny());
}

/*
 * Class:     me_ayydxn_luminescence_Ultralight
 * Method:    nulWebKitVersionMicro
 * Signature: ()I
 */
jint JNICALL ULWebKitVersionMicro_Native(JNIEnv*, jclass)
{
	return static_cast<jint>(WebKitVersionMicro());
}

/*
 * Class:     me_ayydxn_luminescence_Ultralight
 * Method:    nulWebKitVersionNano
 * Signature: ()I
 */
jint JNICALL ULWebKitVersionNano_Native(JNIEnv*, jclass)
{
	return static_cast<jint>(WebKitVersionNano());
}

static constexpr JNINativeMethod UltralightMethods[] =
{
	JNI_METHOD("nulVersionString", "()Ljava/lang/String;", ULVersionString_Native),
	JNI_METHOD("nulVersionMajor", "()I", ULVersionMajor_Native),
	JNI_METHOD("nulVersionMinor", "()I", ULVersionMinor_Native),
	JNI_METHOD("nulVersionPatch", "()I", ULVersionPatch_Native),
	JNI_METHOD("nulWebKitVersionString", "()Ljava/lang/String;", ULWebKitVersionString_Native),
	JNI_METHOD("nulWebKitVersionMajor", "()I", ULWebKitVersionMajor_Native),
	JNI_METHOD("nulWebKitVersionMinor", "()I", ULWebKitVersionMinor_Native),
	JNI_METHOD("nulWebKitVersionTiny", "()I", ULWebKitVersionTiny_Native),
	JNI_METHOD("nulWebKitVersionMicro", "()I", ULWebKitVersionMicro_Native),
	JNI_METHOD("nulWebKitVersionNano", "()I", ULWebKitVersionNano_Native)
};

bool Luminescence::RegisterUltralightMethods(JNIEnv* Environment)
{
	return RegisterNativeMethods(Environment, "me/ayydxn/luminescence/Ultralight", JNI_METHODS_AND_COUNT(UltralightMethods));
}
