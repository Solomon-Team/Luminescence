package me.ayydxn.luminescence.renderer;

import me.ayydxn.luminescence.BaseLuminescenceTest;
import me.ayydxn.luminescence.config.ULConfig;
import me.ayydxn.luminescence.platform.ULPlatform;
import me.ayydxn.luminescence.platform.impl.StandardULFileSystem;
import me.ayydxn.luminescence.platform.impl.StandardULFontLoader;
import org.junit.jupiter.api.AfterAll;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;

import java.io.File;
import java.net.URISyntaxException;
import java.net.URL;
import java.util.Objects;

import static org.junit.jupiter.api.Assertions.*;

public class ULSessionTest extends BaseLuminescenceTest
{
    private static ULRenderer renderer;

    @BeforeAll
    static void setupRenderer()
    {
        URL ultralightResourcesURL = Objects.requireNonNull(ULRendererTest.class.getClassLoader().getResource("./ultralight"));

        ULPlatform.setFileSystem(new StandardULFileSystem());
        ULPlatform.setFontLoader(new StandardULFontLoader());

        try (ULConfig config = new ULConfig())
        {
            config.setResourcePathPrefix(new File(ultralightResourcesURL.toURI()) + "/");

            renderer = new ULRenderer(config);
        }
        catch (URISyntaxException exception)
        {
            exception.printStackTrace();
        }
    }

    @Test
    @DisplayName("Verify Persistent Session Creation and Metadata")
    void testSessionCreation()
    {
        ULSession session = ULSession.create(renderer, true, "TestSession");

        assertNotNull(session);
        assertTrue(session.isPersistent());
        assertEquals("TestSession", session.getName());
        assertNotNull(session.getDiskPath());

        session.destroy();
    }

    @Test
    @DisplayName("Verify Default Session Lifecycle")
    void testDefaultSession()
    {
        ULSession defaultSession = ULSession.createDefault(renderer);

        assertNotNull(defaultSession);
        assertTrue(defaultSession.isPersistent(), "Default session should be persistent");
        assertEquals("default", defaultSession.getName().toLowerCase());

        assertThrows(UnsupportedOperationException.class, defaultSession::destroy);
    }

    @AfterAll
    static void cleanup()
    {
        renderer.destroy();
    }
}
