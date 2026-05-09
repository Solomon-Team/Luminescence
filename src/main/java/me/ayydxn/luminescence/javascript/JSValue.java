package me.ayydxn.luminescence.javascript;

import me.ayydxn.luminescence.util.UsedByNative;

public class JSValue
{
    @UsedByNative("JSValue.cpp")
    final long contextHandle;

    @UsedByNative("JSValue.cpp")
    final long valueHandle;

    JSValue(long contextHandle, long valueHandle)
    {
        this.contextHandle = contextHandle;
        this.valueHandle = valueHandle;
    }

    public JSType getType()
    {
        return NativeMethods.nJSValueGetType(contextHandle, valueHandle);
    }

    public boolean isUndefined()
    {
        return this.getType() == JSType.UNDEFINED;
    }

    public boolean isNull()
    {
        return this.getType() == JSType.NULL;
    }

    public boolean isBoolean()
    {
        return this.getType() == JSType.BOOLEAN;
    }

    public boolean isNumber()
    {
        return this.getType() == JSType.NUMBER;
    }

    public boolean isString()
    {
        return this.getType() == JSType.STRING;
    }

    public boolean isObject()
    {
        return NativeMethods.nJSValueIsObject(contextHandle, valueHandle);
    }

    public boolean isArray()
    {
        return NativeMethods.nJSValueIsArray(contextHandle, valueHandle);
    }

    public boolean isFunction()
    {
        return NativeMethods.nJSValueIsFunction(contextHandle, valueHandle);
    }

    public boolean toBoolean()
    {
        return NativeMethods.nJSValueToBoolean(contextHandle, valueHandle);
    }

    public double toNumber() throws JSException
    {
        return NativeMethods.nJSValueToNumber(contextHandle, valueHandle);
    }

    public String toJSONString(int indent) throws JSException
    {
        return NativeMethods.nJSValueToJSONString(contextHandle, valueHandle, indent);
    }

    @Override
    public String toString()
    {
        return NativeMethods.nJSValueToString(contextHandle, valueHandle);
    }

    private static final class NativeMethods
    {
        private static native JSType nJSValueGetType(long contextHandle, long valueHandle);

        private static native boolean nJSValueIsObject(long contextHandle, long valueHandle);

        private static native boolean nJSValueIsArray(long contextHandle, long valueHandle);

        private static native boolean nJSValueIsFunction(long contextHandle, long valueHandle);

        private static native boolean nJSValueToBoolean(long contextHandle, long valueHandle);

        private static native double nJSValueToNumber(long contextHandle, long valueHandle) throws JSException;

        private static native String nJSValueToJSONString(long contextHandle, long valueHandle, int indent) throws JSException;

        private static native String nJSValueToString(long contextHandle, long valueHandle);
    }
}
