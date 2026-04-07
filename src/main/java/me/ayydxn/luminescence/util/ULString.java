package me.ayydxn.luminescence.util;

import org.jetbrains.annotations.ApiStatus;

/**
 * This class is used to represent strings in Ultralight. It can be created from a variety of string types and converted to a number of Unicode string types.
 *
 * <h5>Accessing string data</h5>
 * <blockquote><pre>
 *     ULString string = ULString.create("Hello World!");
 *
 *     // Print the UTF-8 data (guaranteed to be null-terminated)
 *     System.out.println(string.getData());
 * </pre></blockquote>
 * @author Ayydxn
 */
public class ULString implements AutoCloseable
{
    private long handle;

    private ULString(long handle)
    {
        this.handle = handle;
    }

    public static ULString create(String string)
    {
        if (string == null)
            throw new IllegalArgumentException("Cannot create a ULString with a null Java string!");

        return new ULString(NativeMethods.nulCreateString(string));
    }

    public static ULString createUTF8(String string)
    {
        return new ULString(NativeMethods.nulCreateStringUTF8(string));
    }

    public static ULString createUTF16(String string)
    {
        return new ULString(NativeMethods.nulCreateStringUTF16(string));
    }

    public static ULString createFromCopy(ULString otherString)
    {
        return new ULString(NativeMethods.nulCreateStringFromCopy(otherString.handle));
    }

    @Override
    public void close()
    {
        if (this.handle != 0)
        {
            NativeMethods.nulDestroyString(handle);

            this.handle = 0;
        }
    }

    public String getData()
    {
        return NativeMethods.nulStringGetData(this.handle);
    }

    public long getLength()
    {
        return this.getData().length();
    }

    public long getByteLength()
    {
        return NativeMethods.nulStringGetLength(this.handle);
    }

    public boolean isEmpty()
    {
        return NativeMethods.nulStringIsEmpty(this.handle);
    }

    public void assignString(ULString otherString)
    {
        NativeMethods.nulStringAssignString(this.handle, otherString.handle);
    }

    public void assignCString(String string)
    {
        NativeMethods.nulAssignCString(this.handle, string);
    }

    @ApiStatus.Internal
    public long getHandle()
    {
        return this.handle;
    }

    private static final class NativeMethods
    {
        private static native long nulCreateString(String string);
        private static native long nulCreateStringUTF8(String string);
        private static native long nulCreateStringUTF16(String string);
        private static native long nulCreateStringFromCopy(long otherHandle);
        private static native void nulDestroyString(long handle);
        private static native String nulStringGetData(long handle);
        private static native long nulStringGetLength(long handle);
        private static native boolean nulStringIsEmpty(long handle);
        private static native void nulStringAssignString(long handle, long newStringHandle);
        private static native void nulAssignCString(long handle, String string);
    }
}
