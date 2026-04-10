//
// Created by Ayydxn on 4/8/2026.
//

#pragma once

#include "Core/CallbackAdapter.h"

#include <Ultralight/CAPI/CAPI_Logger.h>

namespace Luminescence
{
	class CLoggerCallbackAdapter : public ICallbackAdapter
	{
	public:
		CLoggerCallbackAdapter(JNIEnv* Env, jobject JavaImplementationInstance);
		~CLoggerCallbackAdapter() override;
		
		ULLogger MakeStruct();
	private:
		inline static thread_local CLoggerCallbackAdapter* m_ActiveCallbackAdapter;
		
		jmethodID m_LogMessageMethodID = nullptr;
	private:
		static void LogMessage_Trampoline(ULLogLevel LogLevel, ULString Message);
	};

}
