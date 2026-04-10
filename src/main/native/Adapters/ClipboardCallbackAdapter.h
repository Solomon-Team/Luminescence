//
// Created by Ayydxn on 4/9/2026.
//

#pragma once

#include "Core/CallbackAdapter.h"

#include <Ultralight/CAPI/CAPI_Clipboard.h>

namespace Luminescence
{
    class CClipboardCallbackAdapter : public ICallbackAdapter
    {
    public:
        CClipboardCallbackAdapter(JNIEnv* Environment, jobject JavaImplementationInstance);
        ~CClipboardCallbackAdapter() override;
        
        ULClipboard MakeStruct();
    private:
        static void Clear_Trampoline();
        static void ReadPlainText_Trampoline(ULString Result);
        static void WritePlainText_Trampoline(ULString Text);
    private:
        inline static thread_local CClipboardCallbackAdapter* m_ActiveCallbackAdapter = nullptr;
        
        jmethodID m_ClearMethodID = nullptr;
        jmethodID m_ReadPlainTextMethodID = nullptr;
        jmethodID m_WritePlainTextMethodID = nullptr;
    };
}
