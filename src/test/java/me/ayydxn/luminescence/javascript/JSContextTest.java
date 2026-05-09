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
class JSContextTest extends BaseLuminescenceTest
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
    @DisplayName("evaluate() Returns Correct Number Result")
    void testEvaluateNumber() throws Exception
    {
        try (JSContext context = view.acquireJSContextLock())
        {
            assertEquals(2.0, context.evaluate("1 + 1").toNumber(), 0.001);
        }
    }

    @Test
    @Order(2)
    @DisplayName("evaluate() With A Syntax Error Throws JSException")
    void testEvaluateSyntaxError()
    {
        assertThrows(JSException.class, () ->
        {
            try (JSContext context = view.acquireJSContextLock())
            {
                context.evaluate("this is not valid javascript !!!");
            }
        });
    }

    @Test
    @Order(3)
    @DisplayName("evaluate() With A Thrown JSEerror Throws JSException")
    void testEvaluateThrow()
    {
        JSException jsException = assertThrows(JSException.class, () ->
        {
            try (JSContext context = view.acquireJSContextLock())
            {
                context.evaluate("throw new Error('oops')", "test.js", 1);
            }
        });

        assertNotNull(jsException);
        assertNotNull(jsException.getJSValue());
    }

    @Test
    @Order(4)
    @DisplayName("try-with-resources Releases The JS Context Lock")
    void testAutoClose() throws Exception
    {
        try (JSContext context = view.acquireJSContextLock())
        {
            context.evaluate("1");
        }

        // If close() is broken this will deadlock or throw
        try (JSContext context = view.acquireJSContextLock())
        {
            assertEquals(4.0, context.evaluate("2 + 2").toNumber(), 0.001);
        }
    }

    @Test
    @Order(5)
    @DisplayName("All Factory Methods Produce Correct Types")
    void testFactoryMethods() throws Exception
    {
        try (JSContext context = view.acquireJSContextLock())
        {
            assertTrue(context.makeUndefined().isUndefined());
            assertTrue(context.makeNull().isNull());
            assertTrue(context.make(true).isBoolean());
            assertFalse(context.make(false).toBoolean());
            assertTrue(context.make(3.14).isNumber());
            assertEquals(3.14, context.make(3.14).toNumber(), 0.0001);
            assertTrue(context.make("hello").isString());
        }
    }
}