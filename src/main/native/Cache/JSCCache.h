#pragma once

#include "Core/ScopedLocalRef.h"

#include <jni.h>

namespace Luminescence::JavaScript
{
    struct CJSExceptionCache
    {
        inline static jclass ClassRef = nullptr;
        inline static jmethodID ConstructorWithJSValue = nullptr;
        inline static jmethodID ConstructorWithString = nullptr;
        inline static jmethodID GetJSValueMethodID = nullptr;

        static bool InitializeCache(JNIEnv* Environment)
        {
            if (bIsInitialized)
                return true;
            
            const CScopedLocalRef Class(Environment, Environment->FindClass("me/ayydxn/luminescence/javascript/JSException"));
            if (!Class)
                return false;
            
            ClassRef = (jclass) Environment->NewGlobalRef(Class);
            ConstructorWithJSValue = Environment->GetMethodID(ClassRef, "<init>", "(Lme/ayydxn/luminescence/javascript/JSValue;)V");
            ConstructorWithString = Environment->GetMethodID(ClassRef, "<init>", "(Ljava/lang/String;)V");
            GetJSValueMethodID = Environment->GetMethodID(ClassRef, "getJSValue", "()Lme/ayydxn/luminescence/javascript/JSValue;");
            
            bIsInitialized = (ClassRef && ConstructorWithJSValue && ConstructorWithString && GetJSValueMethodID);
            
            return bIsInitialized;
        }

        static void ClearCache(JNIEnv* Environment)
        {
            if (ClassRef)
                Environment->DeleteGlobalRef(ClassRef);
            
            ClassRef = nullptr;
            bIsInitialized = false;
        }
    private:
        inline static bool bIsInitialized = false;
    };

    struct CJSValueCache
    {
        inline static jclass ClassRef = nullptr;
        inline static jmethodID Constructor = nullptr;
        inline static jfieldID ValueHandleFieldID = nullptr;

        static bool InitializeCache(JNIEnv* Environment)
        {
            if (bIsInitialized)
                return true;
            
            const CScopedLocalRef Class(Environment, Environment->FindClass("me/ayydxn/luminescence/javascript/JSValue"));
            if (!Class)
                return false;
            
            ClassRef = (jclass) Environment->NewGlobalRef(Class);
            Constructor = Environment->GetMethodID(ClassRef, "<init>", "(JJ)V");
            ValueHandleFieldID = Environment->GetFieldID(ClassRef, "valueHandle", "J");
            
            bIsInitialized = (ClassRef && Constructor && ValueHandleFieldID);
            
            return bIsInitialized;
        }

        static void ClearCache(JNIEnv* Environment)
        {
            if (ClassRef)
                Environment->DeleteGlobalRef(ClassRef);
            
            ClassRef = nullptr;
            bIsInitialized = false;
        }
    private:
        inline static bool bIsInitialized = false;
    };

    struct CJSTypeCache
    {
        inline static jclass ClassRef = nullptr;
        inline static jmethodID FromNativeMethodID = nullptr;

        static bool InitializeCache(JNIEnv* Environment)
        {
            if (bIsInitialized)
                return true;
            
            const CScopedLocalRef Class(Environment, Environment->FindClass("me/ayydxn/luminescence/javascript/JSType"));
            if (!Class)
                return false;
            
            ClassRef = (jclass) Environment->NewGlobalRef(Class);
            FromNativeMethodID = Environment->GetStaticMethodID(ClassRef, "fromNativeValue", "(I)Lme/ayydxn/luminescence/javascript/JSType;");
            bIsInitialized = (ClassRef && FromNativeMethodID);
            
            return bIsInitialized;
        }

        static void ClearCache(JNIEnv* Env)
        {
            if (ClassRef) Env->DeleteGlobalRef(ClassRef);
            ClassRef = nullptr;
            bIsInitialized = false;
        }
    private:
        inline static bool bIsInitialized = false;
    };
    
    struct CJSObjectCache
    {
        inline static jclass ClassRef = nullptr;
        inline static jmethodID Constructor = nullptr;

        static bool InitializeCache(JNIEnv* Environment)
        {
            if (bIsInitialized)
                return true;
            
            const CScopedLocalRef Class(Environment, Environment->FindClass("me/ayydxn/luminescence/javascript/JSObject"));
            if (!Class) return false;
            
            ClassRef = (jclass) Environment->NewGlobalRef(Class);
            Constructor = Environment->GetMethodID(ClassRef, "<init>", "(JJ)V");
            
            bIsInitialized = (ClassRef && Constructor);
            
            return bIsInitialized;
        }

        static void ClearCache(JNIEnv* Environment)
        {
            if (ClassRef)
                Environment->DeleteGlobalRef(ClassRef);
            
            ClassRef = nullptr;
            bIsInitialized = false;
        }
    private:
        inline static bool bIsInitialized = false;
    };

    struct CJSArrayCache
    {
        inline static jclass ClassRef = nullptr;
        inline static jmethodID Constructor = nullptr;

        static bool InitializeCache(JNIEnv* Environment)
        {
            if (bIsInitialized)
                return true;
            
            const CScopedLocalRef Class(Environment, Environment->FindClass("me/ayydxn/luminescence/javascript/JSArray"));
            if (!Class)
                return false;
            
            ClassRef = (jclass) Environment->NewGlobalRef(Class);
            Constructor = Environment->GetMethodID(ClassRef, "<init>", "(JJ)V");
            
            bIsInitialized = (ClassRef && Constructor);
            
            return bIsInitialized;
        }

        static void ClearCache(JNIEnv* Environment)
        {
            if (ClassRef)
                Environment->DeleteGlobalRef(ClassRef);
            
            ClassRef = nullptr;
            bIsInitialized = false;
        }
    private:
        inline static bool bIsInitialized = false;
    };

    struct CJSFunctionCache
    {
        inline static jclass ClassRef = nullptr;
        inline static jmethodID Constructor = nullptr;
        inline static jmethodID CallbackInvokeID = nullptr;

        static bool InitializeCache(JNIEnv* Environment)
        {
            if (bIsInitialized)
                return true;
            
            const CScopedLocalRef Class(Environment, Environment->FindClass("me/ayydxn/luminescence/javascript/JSFunction"));
            if (!Class)
                return false;
            
            ClassRef = (jclass) Environment->NewGlobalRef(Class);
            Constructor = Environment->GetMethodID(ClassRef, "<init>", "(JJ)V");
            
            const CScopedLocalRef Callback(Environment, Environment->FindClass("me/ayydxn/luminescence/javascript/JSFunction$JSCallback"));
            if (Callback)
            {
                CallbackInvokeID = Environment->GetMethodID(Callback.Get(), "invoke",
                                                    "(Lme/ayydxn/luminescence/javascript/JSContext;Lme/ayydxn/luminescence/javascript/JSObject;[Lme/ayydxn/luminescence/javascript/JSValue;)Lme/ayydxn/luminescence/javascript/JSValue;");
            }
            
            bIsInitialized = (ClassRef && Constructor && CallbackInvokeID);
            
            return bIsInitialized;
        }

        static void ClearCache(JNIEnv* Environment)
        {
            if (ClassRef)
                Environment->DeleteGlobalRef(ClassRef);
            
            ClassRef = nullptr;
            bIsInitialized = false;
        }
    private:
        inline static bool bIsInitialized = false;
    };

    struct CJSContextCache
    {
        inline static jclass ClassRef = nullptr;
        inline static jmethodID Constructor = nullptr;
        
        static bool InitializeCache(JNIEnv* Environment)
        {
            if (bIsInitialized)
                return true;
            
            const CScopedLocalRef Class(Environment, Environment->FindClass("me/ayydxn/luminescence/javascript/JSContext"));
            if (!Class)
                return false;
            
            ClassRef = (jclass)Environment->NewGlobalRef(Class);
            Constructor = Environment->GetMethodID(ClassRef, "<init>", "(JJ)V");
            
            bIsInitialized = (ClassRef && Constructor);
            
            return bIsInitialized;
        }

        static void ClearCache(JNIEnv* Environment)
        {
            if (ClassRef)
                Environment->DeleteGlobalRef(ClassRef);
            
            ClassRef = nullptr;
            bIsInitialized = false;
        }
    private:
        inline static bool bIsInitialized = false;
    };
    
    struct CStringCache
    {
        inline static jclass ClassRef = nullptr;

        static bool InitializeCache(JNIEnv* Environment)
        {
            if (ClassRef)
                return true;

            const CScopedLocalRef Class(Environment, Environment->FindClass("java/lang/String"));
            if (!Class)
                return false;
            
            ClassRef = static_cast<jclass>(Environment->NewGlobalRef(Class));
            
            return ClassRef != nullptr;
        }

        static void ClearCache(JNIEnv* Env)
        {
            if (ClassRef)
                Env->DeleteGlobalRef(ClassRef);
            
            ClassRef = nullptr;
        }
    };
}
