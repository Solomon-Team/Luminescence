package me.ayydxn.luminescence.view;

import me.ayydxn.luminescence.BaseLuminescenceTest;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertDoesNotThrow;

public class ULViewConfigTest extends BaseLuminescenceTest
{
    @Test
    @DisplayName("Ensure View Config Can Be Created and Destroyed Cleanly")
    void testViewConfigLifecycle()
    {
        try (ULViewConfig config = new ULViewConfig())
        {
            assertDoesNotThrow(() ->
            {
                config.isAccelerated(true);
                config.setTransparent(true);
                config.setInitialDeviceScale(2.0);
                config.setUserAgent("Luminescence-Bot/1.0");
            });
        }
    }
}
