package me.ayydxn.luminescence.demo;

import me.ayydxn.luminescence.Ultralight;
import me.ayydxn.luminescence.internal.UltralightNativeLoader;

public class LWJGLDemoApp
{
    private static final boolean GPU_DEMO = false;

    public static void main(String[] args) throws Exception
    {
        UltralightNativeLoader.load();

        System.out.printf("Running LWJGL %s Demo... (Ultralight Version: %s)\n", GPU_DEMO ? "GPU" : "CPU", Ultralight.ulVersionString());

        if (GPU_DEMO)
        {
            new LWJGLGPUDemoApp().run();
        }
        else
        {
            new LWJGLCPUDemoApp().run();
        }
    }
}
