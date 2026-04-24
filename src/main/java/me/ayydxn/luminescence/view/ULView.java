package me.ayydxn.luminescence.view;

import me.ayydxn.luminescence.events.ULKeyEvent;
import me.ayydxn.luminescence.events.ULMouseEvent;
import me.ayydxn.luminescence.events.ULScrollEvent;
import me.ayydxn.luminescence.renderer.ULRenderer;
import me.ayydxn.luminescence.renderer.ULSession;
import me.ayydxn.luminescence.surface.ULSurface;
import org.jetbrains.annotations.Nullable;

public class ULView
{
    private long handle;

    public ULView(ULRenderer renderer, int width, int height, ULViewConfig viewConfig, @Nullable ULSession session)
    {
        this.handle = NativeMethods.nulCreateView(renderer.getHandle(), width, height, viewConfig.getHandle(), session != null ? session.getHandle() : 0);
    }

    public void destroy()
    {
        if (this.handle != 0)
        {
            NativeMethods.nulDestroyView(this.handle);
            this.handle = 0;
        }
    }

    public String getURL()
    {
        return NativeMethods.nulViewGetURL(this.handle);
    }

    public String getTitle()
    {
        return NativeMethods.nulViewGetTitle(this.handle);
    }

    public int getWidth()
    {
        return NativeMethods.nulViewGetWidth(this.handle);
    }

    public int getHeight()
    {
        return NativeMethods.nulViewGetHeight(this.handle);
    }

    public int getDisplayID()
    {
        return NativeMethods.nulViewGetDisplayId(this.handle);
    }

    public void setDisplayID(int displayID)
    {
        NativeMethods.nulViewSetDisplayId(this.handle, displayID);
    }

    public double getDeviceScale()
    {
        return NativeMethods.nulViewGetDeviceScale(this.handle);
    }

    public void setDeviceScale(double deviceScale)
    {
        NativeMethods.nulViewSetDeviceScale(this.handle, deviceScale);
    }

    public boolean isAccelerated()
    {
        return NativeMethods.nulViewIsAccelerated(this.handle);
    }

    public boolean isTransparent()
    {
        return NativeMethods.nulViewIsTransparent(this.handle);
    }

    public boolean isLoading()
    {
        return NativeMethods.nulViewIsLoading(this.handle);
    }

    @Nullable
    public ULRenderTarget getRenderTarget()
    {
        if (!this.isAccelerated())
            return null;

        return NativeMethods.nulViewGetRenderTarget(this.handle);
    }

    public ULSurface getSurface()
    {
        return new ULSurface(NativeMethods.nulViewGetSurface(this.handle));
    }

    public void loadHTML(String html)
    {
        NativeMethods.nulViewLoadHTML(this.handle, html);
    }

    public void loadURL(String url)
    {
        NativeMethods.nulViewLoadURL(this.handle, url);
    }

    public void resize(int width, int height)
    {
        NativeMethods.nulViewResize(this.handle, width, height);
    }

    public long lockJSContext()
    {
        return NativeMethods.nulViewLockJSContext(this.handle);
    }

    public void unlockJSContext()
    {
        NativeMethods.nulViewUnlockJSContext(this.handle);
    }

    public JSContext acquireJSContextLock()
    {
        return new JSContext();
    }

    public String evaluateScript(String jsString, String[] exception)
    {
        return NativeMethods.nulViewEvaluateScript(this.handle, jsString, exception);
    }

    public boolean canGoBack()
    {
        return NativeMethods.nulViewCanGoBack(this.handle);
    }

    public boolean canGoForward()
    {
        return NativeMethods.nulViewCanGoForward(this.handle);
    }

    public void goBack()
    {
        NativeMethods.nulViewGoBack(this.handle);
    }

    public void goForward()
    {
        NativeMethods.nulViewGoForward(this.handle);
    }

    public void goToHistoryOffset(int offset)
    {
        NativeMethods.nulViewGoToHistoryOffset(this.handle, offset);
    }

    public void reload()
    {
        NativeMethods.nulViewReload(this.handle);
    }

    public void stop()
    {
        NativeMethods.nulViewStop(this.handle);
    }

    public void focus()
    {
        NativeMethods.nulViewFocus(this.handle);
    }

    public void unfocus()
    {
        NativeMethods.nulViewUnfocus(this.handle);
    }

    public boolean hasFocus()
    {
        return NativeMethods.nulViewHasFocus(this.handle);
    }

    public boolean hasInputFocus()
    {
        return NativeMethods.nulViewHasInputFocus(this.handle);
    }

    public void fireKeyEvent(ULKeyEvent keyEvent)
    {
        NativeMethods.nulViewFireKeyEvent(this.handle, keyEvent.getHandle());
    }

    public void fireMouseEvent(ULMouseEvent mouseEvent)
    {
        NativeMethods.nulViewFireMouseEvent(this.handle, mouseEvent.getHandle());
    }

    public void fireScrollEvent(ULScrollEvent scrollEvent)
    {
        NativeMethods.nulViewFireScrollEvent(this.handle, scrollEvent.getHandle());
    }

    public final class JSContext implements AutoCloseable
    {
        private final long handle;

        public JSContext()
        {
            this.handle = ULView.this.lockJSContext();
        }

        public long context()
        {
            return this.handle;
        }

        @Override
        public void close() throws Exception
        {
            ULView.this.unlockJSContext();
        }
    }

    private static final class NativeMethods
    {
        private static native long nulCreateView(long rendererHandle, int width, int height, long viewConfigHandle, long sessionHandle);

        private static native void nulDestroyView(long handle);

        public static native String nulViewGetURL(long handle);

        public static native String nulViewGetTitle(long handle);

        public static native int nulViewGetWidth(long handle);

        public static native int nulViewGetHeight(long handle);

        public static native int nulViewGetDisplayId(long handle);

        public static native void nulViewSetDisplayId(long handle, int displayId);

        public static native double nulViewGetDeviceScale(long handle);

        public static native void nulViewSetDeviceScale(long handle, double scale);

        public static native boolean nulViewIsAccelerated(long handle);

        public static native boolean nulViewIsTransparent(long handle);

        public static native boolean nulViewIsLoading(long handle);

        public static native ULRenderTarget nulViewGetRenderTarget(long handle);

        public static native long nulViewGetSurface(long handle);

        public static native void nulViewLoadHTML(long handle, String htmlString);

        public static native void nulViewLoadURL(long handle, String urlString);

        public static native void nulViewResize(long handle, int width, int height);

        public static native long nulViewLockJSContext(long handle);

        public static native void nulViewUnlockJSContext(long handle);

        public static native String nulViewEvaluateScript(long handle, String jsString, String[] exception);

        public static native boolean nulViewCanGoBack(long handle);

        public static native boolean nulViewCanGoForward(long handle);

        public static native void nulViewGoBack(long handle);

        public static native void nulViewGoForward(long handle);

        public static native void nulViewGoToHistoryOffset(long handle, int offset);

        public static native void nulViewReload(long handle);

        public static native void nulViewStop(long handle);

        public static native void nulViewFocus(long handle);

        public static native void nulViewUnfocus(long handle);

        public static native boolean nulViewHasFocus(long handle);

        public static native boolean nulViewHasInputFocus(long handle);

        public static native void nulViewFireKeyEvent(long handle, long keyEventHandle);

        public static native void nulViewFireMouseEvent(long handle, long mouseEventHandle);

        public static native void nulViewFireScrollEvent(long handle, long scrollEventHandle);
    }
}
