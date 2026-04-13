package me.ayydxn.luminescence.renderer;

import me.ayydxn.luminescence.config.ULConfig;
import me.ayydxn.luminescence.events.ULGamepadEvent;
import org.jetbrains.annotations.ApiStatus;

public class ULRenderer
{
    private long handle;

    public ULRenderer(ULConfig config)
    {
        this.handle = NativeMethods.nulCreateRenderer(config.getHandle());
    }

    public void destroy()
    {
        if (this.handle != 0)
        {
            NativeMethods.nulDestroyRenderer(this.handle);
            this.handle = 0;
        }
    }

    public void update()
    {
        NativeMethods.nulUpdate(this.handle);
    }

    public void refreshDisplay(int displayID)
    {
        NativeMethods.nulRefreshDisplay(this.handle, displayID);
    }

    public void render()
    {
        NativeMethods.nulRender(this.handle);
    }

    public void purgeMemory()
    {
        NativeMethods.nulPurgeMemory(this.handle);
    }

    public void logMemoryUsage()
    {
        NativeMethods.nulLogMemoryUsage(this.handle);
    }

    public boolean startRemoteInspectorServer(String address, int port)
    {
        return NativeMethods.nulStartRemoteInspectorServer(this.handle, address, port);
    }

    public void setGamepadDetails(int index, String id, int axisCount, int buttonCount)
    {
        NativeMethods.nulSetGamepadDetails(this.handle, index, id, axisCount, buttonCount);
    }

    public void fireGamepadEvent(ULGamepadEvent gamepadEvent)
    {
        this.validateGamepadEventType(gamepadEvent, ULGamepadEvent.Type.REGULAR);

        NativeMethods.nulFireGamepadEvent(this.handle, gamepadEvent.getHandle());
    }

    public void fireGamepadAxisEvent(ULGamepadEvent gamepadEvent)
    {
        this.validateGamepadEventType(gamepadEvent, ULGamepadEvent.Type.AXIS);

        NativeMethods.nulFireGamepadAxisEvent(this.handle, gamepadEvent.getHandle());
    }

    public void fireGamepadButtonEvent(ULGamepadEvent gamepadEvent)
    {
        this.validateGamepadEventType(gamepadEvent, ULGamepadEvent.Type.BUTTON);

        NativeMethods.nulFireGamepadButtonEvent(this.handle, gamepadEvent.getHandle());
    }

    private void validateGamepadEventType(ULGamepadEvent gamepadEvent, ULGamepadEvent.Type expectedEventType)
    {
        if (gamepadEvent.getType() != expectedEventType)
        {
            String message = String.format("Mismatched Gamepad Event! This method requires a %s event, but the provided event is of type %s. " +
                            "Ensure you are using the correct factory method (e.g., ULGamepadEvent.createAxis()).", expectedEventType,
                    gamepadEvent.getType());

            throw new IllegalArgumentException(message);
        }
    }

    @ApiStatus.Internal
    public long getHandle()
    {
        return this.handle;
    }

    private static final class NativeMethods
    {
        private static native long nulCreateRenderer(long configHandle);

        private static native void nulDestroyRenderer(long handle);

        private static native void nulUpdate(long handle);

        private static native void nulRefreshDisplay(long handle, int displayID);

        private static native void nulRender(long handle);

        private static native void nulPurgeMemory(long handle);

        private static native void nulLogMemoryUsage(long handle);

        private static native boolean nulStartRemoteInspectorServer(long handle, String address, int port);

        private static native void nulSetGamepadDetails(long handle, int index, String id, int axisCount, int buttonCount);

        private static native void nulFireGamepadEvent(long handle, long gamepadEvent);

        private static native void nulFireGamepadAxisEvent(long handle, long axisGamepadEvent);

        private static native void nulFireGamepadButtonEvent(long handle, long buttonGamepadEvent);
    }
}
