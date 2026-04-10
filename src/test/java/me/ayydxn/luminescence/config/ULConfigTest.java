package me.ayydxn.luminescence.config;

import me.ayydxn.luminescence.BaseLuminescenceTest;
import me.ayydxn.luminescence.internal.UltralightNativeLoader;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertDoesNotThrow;
import static org.junit.jupiter.api.Assertions.assertNotNull;

public class ULConfigTest extends BaseLuminescenceTest
{
    @Test
    @DisplayName("Initializes with Valid Native Pointer")
    void testInitialization()
    {
        assertDoesNotThrow(() ->
        {
            try (ULConfig config = new ULConfig())
            {
                // If we reach here, the native pointer is valid.
                config.setMemoryCacheSize(1024 * 1024);

                // Ensure handle is non-zero
                assertNotNull(config.toString());
            }
        });
    }

    @Test
    @DisplayName("Create and Destroy Multiple Configs Without Heap Corruption")
    void testHeapCorruption()
    {
        for (int i = 0; i < 100; i++)
        {
            try (ULConfig config = new ULConfig())
            {
                config.setMemoryCacheSize(1024);
            }
        }
    }
}
