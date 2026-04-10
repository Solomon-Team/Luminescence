//
// Created by Ayydxn on 4/8/2026.
//

#pragma once

#include <jni.h>

namespace Luminescence
{
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

		JNIEnv* GetJNIEnvironment() const;
	};
}
