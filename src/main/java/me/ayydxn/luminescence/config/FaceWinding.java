package me.ayydxn.luminescence.config;

/**
 * The winding order for front-facing triangles.
 * <p>
 * (Only used when the GPU renderer is used)
 *
 * @author Ayydxn
 */
public enum FaceWinding
{
    /**
     * Clockwise Winding (Direct3D, etc.)
     */
    CLOCKWISE,

    /**
     * Counter-Clockwise Winding (OpenGL, etc.)
     */
    COUNTER_CLOCKWISE
}
