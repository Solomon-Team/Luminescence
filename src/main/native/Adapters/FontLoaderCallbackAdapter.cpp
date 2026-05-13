//
// Created by Ayydxn on 4/9/2026.
//

#include "FontLoaderCallbackAdapter.h"
#include "Core/CallbackAdapterRegistry.h"
#include "Core/JNIUtilities.h"
#include "Core/Profiling.h"
#include "Core/ScopedLocalRef.h"

namespace Luminescence
{
    CFontLoaderCallbackAdapter::CFontLoaderCallbackAdapter(JNIEnv* Environment, jobject JavaImplementation)
        : ICallbackAdapter(Environment, JavaImplementation)
    {
        const CScopedLocalRef FontLoaderClass(Environment, Environment->FindClass("me/ayydxn/luminescence/platform/ULFontLoader"));

        m_GetFallbackFontMethodID = Environment->GetMethodID(FontLoaderClass, "getFallbackFont", "()Ljava/lang/String;");
        m_GetFallbackFontForCharactersMethodID = Environment->GetMethodID(FontLoaderClass, "getFallbackFontForCharacters", "(Ljava/lang/String;IZ)Ljava/lang/String;");
        m_LoadMethodID = Environment->GetMethodID(FontLoaderClass, "load", "(Ljava/lang/String;IZ)Lme/ayydxn/luminescence/font/ULFontFile;");
        
        const CScopedLocalRef FontFileClass(Environment, Environment->FindClass("me/ayydxn/luminescence/font/ULFontFile"));
        
        m_FontFileClass = (jclass) Environment->NewGlobalRef(FontFileClass);
        m_FontFileGetSourceMethodID = Environment->GetMethodID(FontFileClass, "getSource", "()Lme/ayydxn/luminescence/font/ULFontFile$Source;");
        m_FontFileGetFilePathMethodID = Environment->GetMethodID(FontFileClass, "getFilePath", "()Ljava/lang/String;");
        m_FontFileGetBufferMethodID = Environment->GetMethodID(FontFileClass, "getBuffer", "()[B");
        
        const CScopedLocalRef SourceClass(Environment, Environment->FindClass("me/ayydxn/luminescence/font/ULFontFile$Source"));
        const jfieldID FilePathField = Environment->GetStaticFieldID(SourceClass, "FILE_PATH", "Lme/ayydxn/luminescence/font/ULFontFile$Source;");
        
        m_SourceFilePath = Environment->NewGlobalRef(Environment->GetStaticObjectField(SourceClass, FilePathField));

        CCallbackAdapterRegistry::GetInstance().Register(reinterpret_cast<uintptr_t>(this), this);
    }

    CFontLoaderCallbackAdapter::~CFontLoaderCallbackAdapter()
    {
        if (auto [Environment, VirtualMachine, bWasAttached] = AcquireJNIEnvironment(); Environment)
        {
            Environment->DeleteGlobalRef(m_FontFileClass);
            Environment->DeleteGlobalRef(m_SourceFilePath);
        }

        
        CCallbackAdapterRegistry::GetInstance().Unregister(reinterpret_cast<uintptr_t>(this));
    }

    ULFontLoader CFontLoaderCallbackAdapter::MakeStruct()
    {
        m_ActiveCallbackAdapter = this;

        return {
            .get_fallback_font = &CFontLoaderCallbackAdapter::GetFallbackFont_Trampoline,
            .get_fallback_font_for_characters = &CFontLoaderCallbackAdapter::GetFallbackFontForCharacters_Trampoline,
            .load = &CFontLoaderCallbackAdapter::Load_Trampoline
        };
    }

    ULString CFontLoaderCallbackAdapter::GetFallbackFont_Trampoline()
    {
        ZoneScoped
        
        const CFontLoaderCallbackAdapter* Self = m_ActiveCallbackAdapter;
        
        auto [Environment, VirtualMachine, bWasAttached] = Self->AcquireJNIEnvironment();
        if (!Environment && !Self->m_JavaImplementation)
            return ulCreateString("");
        
        const CScopedLocalRef Result(Environment, (jstring) Environment->CallObjectMethod(Self->m_JavaImplementation, Self->m_GetFallbackFontMethodID));
        if (Environment->ExceptionCheck())
            return ulCreateString("");
        
        // Ultralight calls ulDestroyString() on this — return a fresh allocation.
        return Result.Get() ? JavaStringToULString(Environment, Result) : ulCreateString("Arial");
    }

    ULString CFontLoaderCallbackAdapter::GetFallbackFontForCharacters_Trampoline(ULString Characters, int Weight, bool bIsItalic)
    {
        ZoneScoped
        
        const CFontLoaderCallbackAdapter* Self = m_ActiveCallbackAdapter;
        
        auto [Environment, VirtualMachine, bWasAttached] = Self->AcquireJNIEnvironment();
        if (!Environment && !Self->m_JavaImplementation)
            return ulCreateString("");
        
        const CScopedLocalRef CharsStr(Environment, ULStringToJavaString(Environment, Characters));
        const CScopedLocalRef Result(Environment, (jstring) Environment->CallObjectMethod(Self->m_JavaImplementation, Self->m_GetFallbackFontForCharactersMethodID,
                                            CharsStr.Get(), static_cast<jint>(Weight), static_cast<jboolean>(bIsItalic)));
        
        if (Environment->ExceptionCheck())
            return ulCreateString("");

        return Result.Get() ? JavaStringToULString(Environment, Result) : ulCreateString("Arial");
    }

    ULFontFile CFontLoaderCallbackAdapter::Load_Trampoline(ULString Family, int Weight, bool bIsItalic)
    {
        ZoneScoped
        
        const CFontLoaderCallbackAdapter* Self = m_ActiveCallbackAdapter;
        
        auto [Environment, VirtualMachine, bWasAttached] = Self->AcquireJNIEnvironment();
        if (!Environment && !Self->m_JavaImplementation)
            return nullptr;
        
        const CScopedLocalRef FamilyStr(Environment, ULStringToJavaString(Environment, Family));
        const CScopedLocalRef FontFileObject(Environment, Environment->CallObjectMethod(Self->m_JavaImplementation, Self->m_LoadMethodID, FamilyStr.Get(),
                                  static_cast<jint>(Weight), static_cast<jboolean>(bIsItalic)));
        
        if (Environment->ExceptionCheck())
            return nullptr;

        // Null return means "fall back to another font" — valid per the API contract.
        if (!FontFileObject.Get())
            return nullptr;

        // Determine which creation mode to use by inspecting ULFontFile.source
        const CScopedLocalRef Source(Environment, Environment->CallObjectMethod(FontFileObject, Self->m_FontFileGetSourceMethodID));
        
        if (Environment->IsSameObject(Source, Self->m_SourceFilePath))
        {
            const CScopedLocalRef PathStr(Environment, (jstring) Environment->CallObjectMethod(FontFileObject, Self->m_FontFileGetFilePathMethodID));
            
            const ULString NativePath = JavaStringToULString(Environment, PathStr);
            const ULFontFile Result = ulFontFileCreateFromFilePath(NativePath); // Ultralight consumes and owns the lifecycle of the ULFontFile.
            
            ulDestroyString(NativePath);
            
            return Result;
        }
        
        const CScopedLocalRef ByteArray(Environment, (jbyteArray) Environment->CallObjectMethod(FontFileObject, Self->m_FontFileGetBufferMethodID));
        const jsize Length = Environment->GetArrayLength(ByteArray);
        jbyte* Bytes = Environment->GetByteArrayElements(ByteArray, nullptr);
        
        const ULBuffer Buffer = ulCreateBufferFromCopy(Bytes, static_cast<size_t>(Length));
        Environment->ReleaseByteArrayElements(ByteArray, Bytes, JNI_ABORT);
        
        return ulFontFileCreateFromBuffer(Buffer); // Ultralight consumes and owns the lifecycle of the ULBuffer used to create the font.
    }
}
