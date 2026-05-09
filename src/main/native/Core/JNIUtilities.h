//
// Created by Ayydxn on 4/6/2026.
//

#pragma once

#include <jni.h>
#include <Ultralight/CAPI/CAPI_String.h>

#include <stdexcept>

// Utility macro for cleaner method tables.
// 
// Example Usage: JNI_METHOD("javaName", "(Sig)V", native_func_ptr)
#define JNI_METHOD(Name, MethodSignature, ImplFunctionPointer) { (char*) (Name), (char*) (MethodSignature), (void*) (ImplFunctionPointer) }

// Utility macro for quickly providing your methods array and its size to RegisterNativeMethods
// 
// Example Usage: return RegisterNativeMethods(Environment, "me/ayydxn/luminescence/util/ULBuffer$NativeMethods", JNI_METHODS_AND_COUNT(BufferMethods));
#define JNI_METHODS_AND_COUNT(Methods) Methods, std::size(Methods)

namespace Luminescence
{
    /* -- Ultralight -- */
    bool RegisterUltralightMethods(JNIEnv* Environment);
    bool RegisterConfigMethods(JNIEnv* Environment);
    bool RegisterStringMethods(JNIEnv* Environment);
    bool RegisterPlatformMethods(JNIEnv* Environment);
    bool RegisterBufferMethods(JNIEnv* Environment);
    bool RegisterRendererMethods(JNIEnv* Environment);
    bool RegisterGamepadEventMethods(JNIEnv* Environment);
    bool RegisterBitmapMethods(JNIEnv* Environment);
    bool RegisterSessionMethods(JNIEnv* Environment);
    bool RegisterViewConfigMethods(JNIEnv* Environment);
    bool RegisterViewMethods(JNIEnv* Environment);
    bool RegisterInputEventMethods(JNIEnv* Environment);
    bool RegisterSurfaceMethods(JNIEnv* Environment);
    
    /* -- JavaScriptCore -- */
    bool RegisterJSValueMethods(JNIEnv* Environment);
    bool RegisterJSContextMethods(JNIEnv* Environment);
    bool RegisterJSObjectMethods(JNIEnv* Environment);
    bool RegisterJSArrayMethods(JNIEnv* Environment);
    bool RegisterJSFunctionMethods(JNIEnv* Environment);

    inline bool RegisterNativeMethods(JNIEnv* Environment, const char* ClassName, const JNINativeMethod* Methods, int MethodsCount)
    {
        jclass Class = Environment->FindClass(ClassName);
        if (Class == nullptr)
            return false;

        return Environment->RegisterNatives(Class, Methods, MethodsCount) == 0;
    }

    inline jstring ULStringToJavaString(JNIEnv* Environment, ULString ULString)
    {
        if (!ULString)
            return Environment->NewStringUTF("");

        const char* Data = ulStringGetData(ULString);
        return Environment->NewStringUTF(Data);
    }
    
    inline ULString JavaStringToULString(JNIEnv* Environment, jstring JavaString)
    {
        if (!JavaString)
            return ulCreateString("");

        const jchar* Chars = Environment->GetStringChars(JavaString, nullptr);
        const jsize Length = Environment->GetStringLength(JavaString);
        const ULString Result = ulCreateStringUTF16((ULChar16*) Chars, static_cast<size_t>(Length));

        Environment->ReleaseStringChars(JavaString, Chars);

        return Result;
    }

    // Describe and clear any pending Java exceptions to prevent them from crashing the JVM during native-to-Java callbacks.
    inline void CheckException(JNIEnv* Env)
    {
        if (Env && Env->ExceptionCheck())
        {
            Env->ExceptionDescribe();
            Env->ExceptionClear();

            throw std::runtime_error("Java exception during JNI callback");
        }
    }
}
