package me.ayydxn.luminescence.util;

import me.ayydxn.luminescence.bitmap.ULBitmap;
import me.ayydxn.luminescence.geometry.ULRect;
import org.jetbrains.annotations.ApiStatus;

public class ULImageSource
{
    private long handle;

    private ULImageSource(long handle)
    {
        this.handle = handle;
    }

    public static ULImageSource createFromTexture(int width, int height, int textureID, ULRect textureUV, ULBitmap bitmap)
    {
        long handle = NativeMethods.nulCreateImageSourceFromTexture(width, height, textureID, textureUV, bitmap);

        return new ULImageSource(handle);
    }

    public static ULImageSource createFromBitmap(ULBitmap bitmap)
    {
        return new ULImageSource(NativeMethods.nulCreateImageSourceFromBitmap(bitmap));
    }

    public void destroy()
    {
        if (this.handle != 0)
        {
            NativeMethods.nulDestroyImageSource(this.handle);
            this.handle = 0;
        }
    }

    public void invalidate()
    {
        NativeMethods.nulImageSourceInvalidate(this.handle);
    }

    public void addImageSourceToProvider(String id)
    {
        NativeMethods.nulImageSourceProviderAddImageSource(id, this.handle);
    }

    public void removeImageSourceFromProvider(String id)
    {
        NativeMethods.nulImageSourceProviderRemoveImageSource(id);
    }

    @ApiStatus.Internal
    public long getHandle()
    {
        return this.handle;
    }

    private static final class NativeMethods
    {
        private static native long nulCreateImageSourceFromTexture(int width, int height, int textureID, ULRect textureUV, ULBitmap bitmap);

        private static native long nulCreateImageSourceFromBitmap(ULBitmap bitmap);

        private static native void nulDestroyImageSource(long handle);

        private static native void nulImageSourceInvalidate(long handle);

        private static native void nulImageSourceProviderAddImageSource(String id, long handle);

        private static native void nulImageSourceProviderRemoveImageSource(String id);
    }
}
