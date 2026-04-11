package me.ayydxn.luminescence.config;

import me.ayydxn.luminescence.util.ULString;
import org.jetbrains.annotations.ApiStatus;

import java.util.function.BiConsumer;

/*
 * TODO: (Ayydxn) Link to the corresponding Java classes/methods of the following when they are added:
 *  - Renderer
 */

/**
 * Core configuration for the renderer.
 * <p>
 * These are various configuration options that can be used to customize the behavior of the library.
 * These options can only be set once before creating the Renderer.
 * <p>
 * <h4>Setting the Config</h4>
 * You should create an instance of this class, set its options with the appropriate methods that are available,
 * and then pass it to the {@code Renderer}'s constructor.
 *
 * @author Ayydxn
 */
public class ULConfig implements AutoCloseable
{
    private long handle;

    public ULConfig()
    {
        this.handle = NativeMethods.nulCreateConfig();
    }

    /**
     * Destroys the config.
     *
     * @implNote This method will no-op if the config was already destroyed, making it safe to call multiple times.
     */
    @Override
    public void close()
    {
        if (this.handle != 0)
        {
            NativeMethods.nulDestroyConfig(handle);

            this.handle = 0;
        }
    }

    /**
     * Sets a writable OS file path to store persistent Session data in. This data may include cookies, cached network resources, indexed DB, etc.
     *
     * <h4>Note</h4>
     * Files are only written to the path when using a persistent Session.
     *
     * @param cachePath The filepath to use for the cache. This should be a valid directory on the user's system that the application has permission to write to.
     *                  The library will create any necessary subdirectories, if needed.
     */
    public void setCachePath(String cachePath)
    {
        this.withNativeString(cachePath, NativeMethods::nulConfigSetCachePath);
    }

    /**
     * Sets the relative path to the resources folder (loaded via the FileSystem API).
     * <p>
     * The library loads certain resources (SSL certs, ICU data, etc.) from the FileSystem API during runtime.
     * (For example {@code file:///resources/cacert.pem})
     * <p>
     * You can customize the relative file path to the resources folder by modifying this setting.
     *
     * @param resourcePathPrefix The relative path to the application's resources folder.
     */
    public void setResourcePathPrefix(String resourcePathPrefix)
    {
        this.withNativeString(resourcePathPrefix, NativeMethods::nulConfigSetResourcePathPrefix);
    }

    /**
     * Sets the winding order for front-facing triangles.
     *
     * <h4>Precondition</h4>
     * Only used when GPU rendering is enabled for a view.
     *
     * @param faceWinding The winding order to use for front-facing triangles.
     */
    public void setFaceWinding(FaceWinding faceWinding)
    {
        NativeMethods.nulConfigSetFaceWinding(this.handle, faceWinding.getValue());
    }

    /**
     * Sets the font hinting algorithm to use when rendering fonts.
     *
     * @param fontHinting The font hinting algorithm to use when rendering fonts.
     */
    public void setFontHinting(FontHinting fontHinting)
    {
        NativeMethods.nulConfigSetFontHinting(this.handle, fontHinting.getValue());
    }

    /**
     * Sets the gamma to use when compositing font glyphs, change this value to adjust contrast (Adobe and Apple prefer 1.8, others may prefer 2.2).
     *
     * @param fontGamma The gamma to use when compositing font glyphs.
     */
    public void setFontGamma(double fontGamma)
    {
        NativeMethods.nulConfigSetFontGamma(this.handle, fontGamma);
    }

    /**
     * Sets the global user-defined CSS string (included before any CSS on the page).
     * You can use this to override default styles for various elements on the page.
     *
     * <h4>Note</h4>
     * This is an actual string of CSS, not a filepath.
     *
     * @param stylesheetString The global user-defined CSS string to use.
     */
    public void setUserStylesheet(String stylesheetString)
    {
        this.withNativeString(stylesheetString, NativeMethods::nulConfigSetUserStylesheet);
    }

    // TODO: (Ayydxn) Link to view class.

    /**
     * Sets whether to continuously repaint any Views, regardless of if they are dirty.
     * <p>
     * This is mainly used to diagnose painting/shader issues and profile performance.
     *
     * @param isEnabled Whether to continuously repaint any views, regardless of if they are dirty.
     */
    public void setForceRepaint(boolean isEnabled)
    {
        NativeMethods.nulConfigSetForceRepaint(this.handle, isEnabled);
    }

    /**
     * Sets the <b>seconds</b> of delay between every tick of a CSS animation.
     * A lower value will result in smoother animations, but may increase CPU usage. A higher value will reduce CPU usage, but may result in choppier animations.
     *
     * @param delay The seconds of delay between every tick of a CSS animation.
     */
    public void setAnimationTimerDelay(double delay)
    {
        NativeMethods.nulConfigSetAnimationTimerDelay(this.handle, delay);
    }

    /**
     * Sets the <b>seconds</b> of delay between every tick of a smooth scroll animation.
     *
     * @param delay The seconds of delay between every tick of a smooth scroll animation.
     */
    public void setScrollTimerDelay(double delay)
    {
        NativeMethods.nulConfigSetScrollTimerDelay(this.handle, delay);
    }

    /**
     * Sets the seconds of delay between every call to the recycler.
     * <p>
     * The library attempts to reclaim excess memory during calls to the internal recycler.
     * You can change how often this is run by modifying this value.
     *
     * @param delay The seconds of delay between every call to the recycler.
     */
    public void setRecycleDelay(double delay)
    {
        NativeMethods.nulConfigSetRecycleDelay(this.handle, delay);
    }

    /**
     * Sets the size of WebCore's memory cache in <b>bytes</b>.
     *
     * <h4>Note</h4>
     * You should increase this if you anticipate handling pages with large resources, Safari typically uses 128+ MiB for its cache.
     *
     * @param size The size of WebCore's memory cache in bytes.
     */
    public void setMemoryCacheSize(int size)
    {
        NativeMethods.nulConfigSetMemoryCacheSize(this.handle, size);
    }

    /**
     * Sets the number of pages to keep in the cache.
     * <p>
     * This is <b>not</b> the same as the {@link ULConfig#setMemoryCacheSize(int) memory cache size} which is used for caching resources
     * such as images and scripts. This is used for caching entire rendered pages in memory,
     * which can improve performance when navigating back and forth between pages.
     *
     * <h4>Note</h4>
     * Safari typically caches about 5 pages and maintains an on-disk cache to support typical web-browsing activities.
     * <p>
     * If you increase this, you should probably increase the memory cache size as well.
     *
     * @param size The number of pages to keep in the cache.
     */
    public void setPageCacheSize(int size)
    {
        NativeMethods.nulConfigSetPageCacheSize(this.handle, size);
    }

    /**
     * Sets the system's physical RAM size in <b>bytes</b>.
     * <p>
     * JavaScriptCore tries to detect the system's physical RAM size to set reasonable allocation limits.
     * Set this to anything other than 0 to override the detected value. Size is in bytes.
     * <p>
     * This can be used to force JavaScriptCore to be more conservative with its allocation strategy (at the cost of some performance).
     *
     * @param size The system's physical RAM size in bytes.
     */
    public void setOverrideRAMSize(int size)
    {
        NativeMethods.nulConfigSetOverrideRAMSize(this.handle, size);
    }

    /**
     * Sets the minimum size of large VM heaps in JavaScriptCore.
     * <p>
     * Set this to a lower value to make these heaps start with a smaller initial value.
     *
     * @param size The minimum size of large VM heaps in JavaScriptCore.
     */
    public void setMinLargeHeapSize(int size)
    {
        NativeMethods.nulConfigSetMinLargeHeapSize(this.handle, size);
    }

    /**
     * Sets the minimum size of small VM heaps in JavaScriptCore.
     * <p>
     * Set this to a lower value to make these heaps start with a smaller initial value.
     *
     * @param size The minimum size of small VM heaps in JavaScriptCore.
     */
    public void setMinSmallHeapSize(int size)
    {
        NativeMethods.nulConfigSetMinSmallHeapSize(this.handle, size);
    }

    /**
     * Sets the number of threads to use in the Renderer (for parallel painting on the CPU, etc.).
     * You can set this to a certain number to limit the number of threads to spawn.
     *
     * <h4>Note</h4>
     * If this value is 0, the number of threads will be determined at runtime using the following formula:
     * <blockquote><pre>
     *     max(PhysicalProcessorCount() - 1, 1)
     * </pre></blockquote>
     *
     * @param size The number of threads to use in the Renderer.
     */
    public void setNumRendererThreads(int size)
    {
        NativeMethods.nulConfigSetNumRendererThreads(this.handle, size);
    }

    // TODO: (Ayydxn) Link to Java Renderer::Update implementation

    /**
     * Sets the max amount of time <b>(in seconds)</b> to allow repeating timers to run during each call to Renderer::Update.
     * <p>
     * The library will attempt to throttle timers if this time budget is exceeded.
     *
     * @param delay The max amount of time (in seconds) to allow repeating timers to run during each call to Renderer::Update.
     */
    public void setMaxUpdateTime(double delay)
    {
        NativeMethods.nulConfigSetMaxUpdateTime(this.handle, delay);
    }

    /**
     * Sets the alignment <b>(in bytes)</b> of the BitmapSurface when using the CPU renderer.
     * This should be a power of 2 value such as 4, 8, 16, etc.
     *
     * @param size The alignment (in bytes) of the BitmapSurface when using the CPU renderer.
     */
    public void setBitmapAlignment(int size)
    {
        NativeMethods.nulConfigSetBitmapAlignment(this.handle, size);
    }

    /**
     * Sets the quality of effects (blurs, CSS filters, SVG filters, etc.) to use when rendering.
     *
     * @param effectQuality The quality of effects to use when rendering.
     */
    public void setEffectQuality(EffectQuality effectQuality)
    {
        NativeMethods.nulConfigSetEffectQuality(this.handle, effectQuality.ordinal());
    }

    @ApiStatus.Internal
    public long getHandle()
    {
        return this.handle;
    }

    /**
     * Helper method to safely pass a Java String to a native function as a {@code ULString}.
     * <p>
     * This method handles the creation of a temporary native string, executes the
     * provided action, and ensures the native string is destroyed immediately
     * after the call returns, preventing memory leaks.
     *
     * @param value        The Java string to pass to the native layer.
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

        private static native void nulConfigSetEffectQuality(long configHandle, int effectQualityOrdinal);
    }
}
