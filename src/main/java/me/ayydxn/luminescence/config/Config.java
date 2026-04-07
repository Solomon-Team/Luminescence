package me.ayydxn.luminescence.config;

import me.ayydxn.luminescence.util.ULString;

import java.util.function.BiConsumer;

public class Config implements AutoCloseable
{
    private long handle;

    public Config()
    {
        this.handle = NativeMethods.nulCreateConfig();
    }

    @Override
    public void close()
    {
        if (this.handle != 0)
        {
            NativeMethods.nulDestroyConfig(handle);

            this.handle = 0;
        }
    }

    public void setCachePath(String cachePath)
    {
        this.withNativeString(cachePath, NativeMethods::nulConfigSetCachePath);
    }

    public void setResourcePathPrefix(String resourcePathPrefix)
    {
        this.withNativeString(resourcePathPrefix, NativeMethods::nulConfigSetResourcePathPrefix);
    }

    public void setFaceWinding(FaceWinding faceWinding)
    {
        NativeMethods.nulConfigSetFaceWinding(this.handle, faceWinding.getValue());
    }

    public void setFontHinting(FontHinting fontHinting)
    {
        NativeMethods.nulConfigSetFontHinting(this.handle, fontHinting.getValue());
    }

    public void setFontGamma(double fontGamma)
    {
        NativeMethods.nulConfigSetFontGamma(this.handle, fontGamma);
    }

    public void setUserStylesheet(String stylesheetString)
    {
        this.withNativeString(stylesheetString, NativeMethods::nulConfigSetUserStylesheet);
    }

    public void setForceRepaint(boolean isEnabled)
    {
        NativeMethods.nulConfigSetForceRepaint(this.handle, isEnabled);
    }

    public void setAnimationTimerDelay(double delay)
    {
        NativeMethods.nulConfigSetAnimationTimerDelay(this.handle, delay);
    }

    public void setScrollTimerDelay(double delay)
    {
        NativeMethods.nulConfigSetScrollTimerDelay(this.handle, delay);
    }

    public void setRecycleDelay(double delay)
    {
        NativeMethods.nulConfigSetRecycleDelay(this.handle, delay);
    }

    public void setMemoryCacheSize(int size)
    {
        NativeMethods.nulConfigSetMemoryCacheSize(this.handle, size);
    }

    public void setPageCacheSize(int size)
    {
        NativeMethods.nulConfigSetPageCacheSize(this.handle, size);
    }

    public void setOverrideRAMSize(int size)
    {
        NativeMethods.nulConfigSetOverrideRAMSize(this.handle, size);
    }

    public void setMinLargeHeapSize(int size)
    {
        NativeMethods.nulConfigSetMinLargeHeapSize(this.handle, size);
    }

    public void setMinSmallHeapSize(int size)
    {
        NativeMethods.nulConfigSetMinSmallHeapSize(this.handle, size);
    }

    public void setNumRendererThreads(int size)
    {
        NativeMethods.nulConfigSetNumRendererThreads(this.handle, size);
    }

    public void setMaxUpdateTime(double delay)
    {
        NativeMethods.nulConfigSetMaxUpdateTime(this.handle, delay);
    }

    public void setBitmapAlignment(int size)
    {
        NativeMethods.nulConfigSetBitmapAlignment(this.handle, size);
    }

    /**
     * Helper method to safely pass a Java String to a native function as a {@code ULString}.
     * <p>
     * This method handles the creation of a temporary native string, executes the
     * provided action, and ensures the native string is destroyed immediately
     * after the call returns, preventing memory leaks.
     *
     * @param value The Java string to pass to the native layer.
     * @param nativeMethod The native method reference to execute.
     */
    private void withNativeString(String value, BiConsumer<Long, Long> nativeMethod)
    {
        try (ULString ulString = ULString.create(value))
        {
            nativeMethod.accept(this.handle, ulString.getHandle());
        }
    }

    private static final class NativeMethods
    {
        private static native long nulCreateConfig();
        private static native void nulDestroyConfig(long handle);
        private static native void nulConfigSetCachePath(long configHandle, long cachePathStringHandle);
        private static native void nulConfigSetResourcePathPrefix(long configHandle, long resourcePathPrefixStringHandle);
        private static native void nulConfigSetFaceWinding(long configHandle, int faceWinding);
        private static native void nulConfigSetFontHinting(long configHandle, int fontHinting);
        private static native void nulConfigSetFontGamma(long configHandle, double fontGamma);
        private static native void nulConfigSetUserStylesheet(long configHandle, long cssStringHandle);
        private static native void nulConfigSetForceRepaint(long configHandle, boolean enabled);
        private static native void nulConfigSetAnimationTimerDelay(long configHandle, double delay);
        private static native void nulConfigSetScrollTimerDelay(long configHandle, double delay);
        private static native void nulConfigSetRecycleDelay(long configHandle, double delay);
        private static native void nulConfigSetMemoryCacheSize(long configHandle, int size);
        private static native void nulConfigSetPageCacheSize(long configHandle, int size);
        private static native void nulConfigSetOverrideRAMSize(long configHandle, int size);
        private static native void nulConfigSetMinLargeHeapSize(long configHandle, int size);
        private static native void nulConfigSetMinSmallHeapSize(long configHandle, int size);
        private static native void nulConfigSetNumRendererThreads(long configHandle, int numThreads);
        private static native void nulConfigSetMaxUpdateTime(long configHandle, double maxUpdateTime);
        private static native void nulConfigSetBitmapAlignment(long configHandle, int alignment);
    }
}
