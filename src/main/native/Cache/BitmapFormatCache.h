#pragma once

#include "Core/ScopedLocalRef.h"

#include <jni.h>

namespace Luminescence
{
    struct CBitmapFormatCache
    {
        inline static jclass ClassRef = nullptr;
        inline static jmethodID FromNativeMethodID = nullptr;

        static bool InitializeCache(JNIEnv* Environment)
        {
            if (ClassRef)
                return true;
            jclass LocalClass = Environment->FindClass("me/ayydxn/luminescence/bitmap/BitmapFormat");

            if (!LocalClass)
                return false;

            ClassRef = (jclass)Environment->NewGlobalRef(LocalClass);
            FromNativeMethodID = Environment->GetStaticMethodID(ClassRef, "fromNativeValue", "(I)Lme/ayydxn/luminescence/bitmap/BitmapFormat;");

            return ClassRef && FromNativeMethodID;
        }

        static void ClearCache(JNIEnv* Environment)
        {
            if (ClassRef)
            {
                Environment->DeleteGlobalRef(ClassRef);

                ClassRef = nullptr;
            }
        }
    };
}