//
// Created by Ayydxn on 4/8/2026.
//

#include "CallbackAdapter.h"

#include "Core/Profiling.h"

namespace Luminescence
{
    ICallbackAdapter::ICallbackAdapter(JNIEnv* Env, jobject JavaImplementationInstance)
        : m_JavaImplementation(Env->NewGlobalRef(JavaImplementationInstance))
    {
        Env->GetJavaVM(&m_JVM);
    }

    ICallbackAdapter::~ICallbackAdapter()
    {
        if (!m_JVM)
            return;
        
        auto [Environment, VirtualMachine, bWasAttached] = AcquireJNIEnvironment();
        
        if (Environment && m_JavaImplementation)
            Environment->DeleteGlobalRef(m_JavaImplementation);
    }

    FJNIEnvironmentGuard ICallbackAdapter::AcquireJNIEnvironment() const
    {
        JNIEnv* Environment = nullptr;
        const jint Result = m_JVM->GetEnv(reinterpret_cast<void**>(&Environment), JNI_VERSION_10);

        if (Result == JNI_EDETACHED)
        {
            if (m_JVM->AttachCurrentThreadAsDaemon(reinterpret_cast<void**>(&Environment), nullptr) != JNI_OK)
                return { nullptr, nullptr, false };

            // The environment was attached to the current thread.
            // So, it is marked as such so it will be detached when the guard is destroyed when it goes out of scope.
            return { Environment, m_JVM, true };
        }

        // The environment was already attached to the current thread, so no detach is needed.
        return { Environment, nullptr, false };
    }
}
