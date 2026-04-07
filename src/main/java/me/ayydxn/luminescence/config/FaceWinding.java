package me.ayydxn.luminescence.config;

import me.ayydxn.luminescence.util.NativeEnum;

/**
 * The winding order for front-facing triangles.
 * <p>
 * (Only used when the GPU renderer is used)
 *
 * @author Ayydxn
 */
public enum FaceWinding implements NativeEnum
{
    /**
     * Clockwise Winding (Direct3D, etc.)
     */
    CLOCKWISE(0),

    /**
     * Counter-Clockwise Winding (OpenGL, etc.)
     */
    COUNTER_CLOCKWISE(1);

    private final int value;

    FaceWinding(int value)
    {
        this.value = value;
    }

    @Override
    public int getValue()
    {
        return this.value;
    }
}
