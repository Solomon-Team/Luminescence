#include "Cache/RectCache.h"
#include "Core/JNIUtilities.h"

#include <Ultralight/CAPI/CAPI_ImageSource.h>

#include "Cache/BitmapCache.h"

using namespace Luminescence;

/*
 * Class:     me_ayydxn_luminescence_util_ULImageSource_NativeMethods
 * Method:    nulCreateImageSourceFromTexture
 * Signature: (IIILme/ayydxn/luminescence/geometry/ULRect;Lme/ayydxn/luminescence/bitmap/ULBitmap;)J
 */
jlong JNICALL ULCreateImageSourceFromTexture_Native(JNIEnv* Environment, jclass, jint Width, jint Height, jint TextureID, jobject TextureUVInstance, jobject BitmapInstance)
{
    const ULRect TextureUV = TextureUVInstance != nullptr ? CRectCache::GetNativeHandle(Environment, TextureUVInstance) :
        ULRect { .left = 0.0f, .top = 0.0f, .right = 1.0f, .bottom = 1.0f }; // Safe defaults if null gets passed for the texture UV parameter.
    
    const ULBitmap Bitmap = BitmapInstance != nullptr ? CBitmapCache::GetNativeHandle(Environment, BitmapInstance) : nullptr;
    
    const ULImageSource TextureImageSource = ulCreateImageSourceFromTexture(static_cast<unsigned int>(Width), static_cast<unsigned int>(Height), static_cast<unsigned int>(TextureID),
        TextureUV, Bitmap);
    
    return reinterpret_cast<jlong>(TextureImageSource);
}

/*
 * Class:     me_ayydxn_luminescence_util_ULImageSource_NativeMethods
 * Method:    nulCreateImageSourceFromBitmap
 * Signature: (Lme/ayydxn/luminescence/bitmap/ULBitmap;)J
 */
#include <iostream>

jlong JNICALL ULCreateImageSourceFromBitmap_Native(JNIEnv* Environment, jclass, jobject BitmapInstance)
{
    const auto Bitmap = CBitmapCache::GetNativeHandle(Environment, BitmapInstance);
    
    return reinterpret_cast<jlong>(ulCreateImageSourceFromBitmap(Bitmap));
}

/*
 * Class:     me_ayydxn_luminescence_util_ULImageSource_NativeMethods
 * Method:    nulDestroyImageSource
 * Signature: (J)V
 */
void JNICALL ULDestroyImageSource_Native(JNIEnv*, jclass, jlong ImageSourceHandle)
{
    ulDestroyImageSource(reinterpret_cast<ULImageSource>(ImageSourceHandle));
}

/*
 * Class:     me_ayydxn_luminescence_util_ULImageSource_NativeMethods
 * Method:    nulImageSourceInvalidate
 * Signature: (J)V
 */
void JNICALL ULImageSourceInvalidate_Native(JNIEnv*, jclass, jlong ImageSourceHandle)
{
    ulImageSourceInvalidate(reinterpret_cast<ULImageSource>(ImageSourceHandle));
}

/*
 * Class:     me_ayydxn_luminescence_util_ULImageSource_NativeMethods
 * Method:    nulImageSourceProviderAddImageSource
 * Signature: (Ljava/lang/String;J)V
 */
void JNICALL ULImageSourceProviderAddImageSource_Native(JNIEnv* Environment, jclass, jstring ImageSourceID, jlong ImageSourceHandle)
{
    const auto ImageSource = reinterpret_cast<ULImageSource>(ImageSourceHandle);
    const auto IDChars = Environment->GetStringUTFChars(ImageSourceID, nullptr);
    const auto IDString = ulCreateString(IDChars);
    
    ulImageSourceProviderAddImageSource(IDString, ImageSource);
    
    ulDestroyString(IDString);
    Environment->ReleaseStringUTFChars(ImageSourceID, IDChars);
}

/*
 * Class:     me_ayydxn_luminescence_util_ULImageSource_NativeMethods
 * Method:    nulImageSourceProviderRemoveImageSource
 * Signature: (Ljava/lang/String;)V
 */
void JNICALL ULImageSourceProviderRemoveImageSource_Native(JNIEnv* Environment, jclass, jstring ImageSourceID)
{
    const auto IDChars = Environment->GetStringUTFChars(ImageSourceID, nullptr);
    const auto IDString = ulCreateString(IDChars);
    
    ulImageSourceProviderRemoveImageSource(IDString);

    ulDestroyString(IDString);
    Environment->ReleaseStringUTFChars(ImageSourceID, IDChars);
}

static constexpr JNINativeMethod ImageSourceMethods[] =
{
    JNI_METHOD("nulCreateImageSourceFromTexture", "(IIILme/ayydxn/luminescence/geometry/ULRect;Lme/ayydxn/luminescence/bitmap/ULBitmap;)J", ULCreateImageSourceFromTexture_Native),
    JNI_METHOD("nulCreateImageSourceFromBitmap", "(Lme/ayydxn/luminescence/bitmap/ULBitmap;)J", ULCreateImageSourceFromBitmap_Native),
    JNI_METHOD("nulDestroyImageSource", "(J)V", ULDestroyImageSource_Native),
    JNI_METHOD("nulImageSourceInvalidate", "(J)V", ULImageSourceInvalidate_Native),
    JNI_METHOD("nulImageSourceProviderAddImageSource", "(Ljava/lang/String;J)V", ULImageSourceProviderAddImageSource_Native),
    JNI_METHOD("nulImageSourceProviderRemoveImageSource", "(Ljava/lang/String;)V", ULImageSourceProviderRemoveImageSource_Native)
};

bool Luminescence::RegisterImageSourceMethods(JNIEnv* Environment)
{
    return RegisterNativeMethods(Environment, "me/ayydxn/luminescence/util/ULImageSource$NativeMethods", JNI_METHODS_AND_COUNT(ImageSourceMethods));
}