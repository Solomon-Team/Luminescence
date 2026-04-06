package me.ayydxn.luminescence.events;

public enum KeyEventType
{
    /**
     * This type does not trigger accelerator commands in WebCore (eg, Ctrl+C for copy is an accelerator command).
     * <p>
     * <h6><b>Warning</b></h6>
     * <p>
     * You should probably use kType_RawKeyDown instead. This type is only here for historic compatibility with WebCore's key event types.
     */
    KEY_DOWN,

    /** Use this when a physical key is released. */
    KEY_UP,

    /**
     * Use this when a physical key is pressed.
     */
    RAW_KEY_DOWN,

    /**
     * Use this when the OS generates text from a physical key being pressed (for example, this maps to WM_CHAR on Windows).
     */
    CHAR
}
