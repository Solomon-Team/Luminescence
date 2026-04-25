//
// Created by Ayydxn on 4/8/2026.
//

#include "CallbackAdapter.h"

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
        
        JNIEnv* Environment = GetJNIEnvironment();
        
        if (Environment && m_JavaImplementation)
            Environment->DeleteGlobalRef(m_JavaImplementation);
    }

    JNIEnv* ICallbackAdapter::GetJNIEnvironment() const
    {
        JNIEnv* Environment = nullptr;
        const jint GetEnvResult = m_JVM->GetEnv(reinterpret_cast<void**>(&Environment), JNI_VERSION_10);

        if (GetEnvResult == JNI_EDETACHED)
        {
            if (m_JVM->AttachCurrentThread(reinterpret_cast<void**>(&Environment), nullptr) != JNI_OK)
                return nullptr;
        }

        return Environment;
    }
}
