package me.ayydxn.luminescence.javascript;

import com.google.common.util.concurrent.AtomicDouble;
import me.ayydxn.luminescence.BaseLuminescenceTest;
import me.ayydxn.luminescence.config.ULConfig;
import me.ayydxn.luminescence.platform.ULPlatform;
import me.ayydxn.luminescence.platform.impl.StandardULFileSystem;
import me.ayydxn.luminescence.platform.impl.StandardULFontLoader;
import me.ayydxn.luminescence.renderer.ULRenderer;
import me.ayydxn.luminescence.view.ULView;
import me.ayydxn.luminescence.view.ULViewConfig;
import me.ayydxn.luminescence.view.ULViewListener;
import org.junit.jupiter.api.*;

import java.io.File;
import java.net.URL;
import java.util.Objects;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicReference;

import static org.junit.jupiter.api.Assertions.*;

@TestInstance(TestInstance.Lifecycle.PER_CLASS)
class JSInteropIntegrationTest extends BaseLuminescenceTest
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

    private void pumpUntilLatchFireOrTimeout(CountDownLatch latch, long timeoutMillis) throws InterruptedException
    {
        long deadline = System.currentTimeMillis() + timeoutMillis;

        while (!latch.await(50, TimeUnit.MILLISECONDS))
        {
            renderer.update();
            renderer.render();

            if (System.currentTimeMillis() > deadline)
                break;
        }
    }

    @Test
    @DisplayName("Java-Registered Function Is Callable From Page JavaScript On DOM Ready")
    void testJavaFunctionCalledFromPageJS() throws Exception
    {
        CountDownLatch latch = new CountDownLatch(1);
        AtomicDouble receivedArgument = new AtomicDouble(Double.NaN);
        AtomicReference<Throwable> callbackError = new AtomicReference<>(null);

        view.setListener(new ULViewListener()
        {
            @Override
            public void onDOMReady(long frameID, boolean isMainFrame, String url)
            {
                try (JSContext context = view.acquireJSContextLock())
                {
                    JSFunction jsFunction = JSFunction.create(context, "javaCallback", (c, self, args) ->
                    {
                        receivedArgument.set(args.length > 0 ? args[0].toNumber() : Double.NaN);
                        latch.countDown();
                        return c.makeNull();
                    });

                    context.globalObject().setProperty("javaCallback", jsFunction);
                    context.evaluate("javaCallback(99)");
                }
                catch (Throwable t)
                {
                    callbackError.set(t);
                    latch.countDown();
                }
            }
        });

        view.loadHTML("<html><body>test</body></html>");
        this.pumpUntilLatchFireOrTimeout(latch, 5000);

        assertTrue(latch.await(0, TimeUnit.MILLISECONDS), "DOMReady did not fire within 5 seconds");
        assertNull(callbackError.get(), () -> "Exception in onDOMReady: " + callbackError.get());
        assertEquals(99.0, receivedArgument.get(), 0.001);
    }

    @Test
    @DisplayName("JS Exception Thrown Inside Java Callback Propagates Back To evaluate()")
    void testJavaCallbackThrowsPropagatesToJS() throws Exception
    {
        CountDownLatch latch = new CountDownLatch(1);
        AtomicReference<Throwable> callbackError = new AtomicReference<>(null);

        view.setListener(new ULViewListener()
        {
            @Override
            public void onDOMReady(long frameID, boolean isMainFrame, String url)
            {
                try (JSContext ctx = view.acquireJSContextLock())
                {
                    JSFunction fn = JSFunction.create(ctx, "throwingFn", (c, self, args) ->
                    {
                        throw new JSException("error from Java");
                    });

                    ctx.globalObject().setProperty("throwingFn", fn);

                    // The JS exception should propagate back through evaluate()
                    assertThrows(JSException.class, () -> ctx.evaluate("throwingFn()"));
                }
                catch (Throwable t)
                {
                    callbackError.set(t);
                }
                finally
                {
                    latch.countDown();
                }
            }
        });

        view.loadHTML("<html><body>test</body></html>");
        this.pumpUntilLatchFireOrTimeout(latch, 5000);

        assertTrue(latch.await(0, TimeUnit.MILLISECONDS), "DOMReady did not fire within 5 seconds");
        assertNull(callbackError.get(), () -> "Unexpected exception in onDOMReady: " + callbackError.get());
    }
}