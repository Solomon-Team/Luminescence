package me.ayydxn.luminescence.util;

import java.nio.ByteBuffer;

public class ULBuffer implements AutoCloseable
{
    private long handle;

    private ULBuffer(long handle)
    {
        this.handle = handle;
    }

    public static ULBuffer create(ByteBuffer data)
    {
        if (data == null || !data.isDirect())
            throw new IllegalArgumentException("Buffer must be a non-null direct ByteBuffer!");

        return new ULBuffer(NativeMethods.nulCreateBufferFromDirect(data, data.capacity()));
    }

    public static ULBuffer createFromCopy(byte[] data)
    {
        if (data == null)
            throw new IllegalArgumentException("Cannot create a ULBuffer from null data!");

        return new ULBuffer(NativeMethods.nulCreateBufferFromCopy(data));
    }

    @Override
    public void close()
    {
        if (this.handle != 0)
        {
            NativeMethods.nulDestroyBuffer(this.handle);
            this.handle = 0;
        }
    }

    public long getSize()
    {
        return NativeMethods.nulBufferGetSize(this.handle);
    }

    public boolean ownsData()
    {
        return NativeMethods.nulBufferOwnsData(this.handle);
    }

    /**
     * Copies buffer data into a new byte array.
     *
     * @apiNote Use sparingly — This is an allocation + copy on every call.
     */
    public byte[] toByteArray()
    {
        return NativeMethods.nulBufferToByteArray(this.handle);
    }

    public long getHandle()
    {
        return this.handle;
    }

    private static final class NativeMethods
    {
        private static native long nulCreateBufferFromCopy(byte[] data);

        private static native long nulCreateBufferFromDirect(ByteBuffer buffer, int capacity);

        private static native void nulDestroyBuffer(long handle);

        private static native long nulBufferGetSize(long handle);

        private static native boolean nulBufferOwnsData(long handle);

        private static native byte[] nulBufferToByteArray(long handle);
    }
}
