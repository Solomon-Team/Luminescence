#pragma once

#include "Core/ScopedLocalRef.h"

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

			ClassRef = (jclass)Environment->NewGlobalRef(LocalClass);
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

	struct CRenderTargetCache
	{
		inline static jclass ClassRef;
		inline static jmethodID Constructor;

		// Fields
		inline static jfieldID IsEmptyFieldID;
		inline static jfieldID WidthFieldID;
		inline static jfieldID HeightFieldID;
		inline static jfieldID TextureIDField;
		inline static jfieldID TextureWidthField;
		inline static jfieldID TextureHeightField;
		inline static jfieldID BitmapFormatFieldID;
		inline static jfieldID UVCoordsFieldID;
		inline static jfieldID RenderBufferFieldID;

		static bool InitializeCache(JNIEnv* Environment)
		{
			if (bIsInitialized) return true;

			if (!CBitmapFormatCache::InitializeCache(Environment) || !CRectCache::InitializeCache(Environment))
				return false;

			jclass LocalClass = Environment->FindClass("me/ayydxn/luminescence/view/ULRenderTarget");
			ClassRef = (jclass)Environment->NewGlobalRef(LocalClass);

			Constructor = Environment->GetMethodID(ClassRef, "<init>", "()V");

			IsEmptyFieldID = Environment->GetFieldID(ClassRef, "isEmpty", "Z");
			WidthFieldID = Environment->GetFieldID(ClassRef, "width", "I");
			HeightFieldID = Environment->GetFieldID(ClassRef, "height", "I");
			TextureIDField = Environment->GetFieldID(ClassRef, "textureID", "I");
			TextureWidthField = Environment->GetFieldID(ClassRef, "textureWidth", "I");
			TextureHeightField = Environment->GetFieldID(ClassRef, "textureHeight", "I");
			RenderBufferFieldID = Environment->GetFieldID(ClassRef, "renderBufferID", "I");
			BitmapFormatFieldID = Environment->GetFieldID(ClassRef, "bitmapFormat", "Lme/ayydxn/luminescence/bitmap/BitmapFormat;");
			UVCoordsFieldID = Environment->GetFieldID(ClassRef, "uvCoords", "Lme/ayydxn/luminescence/geometry/ULRect;");

			bIsInitialized = (ClassRef && Constructor);
			return bIsInitialized;
		}

		static void ClearCache(JNIEnv* Environment)
		{
			if (ClassRef)
			{
				Environment->DeleteGlobalRef(ClassRef);

				ClassRef = nullptr;
			}

			CBitmapFormatCache::ClearCache(Environment);
			CRectCache::ClearCache(Environment);

			bIsInitialized = false;
		}
	private:
		inline static bool bIsInitialized = false;
	};
}