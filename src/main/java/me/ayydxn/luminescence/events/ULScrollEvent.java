package me.ayydxn.luminescence.events;

import org.jetbrains.annotations.ApiStatus;

public class ULScrollEvent implements AutoCloseable
{
    private long handle;

    public ULScrollEvent(ScrollEventType type, int deltaX, int deltaY)
    {
        this.handle = NativeMethods.nulCreateScrollEvent(type.ordinal(), deltaX, deltaY);
    }

    @Override
    public void close()
    {
        if (this.handle != 0)
        {
            NativeMethods.nulDestroyScrollEvent(this.handle);
            this.handle = 0;
        }
    }

    @ApiStatus.Internal
    public long getHandle()
    {
        return this.handle;
    }

    private static final class NativeMethods
    {
        private static native long nulCreateScrollEvent(int eventTypeOrdinal, int deltaX, int deltaY);

        private static native void nulDestroyScrollEvent(long handle);
    }
}
