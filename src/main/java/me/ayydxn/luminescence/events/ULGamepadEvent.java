package me.ayydxn.luminescence.events;

import org.jetbrains.annotations.ApiStatus;

public class ULGamepadEvent implements AutoCloseable
{
    private final Type type;

    private long handle;

    private ULGamepadEvent(long handle, Type type)
    {
        this.handle = handle;
        this.type = type;
    }

    public static ULGamepadEvent create(int index, GamepadEventType eventType)
    {
        long handle = NativeMethods.nulCreateGamepadEvent(index, eventType.ordinal());

        return new ULGamepadEvent(handle, Type.REGULAR);
    }

    public static ULGamepadEvent createAxis(int index, int axisIndex, double value)
    {
        long handle = NativeMethods.nulCreateGamepadAxisEvent(index, axisIndex, value);

        return new ULGamepadEvent(handle, Type.AXIS);
    }

    public static ULGamepadEvent createButton(int index, int buttonIndex, double value)
    {
        long handle = NativeMethods.nulCreateGamepadButtonEvent(index, buttonIndex, value);

        return new ULGamepadEvent(handle, Type.BUTTON);
    }

    @Override
    public void close()
    {
        if (this.handle != 0)
        {
            switch (this.type)
            {
                case REGULAR -> NativeMethods.nulDestroyGamepadEvent(this.handle);
                case AXIS -> NativeMethods.nulDestroyGamepadAxisEvent(this.handle);
                case BUTTON -> NativeMethods.nulDestroyGamepadButtonEvent(this.handle);
            }

            this.handle = 0;
        }
    }

    @ApiStatus.Internal
    public Type getType()
    {
        return this.type;
    }

    @ApiStatus.Internal
    public long getHandle()
    {
        return this.handle;
    }

    @ApiStatus.Internal
    public enum Type
    {
        REGULAR,
        AXIS,
        BUTTON
    }

    private static final class NativeMethods
    {
        private static native long nulCreateGamepadEvent(int index, int eventTypeOrdinal);

        private static native void nulDestroyGamepadEvent(long handle);

        private static native long nulCreateGamepadAxisEvent(int index, int axisIndex, double value);

        private static native void nulDestroyGamepadAxisEvent(long handle);

        private static native long nulCreateGamepadButtonEvent(int index, int buttonIndex, double value);

        private static native void nulDestroyGamepadButtonEvent(long handle);
    }
}
