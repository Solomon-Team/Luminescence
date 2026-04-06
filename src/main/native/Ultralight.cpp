//
// Created by Ayydxn on 4/6/2026.
//

#include "JNIUtilities.h"

#include <Ultralight/Defines.h>

jstring JNICALL ULVersionString_Native(JNIEnv* Env, jclass)
{
	return Env->NewStringUTF(UltralightVersionString());
}

jint JNICALL ULVersionMajor_Native(JNIEnv*, jclass)
{
	return static_cast<jint>(UltralightVersionMajor());
}

jint JNICALL ULVersionMinor_Native(JNIEnv*, jclass)
{
	return static_cast<jint>(UltralightVersionMinor());
}

jint JNICALL ULVersionPatch_Native(JNIEnv*, jclass)
{
	return static_cast<jint>(UltralightVersionPatch());
}

jstring JNICALL ULWebKitVersionString_Native(JNIEnv* Env, jclass)
{
	return Env->NewStringUTF(WebKitVersionString());
}

jint JNICALL ULWebKitVersionMajor_Native(JNIEnv*, jclass)
{
	return static_cast<jint>(WebKitVersionMajor());
}

jint JNICALL ULWebKitVersionMinor_Native(JNIEnv*, jclass)
{
	return static_cast<jint>(WebKitVersionMinor());
}

jint JNICALL ULWebKitVersionTiny_Native(JNIEnv*, jclass)
{
	return static_cast<jint>(WebKitVersionTiny());
}

jint JNICALL ULWebKitVersionMicro_Native(JNIEnv*, jclass)
{
	return static_cast<jint>(WebKitVersionMicro());
}

jint JNICALL ULWebKitVersionNano_Native(JNIEnv*, jclass)
{
	return static_cast<jint>(WebKitVersionNano());
}

static const JNINativeMethod Methods[] =
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
	return Luminescence::RegisterNativeMethods(Environment, "me/ayydxn/luminescence/Ultralight", Methods, sizeof(Methods) / sizeof(Methods[0]));
}
