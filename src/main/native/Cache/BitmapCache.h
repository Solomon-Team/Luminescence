#pragma once

#include "Core/ScopedLocalRef.h"

#include <jni.h>
#include <Ultralight/CAPI/CAPI_Defines.h>

namespace Luminescence
{
    struct CBitmapCache
    {
        inline static jclass ClassRef = nullptr;
        
        // Fields
        inline static jfieldID HandleFieldID = nullptr;

        static bool InitializeCache(JNIEnv* Environment)
        {
            if (ClassRef)
                return true;
            
            const CScopedLocalRef LocalClass(Environment, Environment->FindClass("me/ayydxn/luminescence/bitmap/ULBitmap"));
            if (!LocalClass)
                return false;

            ClassRef = (jclass) Environment->NewGlobalRef(LocalClass);
            
            HandleFieldID = Environment->GetFieldID(ClassRef, "handle", "J");
            
            return ClassRef;
        }

        static void ClearCache(JNIEnv* Environment)
        {
            if (ClassRef)
            {
                Environment->DeleteGlobalRef(ClassRef);

                ClassRef = nullptr;
            }
        }
        
        static ULBitmap GetNativeHandle(JNIEnv* Environment, jobject JavaBitmap)
        {
            if (!JavaBitmap)
                return nullptr;
            
           return reinterpret_cast<ULBitmap>(Environment->GetLongField(JavaBitmap, HandleFieldID));
        }
    };
}
