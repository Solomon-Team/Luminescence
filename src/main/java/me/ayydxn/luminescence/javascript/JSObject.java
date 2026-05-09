package me.ayydxn.luminescence.javascript;

import me.ayydxn.luminescence.util.UsedByNative;

public class JSObject extends JSValue
{
    @UsedByNative("JSObject.cpp")
    JSObject(long contextHandle, long valueHandle)
    {
        super(contextHandle, valueHandle);
    }

    public JSValue getProperty(String name) throws JSException
    {
        return NativeMethods.nJSObjectGetProperty(contextHandle, valueHandle, name);
    }

    public void setProperty(String name, JSValue value) throws JSException
    {
        NativeMethods.nJSObjectSetProperty(contextHandle, valueHandle, name, value.valueHandle);
    }

    public boolean hasProperty(String name)
    {
        return NativeMethods.nJSObjectHasProperty(contextHandle, valueHandle, name);
    }

    public boolean deleteProperty(String name) throws JSException
    {
        return NativeMethods.nJSObjectDeleteProperty(contextHandle, valueHandle, name);
    }

    public JSValue getPropertyAtIndex(int index) throws JSException
    {
        return NativeMethods.nJSObjectGetPropertyAtIndex(contextHandle, valueHandle, index);
    }

    public void setPropertyAtIndex(int index, JSValue value) throws JSException
    {
        NativeMethods.nJSObjectSetPropertyAtIndex(contextHandle, valueHandle, index, value.valueHandle);
    }

    public String[] getPropertyNames()
    {
        return NativeMethods.nJSObjectGetPropertyNames(contextHandle, valueHandle);
    }

    private static final class NativeMethods
    {
        private static native JSValue nJSObjectGetProperty(long contextHandle, long object, String name) throws JSException;

        private static native void nJSObjectSetProperty(long contextHandle, long object, String name, long value) throws JSException;

        private static native boolean nJSObjectHasProperty(long contextHandle, long object, String name);

        private static native boolean nJSObjectDeleteProperty(long contextHandle, long object, String name) throws JSException;

        private static native JSValue nJSObjectGetPropertyAtIndex(long contextHandle, long object, int index) throws JSException;

        private static native void nJSObjectSetPropertyAtIndex(long contextHandle, long object, int index, long value) throws JSException;

        private static native String[] nJSObjectGetPropertyNames(long contextHandle, long object);
    }
}
