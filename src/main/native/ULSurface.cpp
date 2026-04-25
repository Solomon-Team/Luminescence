#include "Cache/IntRectCache.h"
#include "Core/JNIUtilities.h"

#include <Ultralight/CAPI/CAPI_Surface.h>

/*
 * Class:     me_ayydxn_luminescence_surface_ULSurface_NativeMethods
 * Method:    nulSurfaceGetWidth
 * Signature: (J)I
 */
jint JNICALL ULSurfaceGetWidth_Native(JNIEnv*, jclass, jlong SurfaceHandle)
{
	const auto Surface = reinterpret_cast<ULSurface>(SurfaceHandle);

	return static_cast<int>(ulSurfaceGetWidth(Surface));
}

/*
 * Class:     me_ayydxn_luminescence_surface_ULSurface_NativeMethods
 * Method:    nulSurfaceGetHeight
 * Signature: (J)I
 */
jint JNICALL ULSurfaceGetHeight_Native(JNIEnv*, jclass, jlong SurfaceHandle)
{
	const auto Surface = reinterpret_cast<ULSurface>(SurfaceHandle);

	return static_cast<int>(ulSurfaceGetHeight(Surface));
}

/*
 * Class:     me_ayydxn_luminescence_surface_ULSurface_NativeMethods
 * Method:    nulSurfaceGetRowBytes
 * Signature: (J)I
 */
jint JNICALL ULSurfaceGetRowBytes_Native(JNIEnv*, jclass, jlong SurfaceHandle)
{
	const auto Surface = reinterpret_cast<ULSurface>(SurfaceHandle);

	return static_cast<int>(ulSurfaceGetRowBytes(Surface));
}

/*
 * Class:     me_ayydxn_luminescence_surface_ULSurface_NativeMethods
 * Method:    nulSurfaceGetSize
 * Signature: (J)J
 */
jlong JNICALL ULSurfaceGetSize_Native(JNIEnv*, jclass, jlong SurfaceHandle)
{
	const auto Surface = reinterpret_cast<ULSurface>(SurfaceHandle);

	return static_cast<jlong>(ulSurfaceGetSize(Surface));
}

/*
 * Class:     me_ayydxn_luminescence_surface_ULSurface_NativeMethods
 * Method:    nulSurfaceLockPixels
 * Signature: (J)Ljava/nio/ByteBuffer;
 */
jobject JNICALL ULSurfaceLockPixels_Native(JNIEnv* Environment, jclass, jlong SurfaceHandle)
{
	const auto PixelsBuffer = ulSurfaceLockPixels(reinterpret_cast<ULSurface>(SurfaceHandle));
	const auto BufferSize = ulSurfaceGetSize(reinterpret_cast<ULSurface>(SurfaceHandle));

	return Environment->NewDirectByteBuffer(PixelsBuffer, static_cast<jlong>(BufferSize));
}

/*
 * Class:     me_ayydxn_luminescence_surface_ULSurface_NativeMethods
 * Method:    nulSurfaceUnlockPixels
 * Signature: (J)V
 */
void JNICALL ULSurfaceUnlockPixels_Native(JNIEnv*, jclass, jlong SurfaceHandle)
{
	ulSurfaceUnlockPixels(reinterpret_cast<ULSurface>(SurfaceHandle));
}

/*
 * Class:     me_ayydxn_luminescence_surface_ULSurface_NativeMethods
 * Method:    nulSurfaceResize
 * Signature: (JII)V
 */
void JNICALL ULSurfaceResize_Native(JNIEnv*, jclass, jlong SurfaceHandle, jint Width, jint Height)
{
	const auto Surface = reinterpret_cast<ULSurface>(SurfaceHandle);

	ulSurfaceResize(Surface, static_cast<unsigned int>(Width), static_cast<unsigned int>(Height));
}

/*
 * Class:     me_ayydxn_luminescence_surface_ULSurface_NativeMethods
 * Method:    nulSurfaceSetDirtyBounds
 * Signature: (JLme/ayydxn/luminescence/geometry/ULIntRect;)V
 */
void JNICALL ULSurfaceSetDirtyBounds_Native(JNIEnv* Environment, jclass, jlong SurfaceHandle, jobject DirtyBounds)
{
	using namespace Luminescence;

	const ULIntRect DirtyBoundsRect = {
		.left = Environment->GetIntField(DirtyBounds, CIntRectCache::LeftFieldID),
		.top = Environment->GetIntField(DirtyBounds, CIntRectCache::TopFieldID),
		.right = Environment->GetIntField(DirtyBounds, CIntRectCache::RightFieldID),
		.bottom = Environment->GetIntField(DirtyBounds, CIntRectCache::BottomFieldID)
	};

    ulSurfaceSetDirtyBounds(reinterpret_cast<ULSurface>(SurfaceHandle), DirtyBoundsRect);
}

/*
 * Class:     me_ayydxn_luminescence_surface_ULSurface_NativeMethods
 * Method:    nulSurfaceGetDirtyBounds
 * Signature: (J)Lme/ayydxn/luminescence/geometry/ULIntRect;
 */
jobject JNICALL ULSurfaceGetDirtyBounds_Native(JNIEnv* Environment, jclass, jlong SurfaceHandle)
{
	using namespace Luminescence;

	const auto Surface = reinterpret_cast<ULSurface>(SurfaceHandle);
	const ULIntRect DirtyBounds = ulSurfaceGetDirtyBounds(Surface);

	return CIntRectCache::CreateIntRect(Environment, DirtyBounds);
}

/*
 * Class:     me_ayydxn_luminescence_surface_ULSurface_NativeMethods
 * Method:    nulSurfaceClearDirtyBounds
 * Signature: (J)V
 */
void JNICALL ULSurfaceClearDirtyBounds_Native(JNIEnv*, jclass, jlong SurfaceHandle)
{
	ulSurfaceClearDirtyBounds(reinterpret_cast<ULSurface>(SurfaceHandle));
}

/*
 * Class:     me_ayydxn_luminescence_surface_ULBitmapSurface_NativeMethods
 * Method:    nulBitmapSurfaceGetBitmap
 * Signature: (J)J
 */
jlong JNICALL ULBitmapSurfaceGetBitmap_Native(JNIEnv*, jclass, jlong SurfaceHandle)
{
	const auto BitmapSurface = reinterpret_cast<ULBitmapSurface>(SurfaceHandle);

	return reinterpret_cast<jlong>(ulBitmapSurfaceGetBitmap(BitmapSurface));
}

static constexpr JNINativeMethod SurfaceMethods[] = {
	JNI_METHOD("nulSurfaceGetWidth", "(J)I", ULSurfaceGetWidth_Native),
	JNI_METHOD("nulSurfaceGetHeight", "(J)I", ULSurfaceGetHeight_Native),
	JNI_METHOD("nulSurfaceGetRowBytes", "(J)I", ULSurfaceGetRowBytes_Native),
	JNI_METHOD("nulSurfaceGetSize", "(J)J", ULSurfaceGetSize_Native),
	JNI_METHOD("nulSurfaceLockPixels", "(J)Ljava/nio/ByteBuffer;", ULSurfaceLockPixels_Native),
	JNI_METHOD("nulSurfaceUnlockPixels", "(J)V", ULSurfaceUnlockPixels_Native),
	JNI_METHOD("nulSurfaceResize", "(JII)V", ULSurfaceResize_Native),
	JNI_METHOD("nulSurfaceSetDirtyBounds", "(JLme/ayydxn/luminescence/geometry/ULIntRect;)V", ULSurfaceSetDirtyBounds_Native),
	JNI_METHOD("nulSurfaceGetDirtyBounds", "(J)Lme/ayydxn/luminescence/geometry/ULIntRect;", ULSurfaceGetDirtyBounds_Native),
	JNI_METHOD("nulSurfaceClearDirtyBounds", "(J)V", ULSurfaceClearDirtyBounds_Native)
};

static constexpr JNINativeMethod BitmapSurfaceMethods[] = {
	JNI_METHOD("nulBitmapSurfaceGetBitmap", "(J)J", ULBitmapSurfaceGetBitmap_Native)
};

bool Luminescence::RegisterSurfaceMethods(JNIEnv* Environment)
{
	if (!Luminescence::CIntRectCache::InitializeCache(Environment))
		return false;

	const bool bSurfaceMethodsRegistration = RegisterNativeMethods(Environment, "me/ayydxn/luminescence/surface/ULSurface$NativeMethods", JNI_METHODS_AND_COUNT(SurfaceMethods));
	const bool bBitmapSurfaceMethodsRegistration = RegisterNativeMethods(Environment, "me/ayydxn/luminescence/surface/ULBitmapSurface$NativeMethods",
		JNI_METHODS_AND_COUNT(BitmapSurfaceMethods));

	return bSurfaceMethodsRegistration && bBitmapSurfaceMethodsRegistration;
}