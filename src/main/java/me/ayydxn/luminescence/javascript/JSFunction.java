package me.ayydxn.luminescence.javascript;

import me.ayydxn.luminescence.util.UsedByNative;

public class JSFunction extends JSObject
{
    @UsedByNative("JSFunction.cpp")
    JSFunction(long contextHandle, long valueHandle)
    {
        super(contextHandle, valueHandle);
    }

    /**
     * Creates a JS function backed by a Java callback and registers it in the given context.
     * <p>
     * <b>Lifetime warning:</b> The {@code callback} lambda and anything captured in its
     * closure will be kept alive by the JSC garbage collector until the returned JS function
     * object becomes unreachable in JavaScript. This may significantly outlive the Java
     * {@code JSFunction} wrapper itself. Avoid capturing large object graphs in the closure.
     */
    public static JSFunction create(JSContext context, String name, JSCallback callback) throws JSException
    {
        return NativeMethods.nJSFunctionCreate(context.getHandle(), name, callback);
    }

    public JSValue call(JSObject thisObject, JSValue... arguments) throws JSException
    {
        long[] handles = new long[arguments.length];
        for (int i = 0; i < arguments.length; i++)
            handles[i] = arguments[i].valueHandle;

        return NativeMethods.nJSFunctionCall(contextHandle, valueHandle,
                thisObject != null ? thisObject.valueHandle : 0L, handles);
    }

    public JSObject callAsConstructor(JSValue... args) throws JSException
    {
        long[] handles = new long[args.length];
        for (int i = 0; i < args.length; i++)
            handles[i] = args[i].valueHandle;

        return NativeMethods.nJSFunctionCallAsConstructor(contextHandle, valueHandle, handles);
    }

    @FunctionalInterface
    public interface JSCallback
    {
        /**
         * @param context    the JS context of the calling frame
         * @param thisObject the JS {@code this} binding for this call
         * @param args       arguments passed from JavaScript
         * @return the return value to pass back to JavaScript; must not be null
         * @throws JSException to propagate a JS exception back to the calling script
         */
        JSValue invoke(JSContext context, JSObject thisObject, JSValue[] args) throws JSException;
    }

    private static final class NativeMethods
    {
        private static native JSFunction nJSFunctionCreate(long contextHandle, String name, JSCallback callback) throws JSException;

        private static native JSValue nJSFunctionCall(long contextHandle, long functionHandle, long thisHandle, long[] argHandles) throws JSException;

        private static native JSObject nJSFunctionCallAsConstructor(long contextHandle, long functionHandle, long[] argHandles) throws JSException;
    }
}
