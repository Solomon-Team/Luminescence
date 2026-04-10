package me.ayydxn.luminescence.platform;

import me.ayydxn.luminescence.BaseLuminescenceTest;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertThrows;

public class ULPlatformTest extends BaseLuminescenceTest
{
    @Test
    @DisplayName("Correctly Handles Null Logger")
    void testNullLoggerHandling()
    {
        assertThrows(IllegalArgumentException.class, () -> ULPlatform.setLogger(null));
    }
}
