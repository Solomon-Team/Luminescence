package me.ayydxn.luminescence.renderer;

import java.nio.file.Path;

public class ULSession
{
    private long handle;

    private ULSession(long handle)
    {
        this.handle = handle;
    }

    public static ULSession create(ULRenderer renderer, boolean isPersistent, String name)
    {
        long handle = NativeMethods.nulCreateSession(renderer.getHandle(), isPersistent, name);

        return new ULSession(handle);
    }

    public static ULSession createDefault(ULRenderer renderer)
    {
        long handle = NativeMethods.nulDefaultSession(renderer.getHandle());

        return new ULSession(handle);
    }

    public void destroy()
    {
        if (this.handle != 0)
        {
            NativeMethods.nulDestroySession(this.handle);
            this.handle = 0;
        }
    }

    public boolean isPersistent()
    {
        return NativeMethods.nulSessionIsPersistent(this.handle);
    }

    public String getName()
    {
        return NativeMethods.nulSessionGetName(this.handle);
    }

    public long getID()
    {
        return NativeMethods.nulSessionGetID(this.handle);
    }

    public Path getDiskPath()
    {
        String diskPathString = NativeMethods.nulSessionGetDiskPath(this.handle);

        return Path.of(diskPathString);
    }

    private static final class NativeMethods
    {
        private static native long nulCreateSession(long rendererHandle, boolean isPersistent, String name);

        private static native void nulDestroySession(long rendererHandle);

        private static native long nulDefaultSession(long rendererHandle);

        private static native boolean nulSessionIsPersistent(long handle);

        private static native String nulSessionGetName(long handle);

        private static native long nulSessionGetID(long handle);

        private static native String nulSessionGetDiskPath(long handle);
    }
}
