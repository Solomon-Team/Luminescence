//
// Created by Ayydxn on 4/6/2026.
//

#include "Core/JNIUtilities.h"

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
	
	if (!Luminescence::RegisterStringMethods(Environment))
		return JNI_ERR;
	
	if (!Luminescence::RegisterPlatformMethods(Environment))
		return JNI_ERR;
	
	if (!Luminescence::RegisterBufferMethods(Environment))
		return JNI_ERR;
	
	if (!Luminescence::RegisterRendererMethods(Environment))
		return JNI_ERR;
	
	if (!Luminescence::RegisterGamepadEventMethods(Environment))
		return JNI_ERR;
	
	return JNI_VERSION_10;
}
