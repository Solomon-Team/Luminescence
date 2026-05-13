//
// Created by Ayydxn on 4/9/2026.
//

#include "FileSystemCallbackAdapter.h"
#include "Core/CallbackAdapterRegistry.h"
#include "Core/JNIUtilities.h"
#include "Core/Profiling.h"
#include "Core/ScopedLocalRef.h"

#include <Ultralight/CAPI/CAPI_Buffer.h>

namespace Luminescence
{
    CFileSystemCallbackAdapter::CFileSystemCallbackAdapter(JNIEnv* Environment, jobject JavaImplementationInstance)
        : ICallbackAdapter(Environment, JavaImplementationInstance)
    {
        const CScopedLocalRef FileSystemClassRef(Environment, Environment->FindClass("me/ayydxn/luminescence/platform/ULFileSystem"));

        m_FileExistsMethodID = Environment->GetMethodID(FileSystemClassRef, "fileExists", "(Ljava/lang/String;)Z");
        m_GetFileMimeTypeMethodID = Environment->GetMethodID(FileSystemClassRef, "getFileMimeType", "(Ljava/lang/String;)Ljava/lang/String;");
        m_GetFileCharsetMethodID = Environment->GetMethodID(FileSystemClassRef, "getFileCharset", "(Ljava/lang/String;)Ljava/lang/String;");
        m_OpenFileMethodID = Environment->GetMethodID(FileSystemClassRef, "openFile", "(Ljava/lang/String;)[B");

        CCallbackAdapterRegistry::GetInstance().Register(reinterpret_cast<uintptr_t>(this), this);
    }

    CFileSystemCallbackAdapter::~CFileSystemCallbackAdapter()
    {
        CCallbackAdapterRegistry::GetInstance().Unregister(reinterpret_cast<uintptr_t>(this));
    }

    ULFileSystem CFileSystemCallbackAdapter::MakeStruct()
    {
        m_ActiveCallbackAdapter = this;

        return {
            .file_exists = &CFileSystemCallbackAdapter::FileExists_Trampoline,
            .get_file_mime_type = &CFileSystemCallbackAdapter::GetFileMimeType_Trampoline,
            .get_file_charset = &CFileSystemCallbackAdapter::GetFileCharset_Trampoline,
            .open_file = &CFileSystemCallbackAdapter::OpenFile_Trampoline
        };
    }

    bool CFileSystemCallbackAdapter::FileExists_Trampoline(ULString Path)
    {
        ZoneScoped
        
        const CFileSystemCallbackAdapter* Self = m_ActiveCallbackAdapter;

        auto [Environment, VirtualMachine, bWasAttached] = Self->AcquireJNIEnvironment();
        if (!Environment && !Self->m_JavaImplementation)
            return false;
        
        const CScopedLocalRef PathStringRef(Environment, ULStringToJavaString(Environment, Path));
        const jboolean Result = Environment->CallBooleanMethod(Self->m_JavaImplementation, Self->m_FileExistsMethodID, PathStringRef.Get());
        if (Environment->ExceptionCheck())
            return false;

        return Result == JNI_TRUE;
    }

    ULString CFileSystemCallbackAdapter::GetFileMimeType_Trampoline(ULString Path)
    {
        ZoneScoped
        
        const CFileSystemCallbackAdapter* Self = m_ActiveCallbackAdapter;

        auto [Environment, VirtualMachine, bWasAttached] = Self->AcquireJNIEnvironment();
        if (!Environment && !Self->m_JavaImplementation)
            return ulCreateString("");
        
        const CScopedLocalRef PathString(Environment, ULStringToJavaString(Environment, Path));
        const CScopedLocalRef Result(Environment, (jstring) Environment->CallObjectMethod(Self->m_JavaImplementation, Self->m_GetFileMimeTypeMethodID, PathString.Get()));
        if (Environment->ExceptionCheck())
            return ulCreateString("");

        // Ultralight will call ulDestroyString() on this — return a fresh copy.
        return Result.Get() ? JavaStringToULString(Environment, Result) : ulCreateString("application/unknown");
    }

    ULString CFileSystemCallbackAdapter::GetFileCharset_Trampoline(ULString Path)
    {
        ZoneScoped
        
        const CFileSystemCallbackAdapter* Self = m_ActiveCallbackAdapter;

        auto [Environment, VirtualMachine, bWasAttached] = Self->AcquireJNIEnvironment();
        if (!Environment && !Self->m_JavaImplementation)
            return ulCreateString("");
        
        const CScopedLocalRef PathStr(Environment, ULStringToJavaString(Environment, Path));
        const CScopedLocalRef Result(Environment, (jstring) Environment->CallObjectMethod(Self->m_JavaImplementation, Self->m_GetFileCharsetMethodID, PathStr.Get()));
        if (Environment->ExceptionCheck())
            return ulCreateString("");

        return Result.Get() ? JavaStringToULString(Environment, Result) : ulCreateString("utf-8");
    }

    ULBuffer CFileSystemCallbackAdapter::OpenFile_Trampoline(ULString Path)
    {
        ZoneScoped
        
        const CFileSystemCallbackAdapter* Self = m_ActiveCallbackAdapter;
        
        auto [Environment, VirtualMachine, bWasAttached] = Self->AcquireJNIEnvironment();
        if (!Environment && !Self->m_JavaImplementation)
            return ulCreateBuffer(nullptr, 1, nullptr, nullptr);

        const CScopedLocalRef PathString(Environment, ULStringToJavaString(Environment, Path));
        const CScopedLocalRef ByteArray(Environment, (jbyteArray) Environment->CallObjectMethod(Self->m_JavaImplementation, Self->m_OpenFileMethodID, PathString.Get()));
        if (Environment->ExceptionCheck())
            return nullptr;

        if (!ByteArray.Get())
            return nullptr;

        const jsize Length = Environment->GetArrayLength(ByteArray);
        jbyte* Bytes = Environment->GetByteArrayElements(ByteArray, nullptr);
        
        const ULBuffer Buffer = ulCreateBufferFromCopy(Bytes, static_cast<size_t>(Length));

        Environment->ReleaseByteArrayElements(ByteArray, Bytes, JNI_ABORT);

        return Buffer;
    }
}
