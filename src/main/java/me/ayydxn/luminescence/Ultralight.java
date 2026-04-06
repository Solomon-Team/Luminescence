package me.ayydxn.luminescence;

import me.ayydxn.luminescence.core.UltralightNativeLoader;
import org.jetbrains.annotations.NotNull;

/**
 * Bindings to the Defines.h file in the C++ API or the CAPI_Defines.h file in the C API.
 *
 * @author Ayydxn
 */
public final class Ultralight
{
    static
    {
        UltralightNativeLoader.load();
    }

    private static native String nulVersionString();
    private static native int nulVersionMajor();
    private static native int nulVersionMinor();
    private static native int nulVersionPatch();
    private static native String nulWebKitVersionString();
    private static native int nulWebKitVersionMajor();
    private static native int nulWebKitVersionMinor();
    private static native int nulWebKitVersionTiny();
    private static native int nulWebKitVersionMicro();
    private static native int nulWebKitVersionNano();

    /**
     * Gets the version string of the library in MAJOR.MINOR.PATCH format.
     *
     * @return The version string of the library in MAJOR.MINOR.PATCH format.
     */
    @NotNull
    public static String ulVersionString()
    {
        return Ultralight.nulVersionString();
    }

    /**
     * Gets the numeric major version of the library.
     *
     * @return The numeric major version of the library.
     */
    public static int ulVersionMajor()
    {
        return Ultralight.nulVersionMajor();
    }

    /**
     * Gets the numeric minor version of the library.
     *
     * @return The numeric minor version of the library.
     */
    public static int ulVersionMinor()
    {
        return Ultralight.nulVersionMinor();
    }

    /**
     * Gets the numeric patch version of the library.
     *
     * @return The numeric patch version of the library.
     */
    public static int ulVersionPatch()
    {
        return Ultralight.nulVersionPatch();
    }

    @NotNull
    public static String ulWebKitVersionString()
    {
        return Ultralight.nulWebKitVersionString();
    }

    /**
     * Gets the full WebKit version string.
     *
     * @return The full WebKit version string.
     */
    public static int ulWebKitVersionMajor()
    {
        return Ultralight.nulWebKitVersionMajor();
    }

    public static int ulWebKitVersionMinor()
    {
        return Ultralight.nulWebKitVersionMinor();
    }

    public static int ulWebKitVersionTiny()
    {
        return Ultralight.nulWebKitVersionTiny();
    }

    public static int ulWebKitVersionMicro()
    {
        return Ultralight.nulWebKitVersionMicro();
    }

    public static int ulWebKitVersionNano()
    {
        return Ultralight.nulWebKitVersionNano();
    }
}
