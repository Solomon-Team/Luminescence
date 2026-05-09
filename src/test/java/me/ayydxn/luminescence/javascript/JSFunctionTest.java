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
import org.junit.jupiter.api.*;

import java.io.File;
import java.net.URL;
import java.util.Objects;
import java.util.concurrent.atomic.AtomicBoolean;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertTrue;

@TestInstance(TestInstance.Lifecycle.PER_CLASS)
@TestMethodOrder(MethodOrderer.OrderAnnotation.class)
class JSFunctionTest extends BaseLuminescenceTest
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
    @DisplayName("JSFunction.create() Produces A Function Object")
    void testCreateIsFunction() throws Exception
    {
        try (JSContext context = view.acquireJSContextLock())
        {
            JSFunction jsFunction = JSFunction.create(context, "testFn", (jsContext, self, args) -> jsContext.makeNull());

            assertTrue(jsFunction.isFunction());
            assertTrue(jsFunction.isObject());
        }
    }

    @Test
    @Order(2)
    @DisplayName("JSFunction.call() From Java Invokes The Lambda")
    void testCallFromJava() throws Exception
    {
        AtomicBoolean called = new AtomicBoolean(false);

        try (JSContext context = view.acquireJSContextLock())
        {
            JSFunction jsFunction = JSFunction.create(context, "myFn", (jsContext, self, args) ->
            {
                called.set(true);

                return jsContext.make(42.0);
            });

            JSValue result = jsFunction.call(context.globalObject());

            assertTrue(called.get());
            assertEquals(42.0, result.toNumber(), 0.001);
        }
    }

    @Test
    @Order(3)
    @DisplayName("JSFunction Registered On globalObject Is Callable From JS")
    void testCallFromJS() throws Exception
    {
        AtomicDouble received = new AtomicDouble(0);

        try (JSContext context = view.acquireJSContextLock())
        {
            JSFunction jsFunction = JSFunction.create(context, "addTen", (jsContext, self, args) ->
            {
                double v = args.length > 0 ? args[0].toNumber() : 0;
                received.set(v);

                return jsContext.make(v + 10.0);
            });

            context.globalObject().setProperty("addTen", jsFunction);

            JSValue result = context.evaluate("addTen(5)");

            assertEquals(5.0, received.get(), 0.001);
            assertEquals(15.0, result.toNumber(), 0.001);
        }
    }

    @Test
    @Order(4)
    @DisplayName("JSFunction Receives Correct Argument Count")
    void testArgCount() throws Exception
    {
        try (JSContext ctx = view.acquireJSContextLock())
        {
            AtomicDouble sum = new AtomicDouble(0);
            JSFunction fn = JSFunction.create(ctx, "summer", (context, self, args) ->
            {
                double currentSum = 0;

                for (JSValue arg : args)
                    currentSum += arg.toNumber();

                sum.set(currentSum);

                return context.make(currentSum);
            });
            ctx.globalObject().setProperty("summer", fn);
            ctx.evaluate("summer(1, 2, 3)");
            assertEquals(6.0, sum.get(), 0.001);
        }
    }
}