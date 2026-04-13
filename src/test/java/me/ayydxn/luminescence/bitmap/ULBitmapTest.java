package me.ayydxn.luminescence.bitmap;

import me.ayydxn.luminescence.BaseLuminescenceTest;
import me.ayydxn.luminescence.platform.ULLogger;
import me.ayydxn.luminescence.platform.ULPlatform;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;

import java.nio.ByteBuffer;
import java.nio.file.Files;
import java.nio.file.Path;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertTrue;

public class ULBitmapTest extends BaseLuminescenceTest
{
    @BeforeAll
    static void setupPlatformHandlers()
    {
        ULPlatform.setLogger((logLevel, message) ->
        {
            if (logLevel != ULLogger.Level.INFO)
                System.out.printf("[%s] %s\n", logLevel.name(), message);
        });
    }

    @Test
    @DisplayName("Verify Pixel Data Integrity & Channel Swapping")
    void testPixelDataIntegrity()
    {
        int width = 2;
        int height = 2;
        ByteBuffer pixels = ByteBuffer.allocateDirect(width * height * 4);

        // Fill with Blue (B=255, G=0, R=0, A=255)
        while (pixels.hasRemaining())
        {
            pixels.put((byte) 255);
            pixels.put((byte) 0);
            pixels.put((byte) 0);
            pixels.put((byte) 255);
        }

        pixels.flip();

        ULBitmap bitmap = ULBitmap.createFromPixels(width, height, BitmapFormat.BGRA8_UNORM_SRGB, pixels, false);

        // Pattern 1: Using the AutoCloseable acquirePixelLock() pattern
        try (ULBitmap.LockedPixels lockedPixels = bitmap.acquirePixelLock())
        {
            ByteBuffer pixelBuffer = lockedPixels.pixels();
            assertEquals(255, Byte.toUnsignedInt(pixelBuffer.get(0)), "Original Blue should be 255");
        }

        bitmap.swapRedBlueChannels();

        // Pattern 2: Using manual lockPixels() / unlockPixels() pattern
        ByteBuffer result = bitmap.lockPixels();
        try
        {
            assertEquals(0, Byte.toUnsignedInt(result.get(0)), "New Blue (from old Red) should be 0");
            assertEquals(255, Byte.toUnsignedInt(result.get(2)), "New Red (from old Blue) should be 255");
        }
        finally
        {
            bitmap.unlockPixels();
        }

        bitmap.destroy();
    }

    @Test
    @DisplayName("Generate and Export Mesh Gradient to PNG")
    void testWritePNG()
    {
        int width = 1024;
        int height = 1024;

        ULBitmap bitmap = ULBitmap.create(width, height, BitmapFormat.BGRA8_UNORM_SRGB);

        try (ULBitmap.LockedPixels lockedPixels = bitmap.acquirePixelLock())
        {
            // (Ayydxn) Yes, this is AI generated. I'm not that good at math.
            for (int y = 0; y < height; y++)
            {
                for (int x = 0; x < width; x++)
                {
                    // These coefficients (0.005, 0.003) control the "frequency" or tightness of the waves.
                    // Lower values make softer, larger color pools.
                    double waveX = Math.sin(x * 0.005);
                    double waveY = Math.sin(y * 0.003);

                    // Combine waves and normalize to 0.0 -> 1.0 range
                    double mixFactor = (waveX + waveY + 2.0) / 4.0;

                    // --- Define the Color Palette (Modern Dark Theme) ---

                    // Blue: Base "Dark Navy" (scales from 15 to 60)
                    byte blue = (byte) (15 + (int) (mixFactor * 45));

                    // Green: Very Low (scales from 5 to 25 for deep saturation)
                    byte green = (byte) (5 + (int) (mixFactor * 20));

                    // Red: Accent "Dark Violet" (scales from 10 to 80)
                    // Using a different frequency for Red makes the Violet accent swirl independently.
                    double accentWave = (Math.sin((x + y) * 0.004) + 1.0) / 2.0;
                    byte red = (byte) (10 + (int) (accentWave * 70));

                    // A: Alpha (Opaque)
                    byte alpha = (byte) 255;

                    // BGRA Order
                    lockedPixels.pixels().put(blue);
                    lockedPixels.pixels().put(green);
                    lockedPixels.pixels().put(red);
                    lockedPixels.pixels().put(alpha);
                }
            }
        }

        Path pngPath = Path.of("ulbitmap_test_output.png").toAbsolutePath();
        boolean wasPNGWriteSuccessful = bitmap.writePNG(pngPath);

        assertTrue(wasPNGWriteSuccessful, "Native ulBitmapWritePNG returned false. Check if the directory is writable.");
        assertTrue(Files.exists(pngPath), "File was not found on disk at: " + pngPath);

        System.out.println("====================================================");
        System.out.println("PNG Exported To: " + pngPath);
        System.out.println("====================================================");

        bitmap.destroy();
    }

    @Test
    @DisplayName("Verify Deep Copy Independence")
    void testCreateFromCopy()
    {
        int width = 128;
        int height = 128;

        // 1. Create original bitmap and fill with a unique pattern (e.g., solid Green)
        ULBitmap original = ULBitmap.create(width, height, BitmapFormat.BGRA8_UNORM_SRGB);
        try (var lockedPixels = original.acquirePixelLock())
        {
            ByteBuffer pixels = lockedPixels.pixels();
            while (pixels.hasRemaining())
            {
                pixels.put((byte) 0);   // B
                pixels.put((byte) 255); // G
                pixels.put((byte) 0);   // R
                pixels.put((byte) 255); // A
            }
        }

        // 2. Perform the native copy
        ULBitmap copy = ULBitmap.createFromCopy(original);

        // 3. Verify metadata match
        assertEquals(original.getWidth(), copy.getWidth(), "Width mismatch");
        assertEquals(original.getHeight(), copy.getHeight(), "Height mismatch");
        assertEquals(original.getFormat(), copy.getFormat(), "Format mismatch");

        // 4. Verify Content Equality
        try (var originalLock = original.acquirePixelLock(); var copyLock = copy.acquirePixelLock())
        {

            ByteBuffer originalBuf = originalLock.pixels();
            ByteBuffer copyBuf = copyLock.pixels();

            // Sample the first pixel
            assertEquals(originalBuf.get(0), copyBuf.get(0), "Pixel data mismatch (Blue)");
            assertEquals(originalBuf.get(1), copyBuf.get(1), "Pixel data mismatch (Green)");
        }

        // 5. Verify Memory Independence (Deep Copy check)
        // Modify the copy—the original should remain Green
        try (var copyLock = copy.acquirePixelLock())
        {
            ByteBuffer copyBuf = copyLock.pixels();
            copyBuf.put(0, (byte) 255); // Turn the first pixel's Blue to 255
        }

        try (var originalLock = original.acquirePixelLock())
        {
            ByteBuffer originalBuf = originalLock.pixels();
            assertEquals(0, Byte.toUnsignedInt(originalBuf.get(0)), "Original was modified by copy! (Shallow copy detected)");
        }

        // Cleanup
        original.destroy(); // Assuming you have a destroy/close method
        copy.destroy();
    }
}
