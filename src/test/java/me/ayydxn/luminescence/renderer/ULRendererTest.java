package me.ayydxn.luminescence.renderer;

import me.ayydxn.luminescence.BaseLuminescenceTest;
import me.ayydxn.luminescence.config.ULConfig;
import me.ayydxn.luminescence.events.ULGamepadEvent;
import me.ayydxn.luminescence.platform.ULPlatform;
import me.ayydxn.luminescence.platform.impl.StandardULFileSystem;
import me.ayydxn.luminescence.platform.impl.StandardULFontLoader;
import org.junit.jupiter.api.*;

import java.io.File;
import java.net.URISyntaxException;
import java.net.URL;
import java.util.Objects;

import static org.junit.jupiter.api.Assertions.*;

@TestInstance(TestInstance.Lifecycle.PER_CLASS)
class ULRendererTest extends BaseLuminescenceTest
{
    private ULConfig config;

    // (Ayydxn) Probably better to retain a ULRenderer instance throughout the lifetime of the test class instead of using @BeforeEach,
    // destroying it after all tests are done to better simulation a production environment.
    // The main reason being that creating and destroying multiple renderers isn't something you typically nor should do with Ultralight.
    @BeforeAll
    @BeforeEach
    void setupPlatformHandlers()
    {
        URL ultralightResourcesURL = Objects.requireNonNull(ULRendererTest.class.getClassLoader().getResource("./ultralight"));

        ULPlatform.setFileSystem(new StandardULFileSystem());
        ULPlatform.setFontLoader(new StandardULFontLoader());

        config = new ULConfig();

        try
        {
            config.setResourcePathPrefix(new File(ultralightResourcesURL.toURI()) + "/");
        }
        catch (URISyntaxException exception)
        {
            exception.printStackTrace();
        }
    }

    @Test
    @DisplayName("Ensure Renderer Can Be Created and Destroyed Cleanly")
    void testRendererLifecycle()
    {
        assertDoesNotThrow(() ->
        {
            ULRenderer renderer = new ULRenderer(this.config);

            renderer.update();
            renderer.render();
            renderer.destroy();
        }, "Renderer should initialize and shutdown without access violations.");
    }

    @Test
    @DisplayName("Ensure Safe Memory Purges and Usage Logging")
    void testMemoryUtilities()
    {
        ULRenderer renderer = new ULRenderer(this.config);

        assertDoesNotThrow(renderer::purgeMemory);
        assertDoesNotThrow(renderer::logMemoryUsage);

        renderer.destroy();
    }

    @Test
    @DisplayName("Validate Expected vs Actual Gamepad Event Type Exceptions")
    void testGamepadEventValidation()
    {
        try (ULGamepadEvent axisEvent = ULGamepadEvent.createAxis(0, 1, 0.5))
        {
            ULRenderer renderer = new ULRenderer(this.config);

            // Correct type should not throw
            assertDoesNotThrow(() -> renderer.fireGamepadAxisEvent(axisEvent));

            // Incorrect type (passing Axis to Button) should throw your custom message
            IllegalArgumentException exception = assertThrows(IllegalArgumentException.class, () ->
                    renderer.fireGamepadButtonEvent(axisEvent));

            assertTrue(exception.getMessage().contains("Mismatched Gamepad Event"),
                    "Exception message should guide the user to the correct factory method.");

            renderer.destroy();
        }
    }

    @Test
    @DisplayName("Verify Remote Inspector Server Return Values")
    void testInspectorServer()
    {
        ULRenderer renderer = new ULRenderer(this.config);

        // We can't guarantee a port is free, but we can verify it doesn't crash and returns a boolean.
        boolean result = renderer.startRemoteInspectorServer("127.0.0.1", 9222);
        assertTrue(result);

        renderer.destroy();
    }

    @AfterAll
    void cleanup()
    {
        this.config.close();
    }
}
