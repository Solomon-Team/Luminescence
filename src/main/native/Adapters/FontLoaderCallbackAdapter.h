//
// Created by Ayydxn on 4/9/2026.
//

#pragma once

#include "Core/CallbackAdapter.h"

#include <Ultralight/CAPI/CAPI_FontLoader.h>

namespace Luminescence
{
    class CFontLoaderCallbackAdapter : public ICallbackAdapter
    {
    public:
        CFontLoaderCallbackAdapter(JNIEnv* Environment, jobject JavaImplementation);
        ~CFontLoaderCallbackAdapter() override;
        
        ULFontLoader MakeStruct();
    private:
        static ULString GetFallbackFont_Trampoline();
        static ULString GetFallbackFontForCharacters_Trampoline(ULString Characters, int Weight, bool bIsItalic);
        static ULFontFile Load_Trampoline(ULString Family, int Weight, bool bIsItalic);
    private:
        inline static thread_local CFontLoaderCallbackAdapter* m_ActiveCallbackAdapter;
        
        jclass m_FontFileClass;
        
        jmethodID m_GetFallbackFontMethodID;
        jmethodID m_GetFallbackFontForCharactersMethodID;
        jmethodID m_LoadMethodID;
        jmethodID m_FontFileGetSourceMethodID;
        jmethodID m_FontFileGetFilePathMethodID;
        jmethodID m_FontFileGetBufferMethodID;
        jobject m_SourceFilePath;
    };
}
