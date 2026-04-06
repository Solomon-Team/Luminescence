//
// Created by Ayydxn on 4/6/2026.
//

#include "me_ayydxn_luminescence_Ultralight.h"

#include <Ultralight/Defines.h>

JNIEXPORT jstring JNICALL Java_me_ayydxn_luminescence_Ultralight_nulVersionString(JNIEnv* Env, jclass)
{
	return Env->NewStringUTF(UltralightVersionString());
}

JNIEXPORT jint JNICALL Java_me_ayydxn_luminescence_Ultralight_nulVersionMajor(JNIEnv*, jclass)
{
	return UltralightVersionMajor();
}

JNIEXPORT jint JNICALL Java_me_ayydxn_luminescence_Ultralight_nulVersionMinor(JNIEnv*, jclass)
{
	return UltralightVersionMinor();
}

JNIEXPORT jint JNICALL Java_me_ayydxn_luminescence_Ultralight_nulVersionPatch(JNIEnv*, jclass)
{
	return UltralightVersionPatch();
}

JNIEXPORT jstring JNICALL Java_me_ayydxn_luminescence_Ultralight_nulWebKitVersionString(JNIEnv* env, jclass)
{
	return env->NewStringUTF(WebKitVersionString());
}

JNIEXPORT jint JNICALL Java_me_ayydxn_luminescence_Ultralight_nulWebKitVersionMajor(JNIEnv*, jclass)
{
	return WebKitVersionMajor();
}

JNIEXPORT jint JNICALL Java_me_ayydxn_luminescence_Ultralight_nulWebKitVersionMinor(JNIEnv*, jclass)
{
	return WebKitVersionMinor();
}

JNIEXPORT jint JNICALL Java_me_ayydxn_luminescence_Ultralight_nulWebKitVersionTiny(JNIEnv*, jclass)
{
	return WebKitVersionTiny();
}

JNIEXPORT jint JNICALL Java_me_ayydxn_luminescence_Ultralight_nulWebKitVersionMicro(JNIEnv*, jclass)
{
	return WebKitVersionMicro();
}

JNIEXPORT jint JNICALL Java_me_ayydxn_luminescence_Ultralight_nulWebKitVersionNano(JNIEnv*, jclass)
{
	return WebKitVersionNano();
}



