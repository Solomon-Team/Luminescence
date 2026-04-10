//
// Created by Ayydxn on 4/8/2026.
//

#pragma once

#include "CallbackAdapter.h"

#include <mutex>
#include <unordered_map>

namespace Luminescence
{
    class CCallbackAdapterRegistry
    {
    public:
        static CCallbackAdapterRegistry& GetInstance()
        {
            static CCallbackAdapterRegistry Instance;
            return Instance;
        }

        void Register(const uintptr_t Key, ICallbackAdapter* Adapter)
        {
            std::scoped_lock Lock(m_Mutex);
            
            m_CallbackAdapters[Key] = Adapter;
        }

        void Unregister(const uintptr_t Key)
        {
            std::scoped_lock Lock(m_Mutex);
            
            m_CallbackAdapters.erase(Key);
        }

        ICallbackAdapter* Find(const uintptr_t Key)
        {
            std::scoped_lock Lock(m_Mutex);
            const auto It = m_CallbackAdapters.find(Key);
            
            return It != m_CallbackAdapters.end() ? It->second : nullptr;
        }
    private:
        std::unordered_map<uintptr_t, ICallbackAdapter*> m_CallbackAdapters;
        std::mutex m_Mutex;
    };
}
