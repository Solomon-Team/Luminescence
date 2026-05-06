package me.ayydxn.luminescence.demo;

import me.ayydxn.luminescence.config.ULConfig;
import me.ayydxn.luminescence.platform.ULPlatform;
import me.ayydxn.luminescence.platform.impl.StandardULFileSystem;
import me.ayydxn.luminescence.platform.impl.StandardULFontLoader;
import me.ayydxn.luminescence.renderer.ULRenderer;
import org.lwjgl.glfw.GLFWErrorCallback;
import org.lwjgl.opengl.GL;
import org.lwjgl.system.MemoryUtil;

import java.util.Objects;

import static org.lwjgl.glfw.Callbacks.glfwFreeCallbacks;
import static org.lwjgl.glfw.GLFW.*;
import static org.lwjgl.opengl.GL11.*;

public abstract class LWJGLDemo
{
    protected final String title;
    protected final int width;
    protected final int height;

    protected long windowHandle;
    protected ULRenderer renderer;
    protected boolean isRunning;

    public LWJGLDemo(String title, int width, int height)
    {
        this.title = title;
        this.width = width;
        this.height = height;
    }

    public void run() throws Exception
    {
        this.initWindow();
        this.initUltralight();
        this.onInit();

        this.isRunning = true;
        while (this.isRunning)
        {
            glfwPollEvents();

            this.onUpdate();
            this.renderer.update();

            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            this.renderer.refreshDisplay(0);
            this.renderer.render();
            this.onRender();

            glfwSwapBuffers(windowHandle);

            if (glfwWindowShouldClose(windowHandle))
                this.isRunning = false;
        }

        this.onShutdown();
        this.cleanup();
    }

    private void initWindow()
    {
        GLFWErrorCallback.createPrint().set();
        if (!glfwInit())
            throw new IllegalStateException("Unable to initialize GLFW");

        glfwDefaultWindowHints();
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        this.windowHandle = glfwCreateWindow(width, height, title, MemoryUtil.NULL, MemoryUtil.NULL);
        if (windowHandle == MemoryUtil.NULL) throw new RuntimeException("Failed to create GLFW window");

        glfwMakeContextCurrent(windowHandle);
        glfwSwapInterval(1);
        glfwShowWindow(windowHandle);

        GL.createCapabilities();
    }

    private void initUltralight()
    {
        ULPlatform.setFontLoader(new StandardULFontLoader());
        ULPlatform.setFileSystem(new StandardULFileSystem());

        try (ULConfig config = new ULConfig())
        {
            this.configureUltralight(config);

            this.renderer = new ULRenderer(config);
        }
    }

    // Overridable hooks
    protected void configureUltralight(ULConfig config)
    {
        // Default configs
    }

    protected abstract void onInit() throws Exception;

    protected abstract void onUpdate();

    protected abstract void onRender();

    protected abstract void onShutdown();

    private void cleanup()
    {
        if (this.renderer != null)
            this.renderer.destroy();

        glfwFreeCallbacks(windowHandle);
        glfwDestroyWindow(windowHandle);
        glfwTerminate();
        Objects.requireNonNull(glfwSetErrorCallback(null)).free();
    }
}
