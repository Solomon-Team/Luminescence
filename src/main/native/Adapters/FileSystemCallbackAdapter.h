//
// Created by Ayydxn on 4/9/2026.
//

#pragma once

#include "Core/CallbackAdapter.h"

#include <Ultralight/CAPI/CAPI_FileSystem.h>

namespace Luminescence
{
    class CFileSystemCallbackAdapter : public ICallbackAdapter
    {
    public:
        CFileSystemCallbackAdapter(JNIEnv* Environment, jobject JavaImplementationInstance);
        ~CFileSystemCallbackAdapter() override;

        ULFileSystem MakeStruct();
    private:
        static bool FileExists_Trampoline(ULString Path);
        static ULString GetFileMimeType_Trampoline(ULString Path);
        static ULString GetFileCharset_Trampoline(ULString Path);
        static ULBuffer OpenFile_Trampoline(ULString Path);
    private:
        inline static thread_local CFileSystemCallbackAdapter* m_ActiveCallbackAdapter = nullptr;

        jmethodID m_FileExistsMethodID = nullptr;
        jmethodID m_GetFileMimeTypeMethodID = nullptr;
        jmethodID m_GetFileCharsetMethodID = nullptr;
        jmethodID m_OpenFileMethodID = nullptr;
    };
}
