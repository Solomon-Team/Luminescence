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
        m_JVM->GetEnv(reinterpret_cast<void**>(&Environment), JNI_VERSION_10);
        
        return Environment;
    }
}
