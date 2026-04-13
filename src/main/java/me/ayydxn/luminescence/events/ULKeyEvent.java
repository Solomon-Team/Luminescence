package me.ayydxn.luminescence.events;

import org.jetbrains.annotations.ApiStatus;

public class ULKeyEvent implements AutoCloseable
{
    private long handle;

    public ULKeyEvent(KeyEventType type, int modifiers, int virtualKeyCode, int nativeKeyCode, String text, String unmodifiedText, boolean isKeypad, boolean isAutoRepeat,
                      boolean isSystemKey)
    {
        this.handle = NativeMethods.nulCreateKeyEvent(type.ordinal(), modifiers, virtualKeyCode, nativeKeyCode, text, unmodifiedText, isKeypad, isAutoRepeat, isSystemKey);
    }

    @Override
    public void close()
    {
        if (this.handle != 0)
        {
            NativeMethods.nulDestroyKeyEvent(this.handle);
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
        private static native long nulCreateKeyEvent(int eventTypeOrdinal, int modifiers, int virtualKeyCode, int nativeKeyCode, String text, String unmodifiedText, boolean isKeypad, boolean isAutoRepeat, boolean isSystemKey);

        private static native void nulDestroyKeyEvent(long handle);
    }
}
