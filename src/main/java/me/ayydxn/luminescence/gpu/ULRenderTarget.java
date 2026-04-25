package me.ayydxn.luminescence.gpu;

// TODO: (Ayydxn) Reference the equivalent Java methods when they get added

import me.ayydxn.luminescence.bitmap.BitmapFormat;
import me.ayydxn.luminescence.geometry.ULRect;

/**
 * Offscreen render target, used when rendering Views via the GPU renderer.
 * <p>
 * When a View is rendered via the GPU renderer (see ulViewIsAccelerated()), it will be rendered to
 * an offscreen render target (ulViewGetRenderTarget()) that you can display in your application.
 *
 * <h3>Displaying the Render Target</h3>
 * To display the View's render target, you should:
 * <p>
 * 1. Retrieve the underlying texture via RenderTarget::texture_id().
 * 2. Bind the texture using your custom GPUDriver implementation.
 * 3. Draw a textured quad with the provided UV coordinates (RenderTarget::uv_coords()).
 *
 * @author Ayydxn
 */
public class ULRenderTarget
{
    /** Whether this target is empty (null texture) */
    public boolean isEmpty;

    /** The viewport width (in device coordinates). */
    public int width;

    /**	The viewport height (in device coordinates). */
    public int height;

    /** The GPUDriver-specific texture ID (you should bind the texture using your custom GPUDriver implementation before drawing a quad). */
    public int textureID;

    /**
     * The texture width (in pixels).
     * <p>
     * This may be padded.
     */
    public int textureWidth;

    /**
     * The texture height (in pixels).
     * <p>
     * This may be padded.
     */
    public int textureHeight;

    /**	The pixel format of the texture. */
    public BitmapFormat bitmapFormat;

    /** UV coordinates of the texture (this is needed because the texture may be padded). */
    public ULRect uvCoords;

    /**	The GPUDriver-specific render buffer ID. */
    public int renderBufferID;
}
