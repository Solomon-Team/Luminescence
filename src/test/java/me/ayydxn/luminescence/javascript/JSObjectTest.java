
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
import java.util.Arrays;
import java.util.Objects;

import static org.junit.jupiter.api.Assertions.*;

@TestInstance(TestInstance.Lifecycle.PER_CLASS)
@TestMethodOrder(MethodOrderer.OrderAnnotation.class)
class JSObjectTest extends BaseLuminescenceTest
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
    @DisplayName("globalObject() Returns A JSObject")
    void testGlobalObjectIsObject()
    {
        try (JSContext context = view.acquireJSContextLock())
        {
            JSObject global = context.globalObject();
            assertNotNull(global);
            assertTrue(global.isObject());
        }
    }

    @Test
    @Order(2)
    @DisplayName("setProperty / getProperty Round-Trips A Number")
    void testSetGetProperty() throws Exception
    {
        try (JSContext context = view.acquireJSContextLock())
        {
            JSObject globalObject = context.globalObject();
            globalObject.setProperty("testNum", context.make(99.0));

            assertEquals(99.0, globalObject.getProperty("testNum").toNumber(), 0.001);
        }
    }

    @Test
    @Order(3)
    @DisplayName("hasProperty Returns Correctly")
    void testHasProperty() throws Exception
    {
        try (JSContext context = view.acquireJSContextLock())
        {
            JSObject globalObject = context.globalObject();
            globalObject.setProperty("existsProp", context.make(true));

            assertTrue(globalObject.hasProperty("existsProp"));
            assertFalse(globalObject.hasProperty("doesNotExist_xyz"));
        }
    }

    @Test
    @Order(4)
    @DisplayName("deleteProperty Removes A Property")
    void testDeleteProperty() throws Exception
    {
        try (JSContext context = view.acquireJSContextLock())
        {
            JSObject globalObject = context.globalObject();
            globalObject.setProperty("toDelete", context.make(1.0));
            globalObject.deleteProperty("toDelete");

            assertFalse(globalObject.hasProperty("toDelete"));
        }
    }

    @Test
    @Order(5)
    @DisplayName("getPropertyNames Includes A Recently-Added Property")
    void testGetPropertyNames() throws Exception
    {
        try (JSContext context = view.acquireJSContextLock())
        {
            JSObject globalObject = context.globalObject();
            globalObject.setProperty("uniquePropXyz", context.make(42.0));

            assertTrue(Arrays.asList(globalObject.getPropertyNames()).contains("uniquePropXyz"));
        }
    }
}