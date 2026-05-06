package me.ayydxn.luminescence.demo;

import me.ayydxn.luminescence.bitmap.BitmapFormat;
import me.ayydxn.luminescence.bitmap.ULBitmap;
import me.ayydxn.luminescence.config.ULConfig;
import me.ayydxn.luminescence.gpu.*;
import me.ayydxn.luminescence.platform.ULGPUDriver;
import me.ayydxn.luminescence.platform.ULPlatform;
import me.ayydxn.luminescence.view.ULView;
import me.ayydxn.luminescence.view.ULViewConfig;
import org.apache.commons.io.FileUtils;
import org.lwjgl.glfw.GLFWVidMode;

import java.io.File;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.charset.StandardCharsets;
import java.util.HashMap;
import java.util.Map;
import java.util.Objects;

import static org.lwjgl.glfw.GLFW.*;
import static org.lwjgl.opengl.GL11.*;
import static org.lwjgl.opengl.GL12.GL_BGRA;
import static org.lwjgl.opengl.GL12.GL_CLAMP_TO_EDGE;
import static org.lwjgl.opengl.GL13.GL_TEXTURE0;
import static org.lwjgl.opengl.GL13.glActiveTexture;
import static org.lwjgl.opengl.GL14.GL_FUNC_ADD;
import static org.lwjgl.opengl.GL14.glBlendEquation;
import static org.lwjgl.opengl.GL15.*;
import static org.lwjgl.opengl.GL20.*;
import static org.lwjgl.opengl.GL30.*;
import static org.lwjgl.opengl.GL31.*;
import static org.lwjgl.opengl.GL45.*;

/**
 * GPU-accelerated Luminescence demo using Ultralight's ULGPUDriver API.
 *
 * <p>Faithfully modeled on Ultralight's own {@code GPUDriverGL} reference implementation.
 * Key design decisions that match the reference:</p>
 * <ul>
 *   <li>Uniforms are uploaded via a <b>Uniform Buffer Object</b> bound to binding point 0,
 *       matching the {@code uniform block type_Uniforms} layout in the SPIRV-Cross GLSL shaders.</li>
 *   <li>Textures use <b>mutable</b> {@code glTexImage2D} storage so they can be resized in
 *       {@code updateTexture} without needing to recreate the texture object.</li>
 *   <li>Blend uses premultiplied-alpha ({@code GL_ONE / GL_ONE_MINUS_SRC_ALPHA}) when {@code enableBlend} is set.
 *       {@code ULGPUState} does not expose per-draw blend factors or equations.</li>
 *   <li>Scissor rect Y is flipped from Ultralight's top-left origin to OpenGL's
 *       bottom-left origin: {@code scissorY = viewportHeight - scissorRect.bottom}.</li>
 *   <li>{@code GL_UNPACK_ROW_LENGTH} is set from {@code bitmap.rowBytes() / bytesPerPixel}
 *       before every pixel upload to handle row-padded bitmaps correctly.</li>
 *   <li>A8 textures get a swizzle mask ({@code R=0, G=0, B=0, A=RED}) so glyph coverage
 *       lands in {@code .a} where the fill shader's {@code fillGlyph()} reads it.</li>
 * </ul>
 */
public class LWJGLGPUDemoApp extends LWJGLDemo
{
    private final OpenGLGPUDriver openGLGPUDriver;

    private ULView demoView;
    private String demoHTML;
    private BlitShader blitShader;
    private FullscreenQuad fullscreenQuad;

    public LWJGLGPUDemoApp()
    {
        super("Luminescence GPU Demo", 1600, 900);
        this.openGLGPUDriver = new OpenGLGPUDriver();
    }

    @Override
    protected void configureUltralight(ULConfig config)
    {
        try
        {
            var resourcesFolderURL = LWJGLCPUDemoApp.class.getClassLoader().getResource("./ultralight");
            Objects.requireNonNull(resourcesFolderURL, "ultralight resources folder not found on classpath");

            GLFWVidMode videoMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
            Objects.requireNonNull(videoMode);

            config.setResourcePathPrefix(new File(resourcesFolderURL.toURI()) + "/");
            config.setScrollTimerDelay(1.0d / videoMode.refreshRate());
            config.setAnimationTimerDelay(1.0d / videoMode.refreshRate());

            File demoHTMLFile = new File(resourcesFolderURL.toURI().resolve("ultralight/html/demo.html"));
            this.demoHTML = FileUtils.readFileToString(demoHTMLFile, StandardCharsets.UTF_8);
        }
        catch (Exception e)
        {
            e.printStackTrace();
        }

        ULPlatform.setGPUDriver(this.openGLGPUDriver);
    }

    @Override
    protected void onInit() throws Exception
    {
        try (ULViewConfig viewConfig = new ULViewConfig())
        {
            viewConfig.isAccelerated(true);
            this.demoView = new ULView(this.renderer, width, height, viewConfig, null);
            this.demoView.loadHTML(this.demoHTML);
        }

        var resourcesFolderURL = LWJGLCPUDemoApp.class.getClassLoader().getResource("./ultralight");
        Objects.requireNonNull(resourcesFolderURL, "ultralight resources folder not found on classpath");

        String fillVert = readShader(resourcesFolderURL, "UltralightFillVertex.glsl");
        String fillFrag = readShader(resourcesFolderURL, "UltralightFillFragment.glsl");
        String pathVert = readShader(resourcesFolderURL, "UltralightPathVertex.glsl");
        String pathFrag = readShader(resourcesFolderURL, "UltralightPathFragment.glsl");

        this.openGLGPUDriver.initShaders(fillVert, fillFrag, pathVert, pathFrag);

        this.blitShader = new BlitShader();
        this.fullscreenQuad = new FullscreenQuad();

        glfwSetWindowSizeCallback(this.windowHandle, (window, width, height) -> this.demoView.resize(800, 600));
    }

    private static String readShader(java.net.URL base, String filename) throws Exception
    {
        File f = new File(base.toURI().resolve("ultralight/shaders/" + filename));
        return FileUtils.readFileToString(f, StandardCharsets.UTF_8);
    }

    @Override
    protected void onUpdate()
    {
        if (demoView != null)
            demoView.setNeedsPaint(true);
    }

    @Override
    protected void onRender()
    {
        this.openGLGPUDriver.executeCommands();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, width, height);

        ULRenderTarget renderTarget = demoView.getRenderTarget();
        if (renderTarget == null || renderTarget.isEmpty)
            return;

        Integer glTexId = openGLGPUDriver.getGLTextureId(renderTarget.textureID);
        if (glTexId == null)
            return;

        // uvCoords origin is top-left. The blit quad has v=0 at bottom, so swap v0/v1.
        float u0 = renderTarget.uvCoords.left;
        float v0 = renderTarget.uvCoords.bottom;
        float u1 = renderTarget.uvCoords.right;
        float v1 = renderTarget.uvCoords.top;

        glDisable(GL_SCISSOR_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        glBlendEquation(GL_FUNC_ADD);

        blitShader.bind();
        blitShader.setUVRect(u0, v0, u1, v1);
        glBindTextureUnit(0, glTexId);
        fullscreenQuad.draw();

        glDisable(GL_BLEND);
        glUseProgram(0);
    }

    @Override
    protected void onShutdown()
    {
        if (demoView != null) demoView.destroy();
        if (blitShader != null) blitShader.destroy();
        if (fullscreenQuad != null) fullscreenQuad.destroy();
        openGLGPUDriver.destroyShaders();
    }

    /*===========================================================*/
    /*  OpenGLGPUDriver                                          */
    /*===========================================================*/

    /**
     * OpenGL GPU driver modelled directly on Ultralight's {@code GPUDriverGL}.
     *
     * <h3>UBO layout — matches {@code struct Uniforms} in the reference driver</h3>
     * <pre>
     *  Offset   Field          GLSL type   Size (bytes)
     *  ------   -----          ---------   ------------
     *     0     State          vec4           16
     *    16     Transform      mat4          64   (row-major)
     *    80     Integer4       ivec4[2]      32
     *   112     Scalar4        vec4[2]       32
     *   144     Vector         vec4[8]      128
     *   272     ClipData       ivec4         16
     *   288     Clip           mat4[8]      512   (row-major)
     *   800     total
     * </pre>
     */
    private static final class OpenGLGPUDriver implements ULGPUDriver
    {
        // UBO size in bytes — matches sizeof(Uniforms) in GPUDriverGL.h
        private static final int UBO_SIZE = 800;

        // Offsets within the UBO (byte offsets matching std140 layout)
        private static final int UBO_OFF_STATE = 0;
        private static final int UBO_OFF_TRANSFORM = 16;
        private static final int UBO_OFF_INTEGER4 = 80;
        private static final int UBO_OFF_SCALAR4 = 112;
        private static final int UBO_OFF_VECTOR = 144;
        private static final int UBO_OFF_CLIPDATA = 272;
        private static final int UBO_OFF_CLIP = 288;

        /*-- ID counters -------------------------------------------------*/
        private int nextTextureId = 1;
        private int nextRenderBufferId = 1;
        private int nextGeometryId = 1;

        /*-- Resource maps -----------------------------------------------*/
        private final Map<Integer, Integer> textures = new HashMap<>();
        private final Map<Integer, RenderBufferData> renderBuffers = new HashMap<>();
        private final Map<Integer, GeometryData> geometries = new HashMap<>();

        /*-- UBO ---------------------------------------------------------*/
        private int uboId = 0;

        /*-- Command list ------------------------------------------------*/
        private ULCommand[] currentCommandList = new ULCommand[0];

        /*-- Shader programs ---------------------------------------------*/
        private int fillProgramId = 0;
        private int pathProgramId = 0;

        /*-- Reusable staging buffer for UBO updates ---------------------*/
        private final ByteBuffer uboStaging = ByteBuffer
                .allocateDirect(UBO_SIZE)
                .order(ByteOrder.nativeOrder());

        /*----------------------------------------------------------------*/
        /* Shader initialisation                                           */
        /*----------------------------------------------------------------*/

        void initShaders(String fillVert, String fillFrag, String pathVert, String pathFrag)
        {
            // Create the Uniform Buffer Object used by both programs.
            int[] ids = new int[1];
            glGenBuffers(ids);
            uboId = ids[0];
            glBindBuffer(GL_UNIFORM_BUFFER, uboId);
            glBufferData(GL_UNIFORM_BUFFER, UBO_SIZE, GL_DYNAMIC_DRAW);
            glBindBuffer(GL_UNIFORM_BUFFER, 0);

            fillProgramId = linkProgram("Fill", fillVert, fillFrag);
            pathProgramId = linkProgram("Path", pathVert, pathFrag);

            // Bind the "type_Uniforms" uniform block in each program to binding point 0.
            bindUniformBlock(fillProgramId, "type_Uniforms", 0);
            bindUniformBlock(pathProgramId, "type_Uniforms", 0);

            // Bind texture samplers to fixed units (these are regular uniforms outside the block).
            setSamplerUnit(fillProgramId, "SPIRV_Cross_CombinedTexture0Sampler0", 0);
            setSamplerUnit(fillProgramId, "SPIRV_Cross_CombinedTexture1Sampler0", 1);
            setSamplerUnit(fillProgramId, "SPIRV_Cross_CombinedTexture2Sampler0", 2);
        }

        private static void bindUniformBlock(int prog, String blockName, int bindingPoint)
        {
            int idx = glGetUniformBlockIndex(prog, blockName);
            if (idx != GL_INVALID_INDEX)
                glUniformBlockBinding(prog, idx, bindingPoint);
        }

        private static void setSamplerUnit(int prog, String name, int unit)
        {
            int loc = glGetUniformLocation(prog, name);
            if (loc >= 0)
            {
                glUseProgram(prog);
                glUniform1i(loc, unit);
                glUseProgram(0);
            }
        }

        void destroyShaders()
        {
            if (uboId != 0)
            {
                glDeleteBuffers(uboId);
                uboId = 0;
            }
            if (fillProgramId != 0)
            {
                glDeleteProgram(fillProgramId);
                fillProgramId = 0;
            }
            if (pathProgramId != 0)
            {
                glDeleteProgram(pathProgramId);
                pathProgramId = 0;
            }
        }

        private static int linkProgram(String name, String vertSrc, String fragSrc)
        {
            int vert = compileShader(GL_VERTEX_SHADER, name, vertSrc);
            int frag = compileShader(GL_FRAGMENT_SHADER, name, fragSrc);

            int prog = glCreateProgram();
            glAttachShader(prog, vert);
            glAttachShader(prog, frag);
            glLinkProgram(prog);
            glDeleteShader(vert);
            glDeleteShader(frag);

            if (glGetProgrami(prog, GL_LINK_STATUS) == GL_FALSE)
            {
                String log = glGetProgramInfoLog(prog);
                glDeleteProgram(prog);
                throw new RuntimeException(name + " shader link failed:\n" + log);
            }
            return prog;
        }

        private static int compileShader(int type, String programName, String source)
        {
            int id = glCreateShader(type);
            glShaderSource(id, source);
            glCompileShader(id);

            if (glGetShaderi(id, GL_COMPILE_STATUS) == GL_FALSE)
            {
                String log = glGetShaderInfoLog(id);
                String typeName = (type == GL_VERTEX_SHADER) ? "vertex" : "fragment";
                glDeleteShader(id);
                throw new RuntimeException(programName + " " + typeName + " shader compile failed:\n" + log);
            }
            return id;
        }

        /*----------------------------------------------------------------*/
        /* UBO upload — matches UpdateUniforms() in the reference driver   */
        /*----------------------------------------------------------------*/

        /**
         * Build the Uniforms struct in {@link #uboStaging} and upload it to the GPU.
         *
         * <p>Mirrors {@code UpdateUniforms()} in the reference driver. The Transform
         * field in the UBO is the product of an orthographic projection matrix and
         * {@code state.transform} (matching {@code ApplyProjection()} in the reference).
         * Both matrices are column-major, matching GLSL mat4 memory order.</p>
         */
        private void uploadUniforms(ULGPUState state)
        {
            uboStaging.clear();

            // State: [time, screenWidth, screenHeight, screenScale]
            uboStaging.position(UBO_OFF_STATE);
            uboStaging.putFloat(0f);
            uboStaging.putFloat(state.viewportWidth);
            uboStaging.putFloat(state.viewportHeight);
            uboStaging.putFloat(1f);

            // Transform = ortho(viewport) * state.transform  (mirrors ApplyProjection).
            // Both are column-major; result is stored column-major for GLSL mat4.
            uboStaging.position(UBO_OFF_TRANSFORM);
            float[] t = (state.transform != null && state.transform.length == 16)
                    ? state.transform : IDENTITY;
            float[] mvp = multiplyOrtho(t, state.viewportWidth, state.viewportHeight);
            for (float f : mvp) uboStaging.putFloat(f);

            // Integer4 (8 ints packed as ivec4[2])
            // ULGPUState does not expose integer uniforms -- zero the block.
            uboStaging.position(UBO_OFF_INTEGER4);
            for (int i = 0; i < 8; i++) uboStaging.putInt(0);

            // Scalar4 (8 floats packed as vec4[2]).
            // Always write all 8 slots; default to zero for any absent entries.
            uboStaging.position(UBO_OFF_SCALAR4);
            for (int i = 0; i < 8; i++)
                uboStaging.putFloat((state.uniformScalar != null && i < state.uniformScalar.length)
                        ? state.uniformScalar[i] : 0f);

            // Vector (8 × vec4 = 32 floats).
            // Always write all 32 slots; default to zero for any absent entries.
            uboStaging.position(UBO_OFF_VECTOR);
            for (int i = 0; i < 32; i++)
                uboStaging.putFloat((state.uniformVector != null && i < state.uniformVector.length)
                        ? state.uniformVector[i] : 0f);

            // ClipData (ivec4: x = clip count)
            uboStaging.position(UBO_OFF_CLIPDATA);
            uboStaging.putInt(state.clipSize);
            uboStaging.putInt(0);
            uboStaging.putInt(0);
            uboStaging.putInt(0);

            // Clip (up to 8 × mat4 = up to 128 floats).
            // Always write all 128 slots; zero out any clips beyond clipSize.
            uboStaging.position(UBO_OFF_CLIP);
            for (int i = 0; i < 128; i++)
            {
                boolean hasData = (state.clip != null)
                        && (i / 16 < state.clipSize)
                        && (i < state.clip.length);
                uboStaging.putFloat(hasData ? state.clip[i] : 0f);
            }

            uboStaging.rewind();

            glBindBuffer(GL_UNIFORM_BUFFER, uboId);
            glBufferSubData(GL_UNIFORM_BUFFER, 0, uboStaging);
            glBindBufferBase(GL_UNIFORM_BUFFER, 0, uboId);
        }

        // Column-major identity matrix.
        private static final float[] IDENTITY = {
                1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1
        };

        private static void putIdentity(ByteBuffer buf)
        {
            for (float f : IDENTITY) buf.putFloat(f);
        }

        /**
         * Mirrors {@code ApplyProjection()} from the reference driver.
         *
         * Builds a column-major orthographic projection for a viewport of
         * {@code w × h} pixels (origin top-left, +Y down, matching Ultralight/CSS),
         * then computes {@code ortho × t} via a full 4×4 column-major multiply.
         *
         * Ortho matrix (column-major, maps pixel-space to GL NDC with Y-flip):
         * <pre>
         *   col 0: [ 2/w,  0,  0, 0 ]
         *   col 1: [   0,-2/h, 0, 0 ]
         *   col 2: [   0,  0, -1, 0 ]
         *   col 3: [  -1,  1,  0, 1 ]
         * </pre>
         */
        private static float[] multiplyOrtho(float[] t, int w, int h)
        {
            float rw =  2.0f / w;
            float rh = -2.0f / h;

            // Ortho matrix in column-major order (index = col*4 + row).
            // col 0        col 1        col 2        col 3
            float[] ortho = {
                rw,  0,  0, 0,   // col 0
                 0, rh,  0, 0,   // col 1
                 0,  0, -1, 0,   // col 2
                -1,  1,  0, 1    // col 3
            };

            // result = ortho * t, both column-major.
            float[] r = new float[16];
            for (int col = 0; col < 4; col++)
            {
                for (int row = 0; row < 4; row++)
                {
                    float sum = 0f;
                    for (int k = 0; k < 4; k++)
                        sum += ortho[k * 4 + row] * t[col * 4 + k];
                    r[col * 4 + row] = sum;
                }
            }
            return r;
        }

        /*----------------------------------------------------------------*/
        /* Public helpers                                                  */
        /*----------------------------------------------------------------*/

        Integer getGLTextureId(int ultralightTextureId)
        {
            return textures.get(ultralightTextureId);
        }

        /*----------------------------------------------------------------*/
        /* ULGPUDriver — synchronise                                       */
        /*----------------------------------------------------------------*/

        @Override
        public void beginSynchronize()
        {
        }

        @Override
        public void endSynchronize()
        {
        }

        /*----------------------------------------------------------------*/
        /* Texture management — mirrors CreateTexture / UpdateTexture      */
        /*----------------------------------------------------------------*/

        @Override
        public int nextTextureId()
        {
            return nextTextureId++;
        }

        @Override
        public void createTexture(int textureId, ULBitmap bitmap)
        {
            int[] ids = new int[1];
            glGenTextures(ids);
            int glId = ids[0];

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, glId);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

            if (bitmap.isEmpty())
            {
                // FBO backing texture — allocate empty mutable storage.
                // Use GL_RGBA (not GL_BGRA) as the external format when data is null;
                // GL_BGRA with a null pointer is rejected as GL_INVALID_OPERATION on
                // some strict drivers because BGRA is only valid for actual pixel uploads.
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,
                        bitmap.getWidth(), bitmap.getHeight(), 0,
                        GL_RGBA, GL_UNSIGNED_BYTE, (ByteBuffer) null);
            }
            else
            {
                BitmapFormat fmt = bitmap.getFormat();
                glPixelStorei(GL_UNPACK_ROW_LENGTH, bitmap.getRowBytes() / fmt.getBytesPerPixel());

                ByteBuffer pixels = bitmap.lockPixels();
                ByteBuffer copy = copyPixels(pixels);
                bitmap.unlockPixels();

                if (fmt == BitmapFormat.A8_UNORM)
                {
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8,
                            bitmap.getWidth(), bitmap.getHeight(), 0,
                            GL_RED, GL_UNSIGNED_BYTE, copy);
                    // Swizzle: RED → alpha so fillGlyph() reads .a correctly.
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_R, GL_ZERO);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_ZERO);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_ZERO);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_A, GL_RED);
                }
                else
                {
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,
                            bitmap.getWidth(), bitmap.getHeight(), 0,
                            GL_BGRA, GL_UNSIGNED_BYTE, copy);
                }

                glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
            }

            glGenerateMipmap(GL_TEXTURE_2D);
            textures.put(textureId, glId);
        }

        @Override
        public void updateTexture(int textureId, ULBitmap bitmap)
        {
            Integer glId = textures.get(textureId);
            if (glId == null) return;

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, glId);
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

            if (!bitmap.isEmpty())
            {
                BitmapFormat fmt = bitmap.getFormat();
                glPixelStorei(GL_UNPACK_ROW_LENGTH, bitmap.getRowBytes() / fmt.getBytesPerPixel());

                ByteBuffer pixels = bitmap.lockPixels();
                ByteBuffer copy = copyPixels(pixels);
                bitmap.unlockPixels();

                // glTexImage2D (mutable storage) handles resizes automatically —
                // no need to track dimensions or recreate the texture object.
                if (fmt == BitmapFormat.A8_UNORM)
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8,
                            bitmap.getWidth(), bitmap.getHeight(), 0,
                            GL_RED, GL_UNSIGNED_BYTE, copy);
                else
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,
                            bitmap.getWidth(), bitmap.getHeight(), 0,
                            GL_BGRA, GL_UNSIGNED_BYTE, copy);

                glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
                glGenerateMipmap(GL_TEXTURE_2D);
            }
        }

        @Override
        public void destroyTexture(int textureId)
        {
            Integer glId = textures.remove(textureId);
            if (glId != null) glDeleteTextures(glId);
        }

        /**
         * Copy a Ultralight-managed DirectByteBuffer into a Java-owned buffer.
         */
        private static ByteBuffer copyPixels(ByteBuffer src)
        {
            ByteBuffer copy = ByteBuffer.allocateDirect(src.remaining())
                    .order(ByteOrder.nativeOrder());
            copy.put(src).flip();
            return copy;
        }

        /*----------------------------------------------------------------*/
        /* Render-buffer management                                        */
        /*----------------------------------------------------------------*/

        @Override
        public int nextRenderBufferId()
        {
            return nextRenderBufferId++;
        }

        @Override
        public void createRenderBuffer(int renderBufferId, ULRenderBuffer buffer)
        {
            // Store the texture association. The FBO is created lazily on first draw
            // (matching the reference driver's CreateFBOIfNeededForActiveContext pattern).
            renderBuffers.put(renderBufferId,
                    new RenderBufferData(-1, buffer.textureID()));
        }

        @Override
        public void destroyRenderBuffer(int renderBufferId)
        {
            RenderBufferData data = renderBuffers.remove(renderBufferId);
            if (data != null && data.fboId >= 0)
                glDeleteFramebuffers(data.fboId);
        }

        /**
         * Bind the FBO for a render buffer, creating it lazily if needed.
         */
        private void bindRenderBuffer(int renderBufferId)
        {
            if (renderBufferId == 0)
            {
                glBindFramebuffer(GL_FRAMEBUFFER, 0);
                return;
            }

            RenderBufferData data = renderBuffers.get(renderBufferId);
            if (data == null) return;

            if (data.fboId < 0)
            {
                // Lazily create the FBO now that we have a GL context.
                Integer texGLId = textures.get(data.textureId);
                if (texGLId == null)
                {
                    System.err.println("[LuminescenceGPU] bindRenderBuffer: texture "
                            + data.textureId + " not found");
                    return;
                }

                int fbo = glGenFramebuffers();
                glBindFramebuffer(GL_FRAMEBUFFER, fbo);
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                        GL_TEXTURE_2D, texGLId, 0);

                int[] drawBufs = {GL_COLOR_ATTACHMENT0};
                glDrawBuffers(drawBufs);

                int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
                if (status != GL_FRAMEBUFFER_COMPLETE)
                    System.err.println("[LuminescenceGPU] FBO incomplete for renderBuffer "
                            + renderBufferId + " — 0x" + Integer.toHexString(status));

                // Replace the entry with the real FBO id.
                renderBuffers.put(renderBufferId,
                        new RenderBufferData(fbo, data.textureId));
            }
            else
            {
                glBindFramebuffer(GL_FRAMEBUFFER, data.fboId);
            }
        }

        /*----------------------------------------------------------------*/
        /* Geometry management                                             */
        /*----------------------------------------------------------------*/

        @Override
        public int nextGeometryId()
        {
            return nextGeometryId++;
        }

        @Override
        public void createGeometry(int geometryId, ULVertexBuffer vertices, ULIndexBuffer indices)
        {
            int vbo = glGenBuffers();
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, vertices.getData(), GL_DYNAMIC_DRAW);

            int ebo = glGenBuffers();
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
            // Use DYNAMIC_DRAW: indices can be replaced on every updateGeometry call.
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.data(), GL_DYNAMIC_DRAW);

            geometries.put(geometryId, new GeometryData(-1, vbo, ebo,
                    vertices.getFormat() == ULVertexBuffer.Format.QUAD));
        }

        @Override
        public void updateGeometry(int geometryId, ULVertexBuffer vertices, ULIndexBuffer indices)
        {
            GeometryData data = geometries.get(geometryId);
            if (data == null) return;

            glBindBuffer(GL_ARRAY_BUFFER, data.vbo);
            glBufferData(GL_ARRAY_BUFFER, vertices.getData(), GL_DYNAMIC_DRAW);

            ByteBuffer idxBuf = indices.data();
            if (idxBuf != null && idxBuf.hasRemaining())
            {
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data.ebo);
                // glBufferSubData avoids orphaning the existing allocation when the
                // size has not changed (which is the common case for Ultralight geometry).
                glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, idxBuf);
            }
        }

        @Override
        public void destroyGeometry(int geometryId)
        {
            GeometryData data = geometries.remove(geometryId);
            if (data == null) return;

            if (data.vao >= 0) glDeleteVertexArrays(data.vao);
            glDeleteBuffers(data.vbo);
            glDeleteBuffers(data.ebo);
        }

        /**
         * Create and bind a VAO for geometry, lazily (mirrors CreateVAOIfNeededForActiveContext).
         * Returns the VAO id.
         */
        private int getOrCreateVAO(int geometryId)
        {
            GeometryData data = geometries.get(geometryId);
            if (data == null) return -1;

            if (data.vao >= 0)
                return data.vao;

            int vao = glGenVertexArrays();
            glBindVertexArray(vao);
            glBindBuffer(GL_ARRAY_BUFFER, data.vbo);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data.ebo);

            if (data.isQuad)
                setupQuadAttribs();
            else
                setupPathAttribs();

            glBindVertexArray(0);

            // Store updated entry with the real VAO id.
            geometries.put(geometryId,
                    new GeometryData(vao, data.vbo, data.ebo, data.isQuad));
            return vao;
        }

        /**
         * _2f_4ub_2f_2f_28f — stride 140 bytes. Matches reference glVertexAttribPointer calls.
         */
        private static void setupQuadAttribs()
        {
            final int S = 140;
            glVertexAttribPointer(0, 2, GL_FLOAT, false, S, 0);
            glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, true, S, 8);
            glVertexAttribPointer(2, 2, GL_FLOAT, false, S, 12);
            glVertexAttribPointer(3, 2, GL_FLOAT, false, S, 20);
            glVertexAttribPointer(4, 4, GL_FLOAT, false, S, 28);
            glVertexAttribPointer(5, 4, GL_FLOAT, false, S, 44);
            glVertexAttribPointer(6, 4, GL_FLOAT, false, S, 60);
            glVertexAttribPointer(7, 4, GL_FLOAT, false, S, 76);
            glVertexAttribPointer(8, 4, GL_FLOAT, false, S, 92);
            glVertexAttribPointer(9, 4, GL_FLOAT, false, S, 108);
            glVertexAttribPointer(10, 4, GL_FLOAT, false, S, 124);
            for (int i = 0; i <= 10; i++) glEnableVertexAttribArray(i);
        }

        /**
         * _2f_4ub_2f — stride 20 bytes.
         */
        private static void setupPathAttribs()
        {
            final int S = 20;
            glVertexAttribPointer(0, 2, GL_FLOAT, false, S, 0);
            glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, true, S, 8);
            glVertexAttribPointer(2, 2, GL_FLOAT, false, S, 12);
            for (int i = 0; i <= 2; i++) glEnableVertexAttribArray(i);
        }

        /*----------------------------------------------------------------*/
        /* Command list                                                    */
        /*----------------------------------------------------------------*/

        @Override
        public void updateCommandList(ULCommand[] commands)
        {
            this.currentCommandList = (commands != null) ? commands : new ULCommand[0];
        }

        public void executeCommands()
        {
            if (fillProgramId == 0 || pathProgramId == 0) return;

            glDisable(GL_SCISSOR_TEST);
            glDisable(GL_DEPTH_TEST);
            glDepthMask(false);   // Ultralight never writes depth; suppress writes to avoid
            glDepthFunc(GL_NEVER); // corrupting the main framebuffer's depth buffer.

            for (ULCommand cmd : currentCommandList)
            {
                if (cmd == null) continue;
                switch (cmd.type)
                {
                    case DRAW_GEOMETRY -> drawGeometry(cmd);
                    case CLEAR_RENDER_BUFFER -> clearRenderBuffer(cmd);
                }
            }

            currentCommandList = new ULCommand[0];
            glDisable(GL_SCISSOR_TEST);
            glDepthMask(true);   // Restore depth writes for the rest of the frame.
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glUseProgram(0);
        }

        private void drawGeometry(ULCommand cmd)
        {
            ULGPUState state = cmd.gpuState;
            if (state == null) return;

            GeometryData geo = geometries.get(cmd.geometryID);
            if (geo == null) return;

            // Bind the render buffer (lazily creates FBO if needed).
            bindRenderBuffer(state.renderBufferId);

            glViewport(0, 0, state.viewportWidth, state.viewportHeight);

            // Select program.
            boolean isPath = (state.shaderType == ULShaderType.FILL_PATH);
            int program = isPath ? pathProgramId : fillProgramId;
            glUseProgram(program);

            // Upload all uniforms via UBO — matches UpdateUniforms() in reference.
            uploadUniforms(state);

            // Bind VAO (lazily created).
            int vao = getOrCreateVAO(cmd.geometryID);
            if (vao < 0) return;
            glBindVertexArray(vao);

            // Bind all three texture slots — reference binds all on every draw.
            bindTexture(0, state.texture1Id);
            bindTexture(1, state.texture2Id);
            bindTexture(2, state.texture3Id);

            // Scissor — Ultralight provides coords in top-left origin (CSS space).
            // glScissor expects the bottom-left corner in window/FBO space (bottom-left origin),
            // so we flip: scissorY = viewportHeight - bottom.
            if (state.enableScissor && state.scissorRect != null && !state.scissorRect.isEmpty())
            {
                glEnable(GL_SCISSOR_TEST);
                int scissorY = state.viewportHeight - state.scissorRect.bottom;
                glScissor(state.scissorRect.left,
                        scissorY,
                        state.scissorRect.right  - state.scissorRect.left,
                        state.scissorRect.bottom - state.scissorRect.top);
            }
            else
            {
                glDisable(GL_SCISSOR_TEST);
            }

            // Blend -- ULGPUState only exposes enableBlend (no per-draw factors/equation).
            // Use premultiplied-alpha blend matching the reference driver's default.
            if (state.enableBlend)
            {
                glEnable(GL_BLEND);
                glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
                glBlendEquation(GL_FUNC_ADD);
            }
            else
            {
                glDisable(GL_BLEND);
            }

            glDrawElements(GL_TRIANGLES,
                    cmd.indicesCount,
                    GL_UNSIGNED_INT,
                    (long) cmd.indicesOffset * Integer.BYTES);

            glBindVertexArray(0);
        }

        private void bindTexture(int unit, int ultralightTexId)
        {
            glActiveTexture(GL_TEXTURE0 + unit);
            if (ultralightTexId == 0)
            {
                glBindTexture(GL_TEXTURE_2D, 0);
                return;
            }
            Integer glId = textures.get(ultralightTexId);
            if (glId == null) return;
            glBindTexture(GL_TEXTURE_2D, glId);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }

        private void clearRenderBuffer(ULCommand cmd)
        {
            ULGPUState state = cmd.gpuState;
            if (state == null) return;
            bindRenderBuffer(state.renderBufferId);
            glDisable(GL_SCISSOR_TEST);
            glClearColor(0f, 0f, 0f, 0f);
            glClear(GL_COLOR_BUFFER_BIT);
        }

    }

    /*===========================================================*/
    /*  Blit shader                                              */
    /*===========================================================*/

    private static final class BlitShader
    {
        private static final String VERT = """
                #version 410 core
                layout(location = 0) in vec2 a_Pos;
                layout(location = 1) in vec2 a_UV;
                out vec2 v_UV;
                void main() {
                    gl_Position = vec4(a_Pos, 0.0, 1.0);
                    v_UV = a_UV;
                }
                """;

        private static final String FRAG = """
                #version 410 core
                in  vec2 v_UV;
                out vec4 o_Color;
                uniform sampler2D u_Texture;
                uniform vec4 u_UVRect;
                void main() {
                    vec2 uv = mix(u_UVRect.xy, u_UVRect.zw, v_UV);
                    o_Color = texture(u_Texture, uv);
                }
                """;

        private final int programId;
        private final int loc_UVRect;

        BlitShader()
        {
            int vert = compile(GL_VERTEX_SHADER, VERT);
            int frag = compile(GL_FRAGMENT_SHADER, FRAG);

            programId = glCreateProgram();
            glAttachShader(programId, vert);
            glAttachShader(programId, frag);
            glLinkProgram(programId);
            glDeleteShader(vert);
            glDeleteShader(frag);

            glUseProgram(programId);
            glUniform1i(glGetUniformLocation(programId, "u_Texture"), 0);
            glUseProgram(0);

            loc_UVRect = glGetUniformLocation(programId, "u_UVRect");
        }

        void bind()
        {
            glUseProgram(programId);
        }

        void setUVRect(float u0, float v0, float u1, float v1)
        {
            glUniform4f(loc_UVRect, u0, v0, u1, v1);
        }

        void destroy()
        {
            glDeleteProgram(programId);
        }

        private static int compile(int type, String src)
        {
            int id = glCreateShader(type);
            glShaderSource(id, src);
            glCompileShader(id);
            if (glGetShaderi(id, GL_COMPILE_STATUS) == GL_FALSE)
                throw new RuntimeException("Blit shader: " + glGetShaderInfoLog(id));
            return id;
        }
    }

    /*===========================================================*/
    /*  Fullscreen quad                                          */
    /*===========================================================*/

    private static final class FullscreenQuad
    {
        private final int vao, vbo, ebo;

        FullscreenQuad()
        {
            float[] verts = {
                    -1f, 1f, 0f, 1f,
                    1f, 1f, 1f, 1f,
                    1f, -1f, 1f, 0f,
                    -1f, -1f, 0f, 0f,
            };

            int[] idx = {0, 1, 2, 2, 3, 0};

            vao = glGenVertexArrays();
            vbo = glGenBuffers();
            ebo = glGenBuffers();

            glBindVertexArray(vao);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, verts, GL_STATIC_DRAW);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, idx, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 2, GL_FLOAT, false, 4 * Float.BYTES, 0L);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 2, GL_FLOAT, false, 4 * Float.BYTES, 2L * Float.BYTES);
            glEnableVertexAttribArray(1);

            glBindVertexArray(0);
        }

        void draw()
        {
            glBindVertexArray(vao);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0L);
            glBindVertexArray(0);
        }

        void destroy()
        {
            glDeleteVertexArrays(vao);
            glDeleteBuffers(vbo);
            glDeleteBuffers(ebo);
        }
    }

    /*===========================================================*/
    /*  Data records                                             */
    /*===========================================================*/

    private record GeometryData(int vao, int vbo, int ebo, boolean isQuad)
    {
    }

    /**
     * fboId = -1 means not yet created (created lazily on first draw, matching
     * the reference driver's CreateFBOIfNeededForActiveContext pattern).
     */
    private record RenderBufferData(int fboId, int textureId)
    {
    }
}