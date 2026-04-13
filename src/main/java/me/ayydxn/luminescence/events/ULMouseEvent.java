package me.ayydxn.luminescence.events;

import org.jetbrains.annotations.ApiStatus;

public class ULMouseEvent implements AutoCloseable
{
    private long handle;

    public ULMouseEvent(MouseEventType type, int x, int y, MouseButton button)
    {
        this.handle = NativeMethods.nulCreateMouseEvent(type.ordinal(), x, y, button.ordinal());
    }

    @Override
    public void close()
    {
        if (this.handle != 0)
        {
            NativeMethods.nulDestroyMouseEvent(this.handle);
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
        private static native long nulCreateMouseEvent(int eventTypeOrdinal, int x, int y, int mouseButtonOrdinal);

        private static native void nulDestroyMouseEvent(long handle);
    }
}
