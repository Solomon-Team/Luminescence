#include "Cache/BitmapCache.h"
#include "Core/JNIUtilities.h"
#include "Core/Profiling.h"

#include <Ultralight/CAPI/CAPI_Bitmap.h>

/*
 * Class:     me_ayydxn_luminescence_bitmap_ULBitmap_NativeMethods
 * Method:    nulCreateEmptyBitmap
 * Signature: ()J
 */
jlong JNICALL ULCreateEmptyBitmap_Native(JNIEnv*, jclass)
{
    ZoneScoped
    
    return reinterpret_cast<jlong>(ulCreateEmptyBitmap());
}

/*
 * Class:     me_ayydxn_luminescence_bitmap_ULBitmap_NativeMethods
 * Method:    nulCreateBitmap
 * Signature: (III)J
 */
jlong JNICALL ULCreateBitmap_Native(JNIEnv*, jclass, jint Width, jint Height, jint BitmapFormatOrdinal)
{
    ZoneScoped
    
    const auto BitmapFormat = static_cast<ULBitmapFormat>(BitmapFormatOrdinal);

    return reinterpret_cast<jlong>(ulCreateBitmap(static_cast<unsigned int>(Width), static_cast<unsigned int>(Height), BitmapFormat));
}

/*
 * Class:     me_ayydxn_luminescence_bitmap_ULBitmap_NativeMethods
 * Method:    nulCreateBitmapFromPixels
 * Signature: (IIIILjava/nio/ByteBuffer;JZ)J
 */
jlong JNICALL ULCreateBitmapFromPixels_Native(JNIEnv* Environment, jclass, jint Width, jint Height, jint BitmapFormatOrdinal, jint RowBytes, jobject Pixels,
                                               jlong Size, jboolean bShouldCopy)
{
    ZoneScoped
    
    const auto BitmapFormat = static_cast<ULBitmapFormat>(BitmapFormatOrdinal);
    const void* PixelsBuffer = Environment->GetDirectBufferAddress(Pixels);

    if (!PixelsBuffer)
        return 0;

    return reinterpret_cast<jlong>(ulCreateBitmapFromPixels(
        static_cast<unsigned int>(Width),
        static_cast<unsigned int>(Height),
        BitmapFormat,
        static_cast<unsigned int>(RowBytes),
        PixelsBuffer,
        static_cast<size_t>(Size),
        static_cast<bool>(bShouldCopy)
    ));
}

/*
 * Class:     me_ayydxn_luminescence_bitmap_ULBitmap_NativeMethods
 * Method:    nulCreateBitmapFromCopy
 * Signature: (J)J
 */
jlong JNICALL ULCreateBitmapFromCopy_Native(JNIEnv*, jclass, jlong OtherBitmapHandle)
{
    ZoneScoped
    
    const auto ExistingBitmap = reinterpret_cast<ULBitmap>(OtherBitmapHandle);
    
    return reinterpret_cast<jlong>(ulCreateBitmapFromCopy(ExistingBitmap));
}

/*
 * Class:     me_ayydxn_luminescence_bitmap_ULBitmap_NativeMethods
 * Method:    nulDestroyBitmap
 * Signature: (J)V
 */
void JNICALL ULDestroyBitmap_Native(JNIEnv*, jclass, jlong BitmapHandle)
{
    ZoneScoped
    
    ulDestroyBitmap(reinterpret_cast<ULBitmap>(BitmapHandle));
}

/*
 * Class:     me_ayydxn_luminescence_bitmap_ULBitmap_NativeMethods
 * Method:    nulBitmapGetWidth
 * Signature: (J)I
 */
jint JNICALL ULBitmapGetWidth_Native(JNIEnv*, jclass, jlong BitmapHandle)
{
    ZoneScoped
    
    return static_cast<jint>(ulBitmapGetWidth(reinterpret_cast<ULBitmap>(BitmapHandle)));
}

/*
 * Class:     me_ayydxn_luminescence_bitmap_ULBitmap_NativeMethods
 * Method:    nulBitmapGetHeight
 * Signature: (J)I
 */
jint JNICALL ULBitmapGetHeight_Native(JNIEnv*, jclass, jlong BitmapHandle)
{
    ZoneScoped
    
    return static_cast<jint>(ulBitmapGetHeight(reinterpret_cast<ULBitmap>(BitmapHandle)));
}

/*
 * Class:     me_ayydxn_luminescence_bitmap_ULBitmap_NativeMethods
 * Method:    nulBitmapGetFormat
 * Signature: (J)I
 */
jint JNICALL ULBitmapGetFormat_Native(JNIEnv*, jclass, jlong BitmapHandle)
{
    ZoneScoped
    
    return ulBitmapGetFormat(reinterpret_cast<ULBitmap>(BitmapHandle));
}

/*
 * Class:     me_ayydxn_luminescence_bitmap_ULBitmap_NativeMethods
 * Method:    nulBitmapGetBpp
 * Signature: (J)I
 */
jint JNICALL ULBitmapGetBpp_Native(JNIEnv*, jclass, jlong BitmapHandle)
{
    ZoneScoped
    
    return static_cast<jint>(ulBitmapGetBpp(reinterpret_cast<ULBitmap>(BitmapHandle)));
}

/*
 * Class:     me_ayydxn_luminescence_bitmap_ULBitmap_NativeMethods
 * Method:    nulBitmapGetRowBytes
 * Signature: (J)I
 */
jint JNICALL ULBitmapGetRowBytes_Native(JNIEnv*, jclass, jlong BitmapHandle)
{
    ZoneScoped
    
    return static_cast<jint>(ulBitmapGetRowBytes(reinterpret_cast<ULBitmap>(BitmapHandle)));
}

/*
 * Class:     me_ayydxn_luminescence_bitmap_ULBitmap_NativeMethods
 * Method:    nulBitmapGetSize
 * Signature: (J)I
 */
jint JNICALL ULBitmapGetSize_Native(JNIEnv*, jclass, jlong BitmapHandle)
{
    ZoneScoped
    
    return static_cast<jint>(ulBitmapGetSize(reinterpret_cast<ULBitmap>(BitmapHandle)));
}

/*
 * Class:     me_ayydxn_luminescence_bitmap_ULBitmap_NativeMethods
 * Method:    nulBitmapOwnsPixels
 * Signature: (J)Z
 */
jboolean JNICALL ULBitmapOwnsPixels_Native(JNIEnv*, jclass, jlong BitmapHandle)
{
    ZoneScoped
    
    return ulBitmapOwnsPixels(reinterpret_cast<ULBitmap>(BitmapHandle));
}

/*
 * Class:     me_ayydxn_luminescence_bitmap_ULBitmap_NativeMethods
 * Method:    nulBitmapLockPixels
 * Signature: (J)Ljava/nio/ByteBuffer;
 */
jobject JNICALL ULBitmapLockPixels_Native(JNIEnv* Environment, jclass, jlong BitmapHandle)
{
    ZoneScoped
    
    const auto PixelsBuffer = ulBitmapLockPixels(reinterpret_cast<ULBitmap>(BitmapHandle));
    const unsigned int Width = ulBitmapGetWidth(reinterpret_cast<ULBitmap>(BitmapHandle));
    const unsigned int Height = ulBitmapGetHeight(reinterpret_cast<ULBitmap>(BitmapHandle));
    const unsigned int BytesPerPixel = ulBitmapGetBpp(reinterpret_cast<ULBitmap>(BitmapHandle));
    const unsigned int BufferCapacity = Width * Height * BytesPerPixel;
    
    return Environment->NewDirectByteBuffer(PixelsBuffer, static_cast<jlong>(BufferCapacity));
}

/*
 * Class:     me_ayydxn_luminescence_bitmap_ULBitmap_NativeMethods
 * Method:    nulBitmapUnlockPixels
 * Signature: (J)V
 */
void JNICALL ULBitmapUnlockPixels_Native(JNIEnv*, jclass, jlong BitmapHandle)
{
    ZoneScoped
    
    ulBitmapUnlockPixels(reinterpret_cast<ULBitmap>(BitmapHandle));
}

/*
 * Class:     me_ayydxn_luminescence_bitmap_ULBitmap_NativeMethods
 * Method:    nulBitmapRawPixels
 * Signature: (J)Ljava/nio/ByteBuffer;
 */
jobject JNICALL ULBitmapRawPixels_Native(JNIEnv* Environment, jclass, jlong BitmapHandle)
{
    ZoneScoped
    
    const auto Bitmap = reinterpret_cast<ULBitmap>(BitmapHandle);

    void* PixelsBuffer = ulBitmapRawPixels(Bitmap);
    if (!PixelsBuffer)
        return nullptr;
    
    const jlong Capacity = static_cast<jlong>(ulBitmapGetSize(Bitmap));
    
    return Environment->NewDirectByteBuffer(PixelsBuffer, Capacity);
}

/*
 * Class:     me_ayydxn_luminescence_bitmap_ULBitmap_NativeMethods
 * Method:    nulBitmapIsEmpty
 * Signature: (J)Z
 */
jboolean JNICALL ULBitmapIsEmpty_Native(JNIEnv*, jclass, jlong BitmapHandle)
{
    ZoneScoped
    
    return ulBitmapIsEmpty(reinterpret_cast<ULBitmap>(BitmapHandle));
}

/*
 * Class:     me_ayydxn_luminescence_bitmap_ULBitmap_NativeMethods
 * Method:    nulBitmapErase
 * Signature: (J)V
 */
void JNICALL ULBitmapErase_Native(JNIEnv*, jclass, jlong BitmapHandle)
{
    ZoneScoped
    
    ulBitmapErase(reinterpret_cast<ULBitmap>(BitmapHandle));
}

/*
 * Class:     me_ayydxn_luminescence_bitmap_ULBitmap_NativeMethods
 * Method:    nulBitmapWritePNG
 * Signature: (JLjava/lang/String;)Z
 */
jboolean JNICALL ULBitmapWritePNG_Native(JNIEnv* Environment, jclass, jlong BitmapHandle, jstring PathString)
{
    ZoneScoped
    
    const auto Bitmap = reinterpret_cast<ULBitmap>(BitmapHandle);
    const auto PathStringChars = Environment->GetStringUTFChars(PathString, nullptr);
    
    const bool Result = ulBitmapWritePNG(Bitmap, PathStringChars);
    
    Environment->ReleaseStringUTFChars(PathString, PathStringChars);
    
    return Result;
}

/*
 * Class:     me_ayydxn_luminescence_bitmap_ULBitmap_NativeMethods
 * Method:    nulBitmapSwapRedBlueChannels
 * Signature: (J)V
 */
void JNICALL ULBitmapSwapRedBlueChannels_Native(JNIEnv*, jclass, jlong BitmapHandle)
{
    ZoneScoped
    
    ulBitmapSwapRedBlueChannels(reinterpret_cast<ULBitmap>(BitmapHandle));
}

static constexpr JNINativeMethod BitmapMethods[] =
{
    JNI_METHOD("nulCreateEmptyBitmap", "()J", ULCreateEmptyBitmap_Native),
    JNI_METHOD("nulCreateBitmap", "(III)J", ULCreateBitmap_Native),
    JNI_METHOD("nulCreateBitmapFromPixels", "(IIIILjava/nio/ByteBuffer;JZ)J", ULCreateBitmapFromPixels_Native),
    JNI_METHOD("nulCreateBitmapFromCopy", "(J)J", ULCreateBitmapFromCopy_Native),
    JNI_METHOD("nulDestroyBitmap", "(J)V", ULDestroyBitmap_Native),
    JNI_METHOD("nulBitmapGetWidth", "(J)I", ULBitmapGetWidth_Native),
    JNI_METHOD("nulBitmapGetHeight", "(J)I", ULBitmapGetHeight_Native),
    JNI_METHOD("nulBitmapGetFormat", "(J)I", ULBitmapGetFormat_Native),
    JNI_METHOD("nulBitmapGetBpp", "(J)I", ULBitmapGetBpp_Native),
    JNI_METHOD("nulBitmapGetRowBytes", "(J)I", ULBitmapGetRowBytes_Native),
    JNI_METHOD("nulBitmapGetSize", "(J)I", ULBitmapGetSize_Native),
    JNI_METHOD("nulBitmapOwnsPixels", "(J)Z", ULBitmapOwnsPixels_Native),
    JNI_METHOD("nulBitmapLockPixels", "(J)Ljava/nio/ByteBuffer;", ULBitmapLockPixels_Native),
    JNI_METHOD("nulBitmapUnlockPixels", "(J)V", ULBitmapUnlockPixels_Native),
    JNI_METHOD("nulBitmapRawPixels", "(J)Ljava/nio/ByteBuffer;", ULBitmapRawPixels_Native),
    JNI_METHOD("nulBitmapIsEmpty", "(J)Z", ULBitmapIsEmpty_Native),
    JNI_METHOD("nulBitmapErase", "(J)V", ULBitmapErase_Native),
    JNI_METHOD("nulBitmapWritePNG", "(JLjava/lang/String;)Z", ULBitmapWritePNG_Native),
    JNI_METHOD("nulBitmapSwapRedBlueChannels", "(J)V", ULBitmapSwapRedBlueChannels_Native)
};

bool Luminescence::RegisterBitmapMethods(JNIEnv* Environment)
{
    if (!CBitmapCache::InitializeCache(Environment))
        return false;
    
    return RegisterNativeMethods(Environment, "me/ayydxn/luminescence/bitmap/ULBitmap$NativeMethods", JNI_METHODS_AND_COUNT(BitmapMethods));
}
