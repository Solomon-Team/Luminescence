package me.ayydxn.luminescence;

import me.ayydxn.luminescence.internal.UltralightNativeLoader;
import org.junit.jupiter.api.BeforeAll;

public abstract class BaseLuminescenceTest
{
    @BeforeAll
    static void setup()
    {
        UltralightNativeLoader.load();
    }
}
