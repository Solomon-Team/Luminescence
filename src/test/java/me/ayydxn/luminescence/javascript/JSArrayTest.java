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

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertTrue;

@TestInstance(TestInstance.Lifecycle.PER_CLASS)
@TestMethodOrder(MethodOrderer.OrderAnnotation.class)
class JSArrayTest extends BaseLuminescenceTest
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
    @DisplayName("[1,2,3] Evaluates To A JSArray With Length 3")
    void testArrayLength() throws Exception
    {
        try (JSContext context = view.acquireJSContextLock())
        {
            JSValue value = context.evaluate("[1, 2, 3]");

            assertTrue(value.isArray());
            assertEquals(3, ((JSArray) value).length());
        }
    }

    @Test
    @Order(2)
    @DisplayName("JSArray.get(index) Returns Correct Element")
    void testArrayGet() throws Exception
    {
        try (JSContext context = view.acquireJSContextLock())
        {
            JSArray jsArray = (JSArray) context.evaluate("[10, 20, 30]");

            assertEquals(10.0, jsArray.get(0).toNumber(), 0.001);
            assertEquals(20.0, jsArray.get(1).toNumber(), 0.001);
            assertEquals(30.0, jsArray.get(2).toNumber(), 0.001);
        }
    }

    @Test
    @Order(3)
    @DisplayName("JSArray.set(index, value) Updates The Element")
    void testArraySet() throws Exception
    {
        try (JSContext context = view.acquireJSContextLock())
        {
            JSArray jsArray = (JSArray) context.evaluate("[1, 2, 3]");
            jsArray.set(1, context.make(99.0));

            assertEquals(99.0, jsArray.get(1).toNumber(), 0.001);
        }
    }
}