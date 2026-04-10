package me.ayydxn.luminescence.platform;

public class ULPlatform
{
    static
    {
        Runtime.getRuntime().addShutdownHook(new Thread(NativeMethods::nulPlatformShutdown, "Luminescence-Shutdown"));
    }

    private ULPlatform()
    {
    }

    public static void setLogger(ULLogger logger)
    {
        if (logger == null)
            throw new IllegalArgumentException("You cannot set the platform logger to a null ULLogger instance!");

        NativeMethods.nulPlatformSetLogger(logger);
    }

    private static final class NativeMethods
    {
        private static native void nulPlatformSetLogger(ULLogger logger);

        /** -- ADDED BY LUMINESCENCE: Used to primarily by the C++ to clean up any callback adapters we have created. */
        private static native void nulPlatformShutdown();
    }
}
