//
// Created by Ayydxn on 4/6/2026.
//

#pragma once

#include <jni.h>

// Utility macro for cleaner method tables.
// 
// Example Usage: JNI_METHOD("javaName", "(Sig)V", native_func_ptr)
#define JNI_METHOD(Name, MethodSignature, ImplFunctionPointer) { (char*) Name, (char*) MethodSignature, (void*) ImplFunctionPointer }

namespace Luminescence
{
	bool RegisterUltralightMethods(JNIEnv* Environment);

	/**
	 * Internal helper to handle the actual registration boilerplate.
	 */
	inline bool RegisterNativeMethods(JNIEnv* Environment, const char* ClassName, const JNINativeMethod* Methods, int MethodsCount)
	{
		jclass Class = Environment->FindClass(ClassName);
		if (Class == nullptr)
			return false;

		return Environment->RegisterNatives(Class, Methods, MethodsCount) == 0;
	}
}
