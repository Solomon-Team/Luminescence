#pragma once

#include "Core/ScopedLocalRef.h"

#include <Ultralight/CAPI/CAPI_Defines.h>

#include <jni.h>

namespace Luminescence
{
    struct CIntRectCache
    {
        inline static jclass ClassRef = nullptr;
        inline static jmethodID ConstructorID = nullptr;

        // Fields
        inline static jfieldID LeftFieldID = nullptr;
        inline static jfieldID TopFieldID = nullptr;
        inline static jfieldID RightFieldID = nullptr;
        inline static jfieldID BottomFieldID = nullptr;

        static bool InitializeCache(JNIEnv* Environment)
        {
            if (bIsCacheInitialized)
                return true;

            const CScopedLocalRef LocalClass(Environment, Environment->FindClass("me/ayydxn/luminescence/geometry/ULIntRect"));
            if (!LocalClass)
                return false;

            ClassRef = (jclass)Environment->NewGlobalRef(LocalClass.Get());
            ConstructorID = Environment->GetMethodID(ClassRef, "<init>", "(IIII)V");

            LeftFieldID = Environment->GetFieldID(ClassRef, "left", "I");
            TopFieldID = Environment->GetFieldID(ClassRef, "top", "I");
            RightFieldID = Environment->GetFieldID(ClassRef, "right", "I");
            BottomFieldID = Environment->GetFieldID(ClassRef, "bottom", "I");

            bIsCacheInitialized = (ClassRef != nullptr);

            return bIsCacheInitialized;
        }

        static void ClearCache(JNIEnv* Environment)
        {
            if (ClassRef)
            {
                Environment->DeleteGlobalRef(ClassRef);

                ClassRef = nullptr;
            }

            bIsCacheInitialized = false;
        }

        static jobject CreateIntRect(JNIEnv* Environment, const ULIntRect& Rect)
        {
            return Environment->NewObject(ClassRef, ConstructorID, static_cast<jint>(Rect.left), static_cast<jint>(Rect.top), static_cast<jint>(Rect.right),
                static_cast<jint>(Rect.bottom));
        }
    private:
        inline static bool bIsCacheInitialized = false;
    };
}