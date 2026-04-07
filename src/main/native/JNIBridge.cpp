//
// Created by Ayydxn on 4/6/2026.
//

#include "JNIUtilities.h"

#include <jni.h>

extern "C" JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* VirtualMachine, void*)
{
	JNIEnv* Environment;

	// Check if the JVM supports JNI 1.10 and fail the load if it doesn't
	if (VirtualMachine->GetEnv(reinterpret_cast<void**>(&Environment), JNI_VERSION_10) != JNI_OK) {
		return JNI_ERR;
	}

	// Register all our methods
	if (!Luminescence::RegisterUltralightMethods(Environment))
		return JNI_ERR;
	
	if (!Luminescence::RegisterConfigMethods(Environment))
		return JNI_ERR;
	
	if (!Luminescence::RegisterULStringMethods(Environment))
		return JNI_ERR;

	return JNI_VERSION_10;
}
