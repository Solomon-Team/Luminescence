package me.ayydxn.luminescence.bitmap;

import org.jetbrains.annotations.ApiStatus;

import java.nio.ByteBuffer;
import java.nio.file.Path;

public class ULBitmap
{
    private long handle;

    @ApiStatus.Internal
    public ULBitmap(long handle)
    {
        this.handle = handle;
    }

    public static ULBitmap createEmpty()
    {
        return new ULBitmap(NativeMethods.nulCreateEmptyBitmap());
    }

    public static ULBitmap create(int width, int height, BitmapFormat format)
    {
        return new ULBitmap(NativeMethods.nulCreateBitmap(width, height, format.ordinal()));
    }

    public static ULBitmap createFromPixels(int width, int height, BitmapFormat format, ByteBuffer pixels, boolean shouldCopy)
    {
        if (!pixels.isDirect())
            throw new IllegalArgumentException("Bitmap pixels must be a direct ByteBuffer!");

        int bytesPerPixel = format.getBytesPerPixel();
        int rowBytes = width * bytesPerPixel;
        long capacity = pixels.capacity();
        long requiredSize = (long) rowBytes * height;

        if (capacity < requiredSize)
        {
            throw new IllegalArgumentException(String.format("ByteBuffer capacity (%d bytes) is too small for %dx%d %s bitmap (requires %d bytes)!",
                    capacity, width, height, format.name(), requiredSize));
        }

        long bitmapHandle = NativeMethods.nulCreateBitmapFromPixels(width, height, format.ordinal(), rowBytes, pixels, capacity, shouldCopy);
        if (bitmapHandle == 0)
        {
            throw new IllegalStateException("Failed to create native ULBitmap! This usually occurs if the ByteBuffer address " +
                            "could not be retrieved by JNI or the native engine encountered an allocation error.");
        }

        return new ULBitmap(bitmapHandle);
    }

    public static ULBitmap createFromCopy(ULBitmap otherBitmap)
    {
        return new ULBitmap(NativeMethods.nulCreateBitmapFromCopy(otherBitmap.handle));
    }

    public void destroy()
    {
        if (this.handle != 0)
        {
            NativeMethods.nulDestroyBitmap(this.handle);
            this.handle = 0;
        }
    }

    public int getWidth()
    {
        return NativeMethods.nulBitmapGetWidth(this.handle);
    }

    public int getHeight()
    {
        return NativeMethods.nulBitmapGetHeight(this.handle);
    }

    public BitmapFormat getFormat()
    {
        return BitmapFormat.fromNativeValue(NativeMethods.nulBitmapGetFormat(this.handle));
    }

    public int getBytesPerPixel()
    {
        return NativeMethods.nulBitmapGetBpp(this.handle);
    }

    public int getRowBytes()
    {
        return NativeMethods.nulBitmapGetRowBytes(this.handle);
    }

    public int getSize()
    {
        return NativeMethods.nulBitmapGetSize(this.handle);
    }

    public boolean ownsPixels()
    {
        return NativeMethods.nulBitmapOwnsPixels(this.handle);
    }

    public ByteBuffer lockPixels()
    {
        return NativeMethods.nulBitmapLockPixels(this.handle);
    }

    public void unlockPixels()
    {
        NativeMethods.nulBitmapUnlockPixels(this.handle);
    }

    public LockedPixels acquirePixelLock()
    {
        return new LockedPixels();
    }

    public ByteBuffer rawPixels()
    {
        return NativeMethods.nulBitmapRawPixels(this.handle);
    }

    public boolean isEmpty()
    {
        return NativeMethods.nulBitmapIsEmpty(this.handle);
    }

    public void erase()
    {
        NativeMethods.nulBitmapErase(this.handle);
    }

    public boolean writePNG(Path path)
    {
        return NativeMethods.nulBitmapWritePNG(this.handle, path.toString());
    }

    public void swapRedBlueChannels()
    {
        NativeMethods.nulBitmapSwapRedBlueChannels(this.handle);
    }

    public final class LockedPixels implements AutoCloseable
    {
        private final ByteBuffer pixels;

        public LockedPixels()
        {
            this.pixels = ULBitmap.this.lockPixels();
        }

        public ByteBuffer pixels()
        {
            return this.pixels;
        }

        @Override
        public void close()
        {
            ULBitmap.this.unlockPixels();
        }
    }

    private static final class NativeMethods
    {
        private static native long nulCreateEmptyBitmap();

        private static native long nulCreateBitmap(int width, int height, int bitmapFormatOrdinal);

        private static native long nulCreateBitmapFromPixels(int width, int height, int bitmapFormatOrdinal, int rowBytes, ByteBuffer pixels, long size, boolean shouldCopy);

        private static native long nulCreateBitmapFromCopy(long otherHandle);

        private static native void nulDestroyBitmap(long handle);

        private static native int nulBitmapGetWidth(long handle);

        private static native int nulBitmapGetHeight(long handle);

        private static native int nulBitmapGetFormat(long handle);

        private static native int nulBitmapGetBpp(long handle);

        private static native int nulBitmapGetRowBytes(long handle);

        private static native int nulBitmapGetSize(long handle);

        private static native boolean nulBitmapOwnsPixels(long handle);

        private static native ByteBuffer nulBitmapLockPixels(long handle);

        private static native void nulBitmapUnlockPixels(long handle);

        private static native ByteBuffer nulBitmapRawPixels(long handle);

        private static native boolean nulBitmapIsEmpty(long handle);

        private static native void nulBitmapErase(long handle);

        private static native boolean nulBitmapWritePNG(long handle, String path);

        private static native void nulBitmapSwapRedBlueChannels(long handle);
    }
}
