#pragma once

#include "Core/ScopedLocalRef.h"

#include <jni.h>
#include <Ultralight/CAPI/CAPI_Defines.h>

namespace Luminescence
{
    struct CRectCache
    {
        inline static jclass ClassRef = nullptr;
        inline static jmethodID ConstructorID = nullptr;
        inline static jfieldID LeftFieldID = nullptr;
        inline static jfieldID TopFieldID = nullptr;
        inline static jfieldID RightFieldID = nullptr;
        inline static jfieldID BottomFieldID = nullptr;

        static bool InitializeCache(JNIEnv* Environment)
        {
            if (ClassRef)
                return true;

            const CScopedLocalRef LocalClass(Environment, Environment->FindClass("me/ayydxn/luminescence/geometry/ULRect"));
            if (!LocalClass)
                return false;

            ClassRef = (jclass) Environment->NewGlobalRef(LocalClass);
            ConstructorID = Environment->GetMethodID(ClassRef, "<init>", "(FFFF)V");

            LeftFieldID = Environment->GetFieldID(ClassRef, "left", "F");
            TopFieldID = Environment->GetFieldID(ClassRef, "top", "F");
            RightFieldID = Environment->GetFieldID(ClassRef, "right", "F");
            BottomFieldID = Environment->GetFieldID(ClassRef, "bottom", "F");
            
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
        
        static ULRect GetNativeHandle(JNIEnv* Environment, jobject JavaRect)
        {
            if (!JavaRect)
                return {};
            
            return {
                .left = Environment->GetFloatField(JavaRect, LeftFieldID),
                .top = Environment->GetFloatField(JavaRect, TopFieldID),
                .right = Environment->GetFloatField(JavaRect, RightFieldID),
                .bottom = Environment->GetFloatField(JavaRect, BottomFieldID)
            };
        }
    };
}