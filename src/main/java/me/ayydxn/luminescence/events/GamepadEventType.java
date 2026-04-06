package me.ayydxn.luminescence.events;

public enum GamepadEventType
{
    // TODO: (Ayydxn) Reference the equivalent Java methods when they get added
    /**
     * This event type should be fired when a gamepad is connected.
     * <p>
     * <b>NOTE:</b> You will need to previously declare the gamepad, its index, and details about its axis and button layout via
     * Renderer::SetGamepadDetails prior to calling Renderer::FireGamepadEvent.
     */
    CONNECTED,

    /**
     * This event type should be fired when a gamepad is disconnected.
     */
    DISCONNECTED
}
