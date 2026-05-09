package me.ayydxn.luminescence.javascript;

import org.jetbrains.annotations.ApiStatus;

public class JSContext implements AutoCloseable
{
    private final long viewHandle;

    final long handle;

    @ApiStatus.Internal
    public JSContext(long viewHandle, long handle)
    {
        this.viewHandle = viewHandle;
        this.handle = handle;
    }

    @Override
    public void close()
    {
        NativeMethods.nJSContextUnlock(viewHandle);
    }

    public JSValue make(double value)
    {
        return NativeMethods.nJSContextMakeNumber(handle, value);
    }

    public JSValue make(String value)
    {
        return NativeMethods.nJSContextMakeString(handle, value);
    }

    public JSValue make(boolean value)
    {
        return NativeMethods.nJSContextMakeBoolean(handle, value);
    }

    public JSValue makeUndefined()
    {
        return NativeMethods.nJSContextMakeUndefined(handle);
    }

    public JSValue makeNull()
    {
        return NativeMethods.nJSContextMakeNull(handle);
    }

    public JSObject globalObject()
    {
        return NativeMethods.nJSContextGetGlobalObject(handle);
    }

    public JSValue evaluate(String script) throws JSException
    {
        return NativeMethods.nJSContextEvaluate(handle, script, null, 0);
    }

    public JSValue evaluate(String script, String sourceURL, int startingLineNumber) throws JSException
    {
        return NativeMethods.nJSContextEvaluate(handle, script, sourceURL, startingLineNumber);
    }

    @ApiStatus.Internal
    public long getHandle()
    {
        return this.handle;
    }

    private static final class NativeMethods
    {
        private static native JSValue nJSContextMakeNumber(long contextHandle, double value);

        private static native JSValue nJSContextMakeString(long contextHandle, String value);

        private static native JSValue nJSContextMakeBoolean(long contextHandle, boolean value);

        private static native JSValue nJSContextMakeUndefined(long contextHandle);

        private static native JSValue nJSContextMakeNull(long contextHandle);

        private static native JSObject nJSContextGetGlobalObject(long contextHandle);

        private static native JSValue nJSContextEvaluate(long contextHandle, String script, String sourceURL, int line) throws JSException;

        private static native void nJSContextUnlock(long viewHandle);
    }
}