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
            throw new IllegalArgumentException("Cannot set the platform logger to a null ULLogger instance!");

        NativeMethods.nulPlatformSetLogger(logger);
    }

    public static void setFileSystem(ULFileSystem fileSystem)
    {
        if (fileSystem == null)
            throw new IllegalArgumentException("Cannot set the platform filesystem to a null ULFileSystem instance!");

        NativeMethods.nulPlatformSetFileSystem(fileSystem);
    }

    public static void setClipboard(ULClipboard clipboard)
    {
        if (clipboard == null)
            throw new IllegalArgumentException("Cannot set the platform clipboard to a null ULClipboard instance!");

        NativeMethods.nulPlatformSetClipboard(clipboard);
    }

    public static void setFontLoader(ULFontLoader fontLoader)
    {
        if (fontLoader == null)
            throw new IllegalArgumentException("Cannot set the platform font loader to a null ULFontLoader instance!");

        NativeMethods.nulPlatformSetFontLoader(fontLoader);
    }

    public static void setGPUDriver(ULGPUDriver gpuDriver)
    {
        if (gpuDriver == null)
            throw new IllegalArgumentException("Cannot set the platform GPU driver to a null ULGPUDriver instance!");

        NativeMethods.nulPlatformSetGPUDriver(gpuDriver);
    }

    private static final class NativeMethods
    {
        private static native void nulPlatformSetLogger(ULLogger logger);

        private static native void nulPlatformSetFileSystem(ULFileSystem fileSystem);

        private static native void nulPlatformSetClipboard(ULClipboard clipboard);

        private static native void nulPlatformSetFontLoader(ULFontLoader fontLoader);

        private static native void nulPlatformSetGPUDriver(ULGPUDriver gpuDriver);

        /** ADDED BY LUMINESCENCE: Used to primarily by the C++ side to clean up any callback adapters we have created. */
        private static native void nulPlatformShutdown();
    }
}
