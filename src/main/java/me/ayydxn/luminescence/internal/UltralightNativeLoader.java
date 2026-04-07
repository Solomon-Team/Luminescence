package me.ayydxn.luminescence.internal;

import java.io.IOException;
import java.io.InputStream;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.StandardCopyOption;

/**
 * A class which handles the loading of the necessary library files for interacting with the native C++ code.
 * <p>
 * Users of the library <b>need</b> to call this class's {@link UltralightNativeLoader#load()} function before using Ultralight in any way.
 *
 * @author Ayydxn
 */
public class UltralightNativeLoader
{
    private static volatile boolean isLoaded = false;

    private UltralightNativeLoader()
    {
    }

    /**
     * Loads the JNI binding library.
     * Search order:
     * 1. System property  -Dultralight.native.path=<dir>
     * 2. Classpath resource  /natives/<platform>/LuminescenceJNI.{dll|so|dylib}
     * 3. java.library.path  (standard JVM lookup)
     */
    public static synchronized void load()
    {
        if (isLoaded)
            return;

        String override = System.getProperty("ultralight.native.path");
        if (override != null)
        {
            System.load(Path.of(override).toAbsolutePath().toString());

            isLoaded = true;

            return;
        }

        String platform = UltralightNativeLoader.getCurrentPlatformString();
        String libraryFilename = UltralightNativeLoader.getNameLibraryFilename();
        String libraryResourceFilepath = "/natives/" + platform + "/" + libraryFilename;

        try (InputStream inputStream = UltralightNativeLoader.class.getResourceAsStream(libraryResourceFilepath))
        {
            if (inputStream != null)
            {
                Path tempFile = Files.createTempFile("LuminescenceJNI", libraryFilename);
                tempFile.toFile().deleteOnExit();

                Files.copy(inputStream, tempFile, StandardCopyOption.REPLACE_EXISTING);
                System.load(tempFile.toAbsolutePath().toString());

                isLoaded = true;

                return;
            }
        }
        catch (IOException e)
        {
            throw new RuntimeException("Failed to extract native library from classpath", e);
        }

        // Fallback — relies on java.library.path
        System.loadLibrary("LuminescenceJNI");

        isLoaded = true;
    }

    private static String getCurrentPlatformString()
    {
        String os = System.getProperty("os.name").toLowerCase();
        String arch = System.getProperty("os.arch").toLowerCase();
        boolean arm = arch.contains("aarch64") || arch.contains("arm");

        if (os.contains("win"))
            return "windows-x64";

        if (os.contains("mac"))
            return arm ? "macos-arm64" : "macos-x64";

        return arm ? "linux-arm64" : "linux-x64";
    }

    private static String getNameLibraryFilename()
    {
        String os = System.getProperty("os.name").toLowerCase();
        if (os.contains("win"))
            return "LuminescenceJNI.dll";

        if (os.contains("mac"))
            return "libLuminescenceJNI.dylib";

        return "libLuminescenceJNI.so";
    }
}
