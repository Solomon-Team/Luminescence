package me.ayydxn.luminescence;

import me.ayydxn.luminescence.internal.UltralightNativeLoader;
import org.junit.jupiter.api.BeforeAll;

import java.io.File;
import java.nio.file.Files;
import java.nio.file.Path;

public abstract class BaseLuminescenceTest
{
    @BeforeAll
    static void setup()
    {
        UltralightNativeLoader.load();
    }
}
