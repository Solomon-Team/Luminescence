//
// Created by Ayydxn on 4/6/2026.
//

#include "Core/JNIUtilities.h"

#include <jni.h>

#define CALL_REGISTER_METHOD(RegistrationMethod) if (!Luminescence::RegistrationMethod(Environment)) return JNI_ERR; 

extern "C" JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* VirtualMachine, void*)
{
	JNIEnv* Environment;

	// Check if the JVM supports JNI 1.10 and fail the load if it doesn't
	if (VirtualMachine->GetEnv(reinterpret_cast<void**>(&Environment), JNI_VERSION_10) != JNI_OK) {
		return JNI_ERR;
	}
	
	/* -- Ultralight -- */
	CALL_REGISTER_METHOD(RegisterUltralightMethods)
	CALL_REGISTER_METHOD(RegisterConfigMethods)
	CALL_REGISTER_METHOD(RegisterStringMethods)
	CALL_REGISTER_METHOD(RegisterPlatformMethods)
	CALL_REGISTER_METHOD(RegisterBufferMethods)
	CALL_REGISTER_METHOD(RegisterRendererMethods)
	CALL_REGISTER_METHOD(RegisterGamepadEventMethods)
	CALL_REGISTER_METHOD(RegisterBitmapMethods)
	CALL_REGISTER_METHOD(RegisterSessionMethods)
	CALL_REGISTER_METHOD(RegisterViewConfigMethods)
	CALL_REGISTER_METHOD(RegisterInputEventMethods)

	return JNI_VERSION_10;
}
