//
// Created by Ayydxn on 4/9/2026.
//

#include "ClipboardCallbackAdapter.h"
#include "Core/CallbackAdapterRegistry.h"
#include "Core/JNIUtilities.h"
#include "Core/ScopedLocalRef.h"

namespace Luminescence
{
    CClipboardCallbackAdapter::CClipboardCallbackAdapter(JNIEnv* Environment, jobject JavaImplementationInstance)
        : ICallbackAdapter(Environment, JavaImplementationInstance)
    {
        const CScopedLocalRef ClipboardClassRef(Environment, Environment->FindClass("me/ayydxn/luminescence/platform/ULClipboard"));
        
        m_ClearMethodID = Environment->GetMethodID(ClipboardClassRef, "clear", "()V");
        m_ReadPlainTextMethodID = Environment->GetMethodID(ClipboardClassRef, "readPlainText", "()Ljava/lang/String;");
        m_WritePlainTextMethodID = Environment->GetMethodID(ClipboardClassRef, "writePlainText", "(Ljava/lang/String;)V");
        
        CCallbackAdapterRegistry::GetInstance().Register(reinterpret_cast<uintptr_t>(this), this);
    }

    CClipboardCallbackAdapter::~CClipboardCallbackAdapter()
    {
        CCallbackAdapterRegistry::GetInstance().Unregister(reinterpret_cast<uintptr_t>(this));
    }

    ULClipboard CClipboardCallbackAdapter::MakeStruct()
    {
        m_ActiveCallbackAdapter = this;
        
        return {
            .clear = &CClipboardCallbackAdapter::Clear_Trampoline,
            .read_plain_text = &CClipboardCallbackAdapter::ReadPlainText_Trampoline,
            .write_plain_text = &CClipboardCallbackAdapter::WritePlainText_Trampoline
        };
    }

    void CClipboardCallbackAdapter::Clear_Trampoline()
    {
        const CClipboardCallbackAdapter* Self = m_ActiveCallbackAdapter;
        JNIEnv* Environment = Self->GetJNIEnvironment();
        
        Environment->CallVoidMethod(Self->m_JavaImplementation, Self->m_ClearMethodID);
        
        CheckException(Environment);
    }

    void CClipboardCallbackAdapter::ReadPlainText_Trampoline(ULString Result)
    {
        const CClipboardCallbackAdapter* Self = m_ActiveCallbackAdapter;
        JNIEnv* Environment = Self->GetJNIEnvironment();

        // Call Java to get the current clipboard contents as a String
        const CScopedLocalRef ClipboardText(Environment, (jstring) Environment->CallObjectMethod(Self->m_JavaImplementation, Self->m_ReadPlainTextMethodID));
        CheckException(Environment);

        if (!ClipboardText.Get())
            return;

        // Write the Java string into the ULString Ultralight provided
        const jchar* Chars = Environment->GetStringChars(ClipboardText, nullptr);
        const jsize Length = Environment->GetStringLength(ClipboardText);
        const auto ClipboardString = ulCreateStringUTF16((ULChar16*) Chars, static_cast<size_t>(Length));
        
        ulStringAssignString(Result, ClipboardString);
        
        ulDestroyString(ClipboardString);
        
        Environment->ReleaseStringChars(ClipboardText, Chars);
    }

    void CClipboardCallbackAdapter::WritePlainText_Trampoline(ULString Text)
    {
        const CClipboardCallbackAdapter* Self = m_ActiveCallbackAdapter;
        JNIEnv* Environment = Self->GetJNIEnvironment();
        
        const CScopedLocalRef TextString(Environment, ULStringToJavaString(Environment, Text));
        
        Environment->CallVoidMethod(Self->m_JavaImplementation, Self->m_WritePlainTextMethodID, TextString.Get());
        
        CheckException(Environment);
    }
}
