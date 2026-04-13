package me.ayydxn.luminescence.view;

public class ULViewConfig implements AutoCloseable
{
    private long handle;

    public ULViewConfig()
    {
        this.handle = NativeMethods.nulCreateViewConfig();
    }

    @Override
    public void close()
    {
        if (this.handle != 0)
        {
            NativeMethods.nulDestroyViewConfig(this.handle);
            this.handle = 0;
        }
    }

    public void setDisplayID(int displayID)
    {
        NativeMethods.nulViewConfigSetDisplayID(this.handle, displayID);
    }

    public void isAccelerated(boolean isAccelerated)
    {
        NativeMethods.nulViewConfigIsAccelerated(this.handle, isAccelerated);
    }

    public void setTransparent(boolean isTransparent)
    {
        NativeMethods.nulViewConfigSetIsTransparent(this.handle, isTransparent);
    }

    public void setInitialDeviceScale(double initialDeviceScale)
    {
        NativeMethods.nulViewConfigSetInitialDeviceScale(this.handle, initialDeviceScale);
    }

    public void setInitialFocus(boolean isFocused)
    {
        NativeMethods.nulViewConfigSetInitialFocus(this.handle, isFocused);
    }

    public void setEnableImages(boolean enabled)
    {
        NativeMethods.nulViewConfigSetEnableImages(this.handle, enabled);
    }

    public void setEnableJavaScript(boolean enabled)
    {
        NativeMethods.nulViewConfigSetEnableJavaScript(this.handle, enabled);
    }

    public void setFontFamilyStandard(String fontName)
    {
        NativeMethods.nulViewConfigSetFontFamilyStandard(this.handle, fontName);
    }

    public void setFontFamilyFixed(String fontName)
    {
        NativeMethods.nulViewConfigSetFontFamilyFixed(this.handle, fontName);
    }

    public void setFontFamilySansSerif(String fontName)
    {
        NativeMethods.nulViewConfigSetFontFamilySansSerif(this.handle, fontName);
    }

    public void setUserAgent(String userAgent)
    {
        NativeMethods.nulViewConfigSetUserAgent(this.handle, userAgent);
    }

    private static final class NativeMethods
    {
        private static native long nulCreateViewConfig();

        private static native void nulDestroyViewConfig(long handle);

        private static native void nulViewConfigSetDisplayID(long handle, int displayID);

        private static native void nulViewConfigIsAccelerated(long handle, boolean isAccelerated);

        private static native void nulViewConfigSetIsTransparent(long handle, boolean isTransparent);

        private static native void nulViewConfigSetInitialDeviceScale(long handle, double initialDeviceScale);

        private static native void nulViewConfigSetInitialFocus(long handle, boolean isFocused);

        private static native void nulViewConfigSetEnableImages(long handle, boolean enabled);

        private static native void nulViewConfigSetEnableJavaScript(long handle, boolean enabled);

        private static native void nulViewConfigSetFontFamilyStandard(long handle, String fontName);

        private static native void nulViewConfigSetFontFamilyFixed(long handle, String fontName);

        private static native void nulViewConfigSetFontFamilySansSerif(long handle, String fontName);

        private static native void nulViewConfigSetUserAgent(long handle, String agentString);
    }
}
