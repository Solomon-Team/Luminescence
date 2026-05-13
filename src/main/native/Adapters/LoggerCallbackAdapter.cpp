//
// Created by Ayydxn on 4/8/2026.
//

#include "LoggerCallbackAdapter.h"
#include "Core/CallbackAdapterRegistry.h"
#include "Core/JNIUtilities.h"
#include "Core/ScopedLocalRef.h"
#include "Core/Profiling.h"

namespace Luminescence
{
    CLoggerCallbackAdapter::CLoggerCallbackAdapter(JNIEnv* Environment, jobject JavaImplementationInstance)
        : ICallbackAdapter(Environment, JavaImplementationInstance)
    {
        const CScopedLocalRef LoggerClassRef(Environment, Environment->FindClass("me/ayydxn/luminescence/platform/ULLogger"));
        m_LogMessageMethodID = Environment->GetMethodID(LoggerClassRef, "logMessage", "(Lme/ayydxn/luminescence/platform/ULLogger$Level;Ljava/lang/String;)V");

        CCallbackAdapterRegistry::GetInstance().Register(reinterpret_cast<uintptr_t>(this), this);
    }

    CLoggerCallbackAdapter::~CLoggerCallbackAdapter()
    {
        CCallbackAdapterRegistry::GetInstance().Unregister(reinterpret_cast<uintptr_t>(this));
    }

    ULLogger CLoggerCallbackAdapter::MakeStruct()
    {
        m_ActiveCallbackAdapter = this;

        return {
            .log_message = &CLoggerCallbackAdapter::LogMessage_Trampoline
        };
    }

    void CLoggerCallbackAdapter::LogMessage_Trampoline(ULLogLevel LogLevel, ULString Message)
    {
        ZoneScoped
        
        const CLoggerCallbackAdapter* Self = m_ActiveCallbackAdapter;
        if (!Self)
            return;

        auto [Environment, VirtualMachine, bWasAttached] = Self->AcquireJNIEnvironment();
        if (!Environment && !Self->m_JavaImplementation)
            return;
        
        const CScopedLocalRef LogLevelEnum(Environment, Environment->FindClass("me/ayydxn/luminescence/platform/ULLogger$Level"));
        const jmethodID FromNativeMethodID = Environment->GetStaticMethodID(LogLevelEnum, "fromNativeValue", "(I)Lme/ayydxn/luminescence/platform/ULLogger$Level;");
        const CScopedLocalRef LogLevelObject(Environment, Environment->CallStaticObjectMethod(LogLevelEnum, FromNativeMethodID, static_cast<jint>(LogLevel)));

        const CScopedLocalRef MessageString(Environment, ULStringToJavaString(Environment, Message));

        Environment->CallVoidMethod(Self->m_JavaImplementation, Self->m_LogMessageMethodID, LogLevelObject.Get(), MessageString.Get());
        Environment->ExceptionCheck();
    }
}
