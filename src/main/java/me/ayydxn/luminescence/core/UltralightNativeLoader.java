package me.ayydxn.luminescence.core;

import java.io.IOException;
import java.io.InputStream;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.StandardCopyOption;

/**
 * An internal class used by the library to load the necessary library files for interacting with the native C++ code.
 * <p>
 * Users of the library should <b>not</b> have to interact with this class in any way.
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
     * 2. Classpath resource  /natives/<platform>/luminescence_jni.{dll|so|dylib}
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

        String platform = getCurrentPlatformString();
        String libName = getNameLibraryFilename();
        String resource = "/natives/" + platform + "/" + libName;

        try (InputStream in = UltralightNativeLoader.class.getResourceAsStream(resource))
        {
            if (in != null)
            {
                Path tmp = Files.createTempFile("luminescence_jni", libName);
                tmp.toFile().deleteOnExit();

                Files.copy(in, tmp, StandardCopyOption.REPLACE_EXISTING);
                System.load(tmp.toAbsolutePath().toString());

                isLoaded = true;

                return;
            }
        }
        catch (IOException e)
        {
            throw new RuntimeException("Failed to extract native library from classpath", e);
        }

        // Fallback — relies on java.library.path
        System.loadLibrary("luminescence_jni");

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
            return "luminescence_jni.dll";

        if (os.contains("mac"))
            return "libluminescence_jni.dylib";

        return "libluminescence_jni.so";
    }
}
