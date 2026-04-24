package me.ayydxn.luminescence.surface;

import me.ayydxn.luminescence.geometry.ULIntRect;
import org.jetbrains.annotations.ApiStatus;

import java.nio.ByteBuffer;

public class ULSurface
{
    protected final long handle;

    @ApiStatus.Internal
    public ULSurface(long handle)
    {
        this.handle = handle;
    }

    public int getWidth()
    {
        return NativeMethods.nulSurfaceGetWidth(this.handle);
    }

    public int getHeight()
    {
        return NativeMethods.nulSurfaceGetHeight(this.handle);
    }

    public int getRowBytes()
    {
        return NativeMethods.nulSurfaceGetRowBytes(this.handle);
    }

    public long getSize()
    {
        return NativeMethods.nulSurfaceGetSize(this.handle);
    }

    public ByteBuffer lockPixels()
    {
        return NativeMethods.nulSurfaceLockPixels(this.handle);
    }

    public void unlockPixels()
    {
        NativeMethods.nulSurfaceUnlockPixels(this.handle);
    }

    public LockedPixels acquirePixelLock()
    {
        return new LockedPixels();
    }

    public void resize(int width, int height)
    {
        NativeMethods.nulSurfaceResize(this.handle, width, height);
    }

    public void setDirtyBounds(ULIntRect dirtyBounds)
    {
        NativeMethods.nulSurfaceSetDirtyBounds(this.handle, dirtyBounds);
    }

    public ULIntRect getDirtyBounds()
    {
        return NativeMethods.nulSurfaceGetDirtyBounds(this.handle);
    }

    public void clearDirtyBounds()
    {
        NativeMethods.nulSurfaceClearDirtyBounds(this.handle);
    }

    public long getHandle()
    {
        return this.handle;
    }

    public final class LockedPixels implements AutoCloseable
    {
        private final ByteBuffer pixels;

        public LockedPixels()
        {
            this.pixels = ULSurface.this.lockPixels();
        }

        public ByteBuffer pixels()
        {
            return this.pixels;
        }

        @Override
        public void close()
        {
            ULSurface.this.unlockPixels();
        }
    }

    private static final class NativeMethods
    {
        private static native int nulSurfaceGetWidth(long handle);

        private static native int nulSurfaceGetHeight(long handle);

        private static native int nulSurfaceGetRowBytes(long handle);

        private static native long nulSurfaceGetSize(long handle);

        private static native ByteBuffer nulSurfaceLockPixels(long hande);

        private static native void nulSurfaceUnlockPixels(long hande);

        private static native void nulSurfaceResize(long handle, int width, int height);

        private static native void nulSurfaceSetDirtyBounds(long handle, ULIntRect intRect);

        private static native ULIntRect nulSurfaceGetDirtyBounds(long handle);

        private static native void nulSurfaceClearDirtyBounds(long handle);

        // TODO: (Ayydxn) Add ulSurfaceGetUserData?
    }
}
