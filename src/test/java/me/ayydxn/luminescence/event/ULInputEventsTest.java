package me.ayydxn.luminescence.event;

import me.ayydxn.luminescence.BaseLuminescenceTest;
import me.ayydxn.luminescence.events.*;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertDoesNotThrow;
import static org.junit.jupiter.api.Assertions.assertNotEquals;

public class ULInputEventsTest extends BaseLuminescenceTest
{
    @Test
    @DisplayName("Verify Key Event Creation and Native Handle Lifecycle")
    void testKeyEventCreation()
    {
        assertDoesNotThrow(() ->
        {
            try (ULKeyEvent keyEvent = new ULKeyEvent(
                    KeyEventType.KEY_DOWN,
                    0,      // modifiers
                    65,     // virtualKeyCode (A)
                    30,     // nativeKeyCode
                    "a",    // text
                    "a",    // unmodifiedText
                    false,  // isKeypad
                    false,  // isAutoRepeat
                    false   // isSystemKey
            ))
            {
                assertNotEquals(0, keyEvent.getHandle());
            }
        });
    }

    @Test
    @DisplayName("Verify Mouse Event Creation and Native Handle Lifecycle")
    void testMouseEventCreation()
    {
        try (ULMouseEvent mouseEvent = new ULMouseEvent(MouseEventType.MOUSE_MOVED, 100, 200, MouseButton.NONE))
        {
            assertNotEquals(0, mouseEvent.getHandle());
        }
    }

    @Test
    @DisplayName("Verify Scroll Event Creation and Native Handle Lifecycle")
    void testScrollEventCreation()
    {
        try (ULScrollEvent scrollEvent = new ULScrollEvent(ScrollEventType.SCROLL_BY_PAGE, 0, -120))
        {
            assertNotEquals(0, scrollEvent.getHandle());
        }
    }
}
