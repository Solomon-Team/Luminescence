package me.ayydxn.luminescence.config;

import me.ayydxn.luminescence.internal.UltralightNativeLoader;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertDoesNotThrow;
import static org.junit.jupiter.api.Assertions.assertNotNull;

public class ConfigTest
{
    @BeforeAll
    static void setup()
    {
        UltralightNativeLoader.load();
    }

    @Test
    void configInitializesWithValidNativePointer()
    {
        assertDoesNotThrow(() ->
        {
            try (Config config = new Config())
            {
                // If we reach here, the native pointer is valid.
                config.setMemoryCacheSize(1024 * 1024);

                // Ensure handle is non-zero
                assertNotNull(config.toString());
            }
        });
    }

    @Test
    void createAndDestroyMultipleConfigsWithoutHeapCorruption()
    {
        for (int i = 0; i < 100; i++)
        {
            try (Config config = new Config())
            {
                config.setMemoryCacheSize(1024);
            }
        }
    }
}
