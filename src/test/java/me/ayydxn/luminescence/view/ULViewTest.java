package me.ayydxn.luminescence.view;

import me.ayydxn.luminescence.BaseLuminescenceTest;
import me.ayydxn.luminescence.config.ULConfig;
import me.ayydxn.luminescence.platform.ULPlatform;
import me.ayydxn.luminescence.platform.impl.StandardULFileSystem;
import me.ayydxn.luminescence.platform.impl.StandardULFontLoader;
import me.ayydxn.luminescence.renderer.ULRenderer;
import me.ayydxn.luminescence.surface.ULSurface;
import org.junit.jupiter.api.*;

import java.io.File;
import java.net.URISyntaxException;
import java.net.URL;
import java.util.Objects;

import static org.junit.jupiter.api.Assertions.*;

@TestInstance(TestInstance.Lifecycle.PER_CLASS)
@TestMethodOrder(MethodOrderer.OrderAnnotation.class)
class ULViewTest extends BaseLuminescenceTest
{
    private ULRenderer renderer;
    private ULView view;

    @BeforeAll
    void setup()
    {
        ULPlatform.setFileSystem(new StandardULFileSystem());
        ULPlatform.setFontLoader(new StandardULFontLoader());

        ULConfig config = new ULConfig();
        try
        {
            URL resourcesURL = Objects.requireNonNull(getClass().getClassLoader().getResource("ultralight"));
            String path = new File(resourcesURL.toURI()).getAbsolutePath() + File.separator;

            config.setResourcePathPrefix(path);
        }
        catch (URISyntaxException | NullPointerException e)
        {
            fail("Could not find Ultralight resources in test/resources/ultralight");
        }

        renderer = new ULRenderer(config);

        try (ULViewConfig viewConfig = new ULViewConfig())
        {
            viewConfig.setInitialFocus(true);

            view = new ULView(renderer, 1024, 768, viewConfig, null);
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
    @DisplayName("Verify Initial View State and Configuration")
    void testInitialState()
    {
        assertEquals(1024, view.getWidth());
        assertEquals(768, view.getHeight());
        assertEquals(1.0d, view.getDeviceScale());
        assertTrue(view.hasFocus(), "View should have initial focus from config");

        view.setDeviceScale(1.5d);
        assertEquals(1.5d, view.getDeviceScale());

        view.setDisplayID(2);
        assertEquals(2, view.getDisplayID());
    }

    @Test
    @Order(2)
    @DisplayName("Test HTML Loading and Title Updates")
    void testContentLoading()
    {
        String testTitle = "Luminescence Test Page";
        String html = "<html><head><title>" + testTitle + "</title></head><body><h1>Hello</h1></body></html>";

        view.loadHTML(html);

        int attempts = 0;
        while ((view.getTitle().isEmpty() || view.getURL().isEmpty()) && attempts < 100)
        {
            renderer.update();

            try
            {
                Thread.sleep(10);
            }
            catch (InterruptedException ignored)
            {
            }

            attempts++;
        }

        renderer.render();

        assertEquals(testTitle, view.getTitle());
    }

    @Test
    @Order(3)
    @DisplayName("Test JavaScript Bridge and Exception Handling")
    void testJavaScript()
    {
        String[] exception = new String[1];

        String result = view.evaluateScript("var x = 5; var y = 10; (x + y).toString();", exception);
        assertEquals("15", result);
        assertNull(exception[0]);

        view.evaluateScript("thisObjectDoesNotExist.property", exception);
        assertNotNull(exception[0]);
        assertTrue(exception[0].contains("ReferenceError"), "Should capture ReferenceError");

        assertDoesNotThrow(() ->
        {
            try (ULView.JSContext jsContext = view.acquireJSContextLock())
            {
                assertNotEquals(0, jsContext.context(), "Native JSContext handle should be non-zero");
            }
        });
    }

    // TODO: (Ayydxn) Extend to also test GPU-accelerated views
    @Test
    @Order(4)
    @DisplayName("Test Rendering Backend (Surface)")
    void testRenderingBackend()
    {
        ULSurface surface = view.getSurface();
        assertNotEquals(0, surface.getHandle(), "Software view must have a Surface");
        assertEquals(view.getWidth(), surface.getWidth());

        try (ULSurface.LockedPixels lock = surface.acquirePixelLock())
        {
            assertNotNull(lock.pixels(), "Locked ByteBuffer should not be null");
            assertTrue(lock.pixels().capacity() > 0);
        }
    }

    @Test
    @Order(5)
    @DisplayName("Test Navigation History")
    void testHistory()
    {
        view.loadURL("about:blank");

        renderer.update();

        assertDoesNotThrow(() ->
        {
            if (view.canGoBack())
                view.goBack();

            if (view.canGoForward())
                view.goForward();

            view.reload();
            view.stop();
        });
    }

    @Test
    @Order(6)
    @DisplayName("Test View Resizing")
    void testResize()
    {
        int newWidth = 1920;
        int newHeight = 1080;

        view.resize(newWidth, newHeight);
        renderer.update();

        assertEquals(newWidth, view.getWidth());
        assertEquals(newHeight, view.getHeight());

        if (!view.isAccelerated())
        {
            assertEquals(newWidth, view.getSurface().getWidth());
        }
    }
}