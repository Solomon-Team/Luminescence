package me.ayydxn.luminescence.javascript;

import me.ayydxn.luminescence.BaseLuminescenceTest;
import me.ayydxn.luminescence.config.ULConfig;
import me.ayydxn.luminescence.platform.ULPlatform;
import me.ayydxn.luminescence.platform.impl.StandardULFileSystem;
import me.ayydxn.luminescence.platform.impl.StandardULFontLoader;
import me.ayydxn.luminescence.renderer.ULRenderer;
import me.ayydxn.luminescence.view.ULView;
import me.ayydxn.luminescence.view.ULViewConfig;
import org.junit.jupiter.api.*;

import java.io.File;
import java.net.URL;
import java.util.Objects;

import static org.junit.jupiter.api.Assertions.*;

@TestInstance(TestInstance.Lifecycle.PER_CLASS)
@TestMethodOrder(MethodOrderer.OrderAnnotation.class)
class JSValueTest extends BaseLuminescenceTest
{
    private ULRenderer renderer;
    private ULView view;

    @BeforeAll
    void setup() throws Exception
    {
        ULPlatform.setFileSystem(new StandardULFileSystem());
        ULPlatform.setFontLoader(new StandardULFontLoader());

        URL ultralightResourcesURL = Objects.requireNonNull(JSValueTest.class.getClassLoader().getResource("./ultralight"));

        try (ULConfig config = new ULConfig())
        {
            config.setResourcePathPrefix(new File(ultralightResourcesURL.toURI()) + "/");

            renderer = new ULRenderer(config);
        }

        try (ULViewConfig viewConfig = new ULViewConfig())
        {
            view = new ULView(renderer, 800, 600, viewConfig, null);
        }
    }

    @AfterAll
    void tearDown()
    {
        if (view != null)
            view.destroy();

        if (renderer != null)
            renderer.destroy();
    }

    @Test
    @Order(1)
    @DisplayName("JSValue.isNumber() Returns True For A JS Number")
    void testIsNumber()
    {
        try (JSContext context = view.acquireJSContextLock())
        {
            JSValue value = context.make(42.0);

            assertTrue(value.isNumber());
            assertFalse(value.isString());
            assertEquals(JSType.NUMBER, value.getType());
        }
    }

    @Test
    @Order(2)
    @DisplayName("JSValue.isString() Returns True For A JS String")
    void testIsString()
    {
        try (JSContext context = view.acquireJSContextLock())
        {
            JSValue value = context.make("hello");

            assertTrue(value.isString());
            assertEquals(JSType.STRING, value.getType());
        }
    }

    @Test
    @Order(3)
    @DisplayName("JSValue.isBoolean() Returns True For A JS Boolean")
    void testIsBoolean()
    {
        try (JSContext context = view.acquireJSContextLock())
        {
            JSValue value = context.make(true);

            assertTrue(value.isBoolean());
            assertTrue(value.toBoolean());
        }
    }

    @Test
    @Order(4)
    @DisplayName("JSValue.toNumber() Returns The Correct Double")
    void testToNumber() throws Exception
    {
        try (JSContext context = view.acquireJSContextLock())
        {
            assertEquals(3.14, context.make(3.14).toNumber(), 0.0001);
        }
    }

    @Test
    @Order(5)
    @DisplayName("JSValue.isUndefined() Returns True For Undefined")
    void testIsUndefined()
    {
        try (JSContext context = view.acquireJSContextLock())
        {
            JSValue value = context.makeUndefined();

            assertTrue(value.isUndefined());
            assertEquals(JSType.UNDEFINED, value.getType());
        }
    }

    @Test
    @Order(6)
    @DisplayName("JSValue.isNull() Returns True For Null")
    void testIsNull()
    {
        try (JSContext context = view.acquireJSContextLock())
        {
            JSValue value = context.makeNull();

            assertTrue(value.isNull());
            assertEquals(JSType.NULL, value.getType());
        }
    }
}