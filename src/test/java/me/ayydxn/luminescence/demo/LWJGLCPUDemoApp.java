package me.ayydxn.luminescence.demo;

import me.ayydxn.luminescence.config.ULConfig;
import me.ayydxn.luminescence.events.*;
import me.ayydxn.luminescence.javascript.JSContext;
import me.ayydxn.luminescence.javascript.JSException;
import me.ayydxn.luminescence.javascript.JSFunction;
import me.ayydxn.luminescence.surface.ULBitmapSurface;
import me.ayydxn.luminescence.surface.ULSurface;
import me.ayydxn.luminescence.view.ULView;
import me.ayydxn.luminescence.view.ULViewConfig;
import me.ayydxn.luminescence.view.ULViewListener;
import org.apache.commons.io.FileUtils;
import org.lwjgl.glfw.GLFWVidMode;
import org.lwjgl.system.MemoryUtil;

import java.io.File;
import java.nio.ByteBuffer;
import java.nio.charset.StandardCharsets;
import java.util.Objects;

import static org.lwjgl.glfw.GLFW.*;
import static org.lwjgl.opengl.GL11.*;
import static org.lwjgl.opengl.GL12.GL_BGRA;
import static org.lwjgl.opengl.GL12C.GL_CLAMP_TO_EDGE;
import static org.lwjgl.opengl.GL20.*;
import static org.lwjgl.opengl.GL20.GL_BLEND;
import static org.lwjgl.opengl.GL20.GL_FLOAT;
import static org.lwjgl.opengl.GL20.GL_ONE_MINUS_SRC_ALPHA;
import static org.lwjgl.opengl.GL20.GL_SRC_ALPHA;
import static org.lwjgl.opengl.GL20.GL_TRIANGLES;
import static org.lwjgl.opengl.GL20.GL_UNSIGNED_INT;
import static org.lwjgl.opengl.GL20.glBlendFunc;
import static org.lwjgl.opengl.GL20.glDisable;
import static org.lwjgl.opengl.GL20.glDrawElements;
import static org.lwjgl.opengl.GL20.glEnable;
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

            File demoHTMLFile = new File(resourcesFolderURL.toURI().resolve("ultralight/html/advanced_demo.html"));
            this.demoHTML = FileUtils.readFileToString(demoHTMLFile, StandardCharsets.UTF_8);

            this.ultralightVertexShaderFile = new File(resourcesFolderURL.toURI().resolve("ultralight/shaders/UltralightQuadVertex.glsl"));
            this.ultralightFragmentShaderFile = new File(resourcesFolderURL.toURI().resolve("ultralight/shaders/UltralightQuadFragment.glsl"));
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
            this.demoView.setListener(new ULViewListener()
            {
                @Override
                public void onDOMReady(long frameID, boolean isMainFrame, String url)
                {
                    if (!isMainFrame)
                        return;

                    try (JSContext context = LWJGLCPUDemoApp.this.demoView.acquireJSContextLock())
                    {
                        context.globalObject().setProperty("onPrimaryAction", JSFunction.create(context, "onPrimaryAction", (jsContext, self, args) ->
                        {
                            System.out.println("[Java] onPrimaryAction");
                            return jsContext.makeUndefined();
                        }));
                        context.globalObject().setProperty("onSecondaryAction", JSFunction.create(context, "onSecondaryAction", (jsContext, self, args) ->
                        {
                            System.out.println("[Java] onSecondaryAction");
                            return jsContext.makeUndefined();
                        }));
                        context.globalObject().setProperty("onWarningAction", JSFunction.create(context, "onWarningAction", (jsContext, self, args) ->
                        {
                            System.out.println("[Java] onWarningAction");
                            return jsContext.makeUndefined();
                        }));
                        context.globalObject().setProperty("onDangerAction", JSFunction.create(context, "onDangerAction", (jsContext, self, args) ->
                        {
                            System.out.println("[Java] onDangerAction");
                            return jsContext.makeUndefined();
                        }));
                        context.globalObject().setProperty("onTextSubmit", JSFunction.create(context, "onTextSubmit", (jsContext, self, args) ->
                        {
                            System.out.println("[Java] onTextSubmit: " + (args.length > 0 ? args[0].toString() : ""));
                            return jsContext.makeUndefined();
                        }));
                        context.globalObject().setProperty("onNumberChange", JSFunction.create(context, "onNumberChange", (jsContext, self, args) ->
                        {
                            System.out.println("[Java] onNumberChange: " + (args.length > 0 ? args[0].toString() : ""));
                            return jsContext.makeUndefined();
                        }));
                        context.globalObject().setProperty("onPayloadSubmit", JSFunction.create(context, "onPayloadSubmit", (jsContext, self, args) ->
                        {
                            System.out.println("[Java] onPayloadSubmit: " + (args.length > 0 ? args[0].toString() : ""));
                            return jsContext.makeUndefined();
                        }));
                        context.globalObject().setProperty("onRenderModeChange", JSFunction.create(context, "onRenderModeChange", (jsContext, self, args) ->
                        {
                            System.out.println("[Java] onRenderModeChange: " + (args.length > 0 ? args[0].toString() : ""));
                            return jsContext.makeUndefined();
                        }));
                        context.globalObject().setProperty("onLogLevelChange", JSFunction.create(context, "onLogLevelChange", (jsContext, self, args) ->
                        {
                            System.out.println("[Java] onLogLevelChange: " + (args.length > 0 ? args[0].toString() : ""));
                            return jsContext.makeUndefined();
                        }));
                        context.globalObject().setProperty("onFlagChange", JSFunction.create(context, "onFlagChange", (jsContext, self, args) ->
                        {
                            System.out.println("[Java] onFlagChange: " + (args.length > 0 ? args[0].toString() : ""));
                            return jsContext.makeUndefined();
                        }));

                        context.evaluate("window.pushLogEntry && window.pushLogEntry('ok', 'Java bridge registered — " + 10 + " functions')");
                    }
                    catch (JSException exception)
                    {
                        exception.printStackTrace();
                    }
                }
            });
            this.demoView.loadHTML(this.demoHTML);
        }

        this.uiShader = new Shader(FileUtils.readFileToString(this.ultralightVertexShaderFile, StandardCharsets.UTF_8),
                FileUtils.readFileToString(this.ultralightFragmentShaderFile, StandardCharsets.UTF_8));
        this.uiTexture = new Texture();
        this.uiQuad = new GLQuad();

        glfwSetKeyCallback(this.windowHandle, (window, key, scancode, action, mods) ->
        {
            if (action == GLFW_REPEAT)
                return;

            KeyEventType type = (action == GLFW_RELEASE) ? KeyEventType.KEY_UP : KeyEventType.RAW_KEY_DOWN;

            try (ULKeyEvent keyEvent = new ULKeyEvent(
                    type,
                    this.convertGLFWModifiersToUltralight(mods),
                    this.convertGLFWKeyToWindowsVK(key), // virtualKeyCode: GLFW keys align with most Windows VK codes, but some need to be remapped.
                    scancode, // nativeKeyCode
                    "", // text (handled by CharCallback)
                    "", // unmodifiedText
                    false, // isKeypad (can be refined by checking scancodes if needed)
                    false, // isAutoRepeat
                    false // isSystemKey
            ))
            {
                this.demoView.fireKeyEvent(keyEvent);
            }
        });

        glfwSetCharCallback(windowHandle, (window, codepoint) ->
        {
            String text = String.valueOf((char) codepoint);

            try (ULKeyEvent keyEvent = new ULKeyEvent(
                    KeyEventType.CHAR,
                    0, // modifiers
                    0, // virtualKeyCode
                    0, // nativeKeyCode
                    text, // The actual character typed
                    text, // unmodifiedText
                    false, // isKeypad
                    false, // isAutoRepeat
                    false // isSystemKey
            ))
            {
                this.demoView.fireKeyEvent(keyEvent);
            }
        });

        glfwSetMouseButtonCallback(this.windowHandle, (window, button, action, mods) ->
        {
            MouseButton mouseButton = switch (button)
            {
                case GLFW_MOUSE_BUTTON_LEFT -> MouseButton.LEFT;
                case GLFW_MOUSE_BUTTON_RIGHT -> MouseButton.RIGHT;
                case GLFW_MOUSE_BUTTON_MIDDLE -> MouseButton.MIDDLE;
                default -> MouseButton.NONE;
            };

            MouseEventType eventType = switch (action)
            {
                case GLFW_PRESS -> MouseEventType.MOUSE_DOWN;
                case GLFW_RELEASE -> MouseEventType.MOUSE_UP;
                default -> null;
            };

            if (eventType != null)
            {
                double[] xPos = new double[1];
                double[] yPos = new double[1];
                glfwGetCursorPos(window, xPos, yPos);

                try (ULMouseEvent mouseEvent = new ULMouseEvent(eventType, (int) xPos[0], (int) yPos[0], mouseButton))
                {
                    this.demoView.fireMouseEvent(mouseEvent);
                }
            }
        });

        glfwSetCursorPosCallback(this.windowHandle, (window, xPosition, yPosition) ->
        {
            try (ULMouseEvent mouseMovedEvent = new ULMouseEvent(MouseEventType.MOUSE_MOVED, (int) xPosition, (int) yPosition, MouseButton.NONE))
            {
                this.demoView.fireMouseEvent(mouseMovedEvent);
            }
        });

        glfwSetScrollCallback(this.windowHandle, (window, xOffset, yOffset) ->
        {
            try (ULScrollEvent scrollEvent = new ULScrollEvent(ScrollEventType.SCROLL_BY_PIXEL, (int) (xOffset * 32), (int) (yOffset * 32)))
            {
                this.demoView.fireScrollEvent(scrollEvent);
            }
        });

        glfwSetFramebufferSizeCallback(this.windowHandle, (window, width, height) ->
        {
            this.demoView.resize(width, height);

            glViewport(0, 0, width, height);
        });
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

    private int convertGLFWKeyToWindowsVK(int glfwKey)
    {
        return switch (glfwKey) {
            case GLFW_KEY_BACKSPACE -> 0x08;
            case GLFW_KEY_TAB -> 0x09;
            case GLFW_KEY_ENTER -> 0x0D;
            case GLFW_KEY_ESCAPE -> 0x1B;
            case GLFW_KEY_SPACE -> 0x20;
            case GLFW_KEY_PAGE_UP -> 0x21;
            case GLFW_KEY_PAGE_DOWN -> 0x22;
            case GLFW_KEY_END -> 0x23;
            case GLFW_KEY_HOME -> 0x24;
            case GLFW_KEY_LEFT -> 0x25;
            case GLFW_KEY_UP -> 0x26;
            case GLFW_KEY_RIGHT -> 0x27;
            case GLFW_KEY_DOWN -> 0x28;
            case GLFW_KEY_INSERT -> 0x2D;
            case GLFW_KEY_DELETE -> 0x2E;

            // Standard alphanumeric keys (A-Z, 0-9) usually match 1:1
            default -> glfwKey;
        };
    }

    private int convertGLFWModifiersToUltralight(int glfwMods)
    {
        int ulMods = 0;

        if ((glfwMods & GLFW_MOD_ALT) != 0) ulMods |= 1;
        if ((glfwMods & GLFW_MOD_CONTROL) != 0) ulMods |= 1 << 1;
        if ((glfwMods & GLFW_MOD_SUPER) != 0) ulMods |= 1 << 2;
        if ((glfwMods & GLFW_MOD_SHIFT) != 0) ulMods |= 1 << 3;

        return ulMods;
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

    private static class GLQuad
    {
        private final int vao, vbo, ebo;
        private final int indexCount;

        public GLQuad()
        {
            float[] vertices = {
                    // positions    // texture coords
                    -1.0f, 1.0f, 0.0f, 0.0f,
                    1.0f, 1.0f, 1.0f, 0.0f,
                    1.0f, -1.0f, 1.0f, 1.0f,
                    -1.0f, -1.0f, 0.0f, 1.0f
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
