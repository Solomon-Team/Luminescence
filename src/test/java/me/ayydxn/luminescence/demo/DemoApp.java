package me.ayydxn.luminescence.demo;

import me.ayydxn.luminescence.Ultralight;
import me.ayydxn.luminescence.internal.UltralightNativeLoader;

public class DemoApp
{
    public static void main(String[] args)
    {
        // Always load the native libraries first
        UltralightNativeLoader.load();

        /* -- Display Ultralight and WebKit version information --*/
        System.out.printf("Ultralight Version: %s\n", Ultralight.ulVersionString());
        System.out.printf("Ultralight Version Major: %d\n", Ultralight.ulVersionMajor());
        System.out.printf("Ultralight Version Minor: %d\n", Ultralight.ulVersionMinor());
        System.out.printf("Ultralight Version Patch: %d\n", Ultralight.ulVersionPatch());

        System.out.println("--------------------------------");

        System.out.printf("WebKit Version: %s\n", Ultralight.ulWebKitVersionString());
        System.out.printf("WebKit Version Major: %d\n", Ultralight.ulWebKitVersionMajor());
        System.out.printf("WebKit Version Minor: %d\n", Ultralight.ulWebKitVersionMinor());
        System.out.printf("WebKit Version Tiny: %d\n", Ultralight.ulWebKitVersionTiny());
        System.out.printf("WebKit Version Micro: %d\n", Ultralight.ulWebKitVersionMicro());
        System.out.printf("WebKit Version Nano: %d\n", Ultralight.ulWebKitVersionNano());

        /* -- TODO: (Ayydxn) Once implemented, setup and run a basic application using Ultralight's AppCore API -- */
    }
}
