package me.ayydxn.luminescence.config;

import me.ayydxn.luminescence.util.NativeEnum;

public enum FontHinting implements NativeEnum
{
    /**
     * Lighter hinting algorithm – glyphs are slightly fuzzier but better resemble their original shape.
     * <p>
     * This is achieved by snapping glyphs to the pixel grid only vertically which better preserves inter-glyph spacing.
     */
    SMOOTH(0),

    /**
     *Default hinting algorithm – offers a good balance between sharpness and shape at smaller font sizes.
     */
    NORMAL(1),

    /**
     * Strongest hinting algorithm – outputs only black/white glyphs.
     * <p>
     * The result is usually unpleasant if the underlying TTF does not contain hints for this type of rendering.
     */
    MONOCHROME(2),

    /**
     * No hinting is performed – fonts may be blurry at smaller font sizes.
     */
    NONE(3);

    private final int value;

    FontHinting(int value)
    {
        this.value = value;
    }

    @Override
    public int getValue()
    {
        return this.value;
    }
}
