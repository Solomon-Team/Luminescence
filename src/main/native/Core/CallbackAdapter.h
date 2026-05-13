//
// Created by Ayydxn on 4/8/2026.
//

#pragma once

#include <jni.h>

namespace Luminescence
{
	// An RAII helper over JNI environment thread attachment and detachment to prevent the accumulation of leaked thread attachments.
	// This helps to prevent things like the JVM running of thread slots or wasting memory on hanging thread attachments.
	// This is useful and important in thread-pooled environments, especially if those threads live for the lifetime of the application.
	struct FJNIEnvironmentGuard
	{
		JNIEnv* Environment;
		JavaVM* VirtualMachine;
		bool bWasAttached;
		
		FJNIEnvironmentGuard(JNIEnv* Environment, JavaVM* VirtualMachine, bool bWasAttached)
			: Environment(Environment), VirtualMachine(VirtualMachine), bWasAttached(bWasAttached) {}
		
		~FJNIEnvironmentGuard()
		{
			if (bWasAttached && VirtualMachine)
				VirtualMachine->DetachCurrentThread();
		}
		
		FJNIEnvironmentGuard(const FJNIEnvironmentGuard&) = delete;
		FJNIEnvironmentGuard& operator=(const FJNIEnvironmentGuard&) = delete;
	};
	
	class ICallbackAdapter
	{
	public:
		explicit ICallbackAdapter(JNIEnv* Env, jobject JavaImplementationInstance);
		ICallbackAdapter(const ICallbackAdapter&) = delete;
		virtual ~ICallbackAdapter();

		ICallbackAdapter& operator=(const ICallbackAdapter&) = delete;
	protected:
		JavaVM* m_JVM = nullptr;
		jobject m_JavaImplementation = nullptr;

		FJNIEnvironmentGuard AcquireJNIEnvironment() const;
	};
}
