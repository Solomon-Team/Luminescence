package me.ayydxn.luminescence.demo;

import me.ayydxn.luminescence.config.ULConfig;
import me.ayydxn.luminescence.surface.ULBitmapSurface;
import me.ayydxn.luminescence.surface.ULSurface;
import me.ayydxn.luminescence.view.ULView;
import me.ayydxn.luminescence.view.ULViewConfig;
import org.apache.commons.io.FileUtils;
import org.lwjgl.glfw.GLFWVidMode;
import org.lwjgl.system.MemoryUtil;

import java.io.File;
import java.nio.ByteBuffer;
import java.nio.charset.StandardCharsets;
import java.util.Objects;

import static org.lwjgl.glfw.GLFW.glfwGetPrimaryMonitor;
import static org.lwjgl.glfw.GLFW.glfwGetVideoMode;
import static org.lwjgl.opengl.GL11.GL_LINEAR;
import static org.lwjgl.opengl.GL11.GL_RGBA;
import static org.lwjgl.opengl.GL11.GL_TEXTURE_2D;
import static org.lwjgl.opengl.GL11.GL_TEXTURE_MAG_FILTER;
import static org.lwjgl.opengl.GL11.GL_TEXTURE_MIN_FILTER;
import static org.lwjgl.opengl.GL11.GL_TEXTURE_WRAP_S;
import static org.lwjgl.opengl.GL11.GL_TEXTURE_WRAP_T;
import static org.lwjgl.opengl.GL11.GL_UNSIGNED_BYTE;
import static org.lwjgl.opengl.GL11.glBindTexture;
import static org.lwjgl.opengl.GL11.glDeleteTextures;
import static org.lwjgl.opengl.GL11.glTexImage2D;
import static org.lwjgl.opengl.GL11.glTexSubImage2D;
import static org.lwjgl.opengl.GL12.GL_BGRA;
import static org.lwjgl.opengl.GL12C.GL_CLAMP_TO_EDGE;
import static org.lwjgl.opengl.GL20.*;
import static org.lwjgl.opengl.GL20.GL_FRAGMENT_SHADER;
import static org.lwjgl.opengl.GL20.glAttachShader;
import static org.lwjgl.opengl.GL20.glCompileShader;
import static org.lwjgl.opengl.GL20.glCreateProgram;
import static org.lwjgl.opengl.GL20.glCreateShader;
import static org.lwjgl.opengl.GL20.glDeleteProgram;
import static org.lwjgl.opengl.GL20.glDeleteShader;
import static org.lwjgl.opengl.GL20.glLinkProgram;
import static org.lwjgl.opengl.GL20.glShaderSource;
import static org.lwjgl.opengl.GL20.glUseProgram;
import static org.lwjgl.opengl.GL30.*;
import static org.lwjgl.opengl.GL45.glCreateTextures;
import static org.lwjgl.opengl.GL45.glTextureParameteri;

public class LWJGLCPUDemoApp extends LWJGLDemo
{
    private ULView demoView;
    private GLQuad uiQuad;
    private Texture uiTexture;
    private Shader uiShader;
    private String demoHTML;
    private File ultralightVertexShaderFile;
    private File ultralightFragmentShaderFile;

    public LWJGLCPUDemoApp()
    {
        super("Luminescence CPU Demo", 1600, 900);
    }

    @Override
    protected void configureUltralight(ULConfig config)
    {
        try
        {
            var resourcesFolderURL = LWJGLCPUDemoApp.class.getClassLoader().getResource("./ultralight");
            Objects.requireNonNull(resourcesFolderURL);

            GLFWVidMode videoMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
            Objects.requireNonNull(videoMode);

            config.setResourcePathPrefix(new File(resourcesFolderURL.toURI()) + "/");
            config.setUserStylesheet("body { background: white; }");
            config.setScrollTimerDelay(1.0d / videoMode.refreshRate());
            config.setAnimationTimerDelay(1.0d / videoMode.refreshRate());

            File demoHTMLFile = new File(resourcesFolderURL.toURI().resolve("ultralight/html/demo.html"));
            this.demoHTML = FileUtils.readFileToString(demoHTMLFile, StandardCharsets.UTF_8);

            this.ultralightVertexShaderFile = new File(resourcesFolderURL.toURI().resolve("ultralight/shaders/UltralightPathVertex.glsl"));
            this.ultralightFragmentShaderFile = new File(resourcesFolderURL.toURI().resolve("ultralight/shaders/UltralightPathFragment.glsl"));
        }
        catch (Exception exception)
        {
            exception.printStackTrace();
        }
    }

    @Override
    protected void onInit() throws Exception
    {
        try (ULViewConfig viewConfig = new ULViewConfig())
        {
            viewConfig.isAccelerated(false);

            this.demoView = new ULView(this.renderer, width, height, viewConfig, null);
            this.demoView.loadHTML(this.demoHTML);
        }

        this.uiShader = new Shader(FileUtils.readFileToString(this.ultralightVertexShaderFile, StandardCharsets.UTF_8),
                FileUtils.readFileToString(this.ultralightFragmentShaderFile, StandardCharsets.UTF_8));
        this.uiTexture = new Texture();
        this.uiQuad = new GLQuad();
    }

    @Override
    protected void onUpdate()
    {
    }

    @Override
    protected void onRender()
    {
        ULSurface surface = demoView.getSurface();
        if (surface != null && surface.getHandle() != MemoryUtil.NULL)
        {
            ULBitmapSurface bitmapSurface = ULBitmapSurface.fromSurface(surface);

            try (ULSurface.LockedPixels locked = bitmapSurface.acquirePixelLock())
            {
                this.uiTexture.setData(bitmapSurface.getWidth(), bitmapSurface.getHeight(), locked.pixels());
            }
        }

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        this.uiShader.bind();
        this.uiTexture.bind();
        this.uiQuad.draw();

        glDisable(GL_BLEND);
    }

    @Override
    protected void onShutdown()
    {
        if (demoView != null)
            demoView.destroy();

        uiTexture.destroy();
        uiShader.destroy();
        uiQuad.destroy();
    }

    private static final class Shader
    {
        private final int programId;

        public Shader(String vertexSource, String fragmentSource)
        {
            int vertexShader = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertexShader, vertexSource);
            glCompileShader(vertexShader);

            int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragmentShader, fragmentSource);
            glCompileShader(fragmentShader);

            this.programId = glCreateProgram();
            glAttachShader(programId, vertexShader);
            glAttachShader(programId, fragmentShader);
            glLinkProgram(programId);

            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);
        }

        public void bind()
        {
            glUseProgram(programId);
        }

        public void destroy()
        {
            glDeleteProgram(programId);
        }
    }

    private static final class Texture
    {
        private final int id;
        private int width = -1;
        private int height = -1;

        public Texture()
        {
            this.id = glCreateTextures(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, id);

            glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }

        public void bind()
        {
            glBindTexture(GL_TEXTURE_2D, id);
        }

        public void destroy()
        {
            glDeleteTextures(id);
        }

        public void setData(int width, int height, ByteBuffer pixels)
        {
            glBindTexture(GL_TEXTURE_2D, id);

            if (this.width == width && this.height == height)
            {
                glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_BGRA, GL_UNSIGNED_BYTE, pixels);
            }
            else
            {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, pixels);

                this.width = width;
                this.height = height;
            }
        }
    }

    public class GLQuad
    {
        private final int vao, vbo, ebo;
        private final int indexCount;

        public GLQuad()
        {
            float[] vertices = {
                    // positions    // texture coords
                    -0.7f,  0.9f,   0.0f, 0.0f,
                     0.7f,  0.9f,   1.0f, 0.0f,
                     0.7f, -0.9f,   1.0f, 1.0f,
                    -0.7f, -0.9f,   0.0f, 1.0f
            };

            int[] indices = {0, 1, 2, 2, 3, 0};
            this.indexCount = indices.length;

            this.vao = glGenVertexArrays();
            this.vbo = glGenBuffers();
            this.ebo = glGenBuffers();

            glBindVertexArray(vao);

            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, vertices, GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 2, GL_FLOAT, false, 4 * 4, 0);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 2, GL_FLOAT, false, 4 * 4, 2 * 4);
            glEnableVertexAttribArray(1);
        }

        public void draw()
        {
            glBindVertexArray(vao);
            glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
        }

        public void destroy()
        {
            glDeleteVertexArrays(vao);
            glDeleteBuffers(vbo);
            glDeleteBuffers(ebo);
        }
    }
}
