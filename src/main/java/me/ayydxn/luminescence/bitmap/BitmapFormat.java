package me.ayydxn.luminescence.bitmap;

// TODO: (Ayydxn) Link to the corresponding Java Bitmap class when added.
/**
 * The various Bitmap formats.
 *
 * @author Ayydxn
 */
public enum BitmapFormat
{
    /**
     * Alpha channel only, 8-bits per pixel.
     * <p>
     * Encoding: 8-bits per channel, unsigned normalized.
     * <p>?
     * Color-space: Linear (no gamma), alpha-coverage only.
     */
    A8_UNORM(1),

    /**
     * Blue Green Red Alpha channels, 32-bits per pixel.
     * <p>
     * Encoding: 8-bits per channel, unsigned normalized.
     * <p>
     * Color-space: sRGB gamma with premultiplied linear alpha channel.
     */
    BGRA8_UNORM_SRGB(4);

    private final int bytesPerPixel;

    BitmapFormat(int bytesPerPixel)
    {
        this.bytesPerPixel = bytesPerPixel;
    }

    public static BitmapFormat fromNativeValue(int nativeValue)
    {
        return switch(nativeValue)
        {
            case 0 -> A8_UNORM;
            case 1 -> BGRA8_UNORM_SRGB;
            default -> throw new IllegalArgumentException("Unexpected bitmap format native value: " + nativeValue);
        };
    }

    public int getBytesPerPixel()
    {
        return this.bytesPerPixel;
    }
}
