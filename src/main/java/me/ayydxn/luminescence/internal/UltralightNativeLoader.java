package me.ayydxn.luminescence.internal;

import org.jetbrains.annotations.Nullable;

import java.io.IOException;
import java.io.InputStream;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.StandardCopyOption;
import java.util.List;

/**
 * Handles loading of all native libraries required by Luminescence.
 * <p>
 * Ultralight's runtime binaries are loaded into the process space before the JNI bridge library so that the OS linker can resolve their imports correctly.
 * Without this, the library's temp-file extraction strategy causes dependency resolution to fail because the temp directory is not on the library search path.
 * <p>
 * Users of the library <b>must</b> call {@link #load()} before using any of the Ultralight API.
 *
 * @author Ayydxn
 */
public class UltralightNativeLoader
{
    private static final String UL_NATIVE_PATH_PROPERTY = "ultralight.native.path";
    private static final String UL_BIN_PATH_PROPERTY = "ultralight.bin.path";

    private static final List<String> ULTRALIGHT_LOAD_ORDER = List.of(
            "UltralightCore",
            "WebCore",
            "Ultralight",
            "AppCore"
    );

    private static volatile boolean isLoaded = false;

    private UltralightNativeLoader()
    {
    }

    /**
     * Loads the library using the default discovery strategy.
     * <ol>
     * <li>Checks the {@code ultralight.bin.path} system property.</li>
     * <li>If null, falls back to the OS library search path.</li>
     * </ol>
     */
    public static synchronized void load()
    {
        String binPathProperty = System.getProperty(UL_BIN_PATH_PROPERTY);

        load(binPathProperty != null ? Path.of(binPathProperty) : null);
    }

    /**
     * Loads the library using an explicit directory for the Ultralight binaries.
     *
     * @param ultralightBinaryPath The directory containing the Ultralight runtime (DLLs/SOs), or null to use the OS search path.
     */
    public static synchronized void load(@Nullable Path ultralightBinaryPath)
    {
        if (isLoaded)
            return;

        loadUltralightBinaries(ultralightBinaryPath);
        loadJNIBridge();

        isLoaded = true;
    }

    private static void loadUltralightBinaries(@Nullable Path binaryPath)
    {
        for (String libraryName : ULTRALIGHT_LOAD_ORDER)
        {
            String filename = getPlatformLibraryFilename(libraryName);

            if (binaryPath != null)
            {
                Path libraryPath = binaryPath.resolve(filename).toAbsolutePath();

                if (!Files.exists(libraryPath))
                    throw new RuntimeException("Ultralight binary not found at: " + libraryPath + "\nEnsure the directory contains the 1.4.0 SDK binaries.");

                System.load(libraryPath.toString());
            }
            else
            {
                try
                {
                    System.loadLibrary(libraryName);
                }
                catch (UnsatisfiedLinkError exception)
                {
                    throw new RuntimeException("Failed to load '" + libraryName + "' from OS search path.\n" +
                                    "Either place binaries on the PATH/LD_LIBRARY_PATH or use load(Path).", exception);
                }
            }
        }
    }

    private static void loadJNIBridge()
    {
        // 1. Property Override
        String override = System.getProperty(UL_NATIVE_PATH_PROPERTY);
        if (override != null)
        {
            System.load(Path.of(override).toAbsolutePath().toString());
            return;
        }

        // 2. Classpath Extraction
        String platform = getCurrentPlatformString();
        String libraryFilename = getPlatformLibraryFilename("LuminescenceJNI");
        String resourcePath = "/natives/" + platform + "/" + libraryFilename;

        try (InputStream inputStream = UltralightNativeLoader.class.getResourceAsStream(resourcePath))
        {
            if (inputStream != null)
            {
                Path tempFile = Files.createTempFile("LuminescenceJNI", libraryFilename);
                tempFile.toFile().deleteOnExit();

                Files.copy(inputStream, tempFile, StandardCopyOption.REPLACE_EXISTING);
                System.load(tempFile.toAbsolutePath().toString());
                return;
            }
        }
        catch (IOException exception)
        {
            throw new RuntimeException("Failed to extract JNI bridge from " + resourcePath, exception);
        }

        // 3. Final Fallback
        try
        {
            System.loadLibrary("LuminescenceJNI");
        }
        catch (UnsatisfiedLinkError exception)
        {
            throw new RuntimeException("LuminescenceJNI not found on classpath or library path.", exception);
        }
    }

    private static String getCurrentPlatformString()
    {
        String os = System.getProperty("os.name").toLowerCase();
        String arch = System.getProperty("os.arch").toLowerCase();
        boolean isArm = arch.contains("aarch64") || arch.contains("arm");

        if (os.contains("win"))
            return "windows-x64";

        if (os.contains("mac"))
            return isArm ? "macos-arm64" : "macos-x64";

        return isArm ? "linux-arm64" : "linux-x64";
    }

    private static String getPlatformLibraryFilename(String baseName)
    {
        String os = System.getProperty("os.name").toLowerCase();

        if (os.contains("win"))
            return baseName + ".dll";

        if (os.contains("mac"))
            return "lib" + baseName + ".dylib";

        return "lib" + baseName + ".so";
    }
}