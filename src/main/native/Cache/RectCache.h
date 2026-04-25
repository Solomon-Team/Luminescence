#pragma once

#include "Core/ScopedLocalRef.h"

#include <jni.h>

namespace Luminescence
{
    struct CRectCache
    {
        inline static jclass ClassRef = nullptr;
        inline static jmethodID ConstructorID = nullptr;

        static bool InitializeCache(JNIEnv* Environment)
        {
            if (ClassRef)
                return true;
            
            jclass LocalClass = Environment->FindClass("me/ayydxn/luminescence/geometry/ULRect");

            if (!LocalClass)
                return false;

            ClassRef = (jclass) Environment->NewGlobalRef(LocalClass);
            ConstructorID = Environment->GetMethodID(ClassRef, "<init>", "(FFFF)V");

            return ClassRef && ConstructorID;
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