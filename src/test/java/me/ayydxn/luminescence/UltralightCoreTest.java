package me.ayydxn.luminescence;

import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;

public class UltralightCoreTest extends BaseLuminescenceTest
{
    @Test
    @DisplayName("Ultralight and WebKit Version Information are Valid")
    void testUltralightAndWebKitVersionInformation()
    {
        // Validate Ultralight semantic versioning
        String ultralightVersion = Ultralight.ulVersionString();
        Assertions.assertNotNull(ultralightVersion, "Ultralight version string should not be null");
        Assertions.assertTrue(ultralightVersion.matches("\\d+\\.\\d+\\.\\d+"), "Ultralight version string format should be X.Y.Z, but was: " +
                ultralightVersion);

        // Verify individual integer components match the string
        String expectedUltralightVersionString = String.format("%d.%d.%d", Ultralight.ulVersionMajor(), Ultralight.ulVersionMinor(), Ultralight.ulVersionPatch());
        Assertions.assertEquals(expectedUltralightVersionString, ultralightVersion, "Integer version components should match the version string");

        // Validate WebKit Versioning
        String wkVersion = Ultralight.ulWebKitVersionString();
        Assertions.assertNotNull(wkVersion, "WebKit version string should not be null");

        // Ensure the major version is a reasonable WebKit baseline (e.g., > 600 for modern Ultralight)
        Assertions.assertTrue(Ultralight.ulWebKitVersionMajor() >= 600, "WebKit major version should be a modern baseline (>= 600)");

        // Verify all granular version components are non-negative
        Assertions.assertTrue(Ultralight.ulWebKitVersionMajor() >= 0);
        Assertions.assertTrue(Ultralight.ulWebKitVersionMinor() >= 0);
        Assertions.assertTrue(Ultralight.ulWebKitVersionTiny() >= 0);
        Assertions.assertTrue(Ultralight.ulWebKitVersionMicro() >= 0);
        Assertions.assertTrue(Ultralight.ulWebKitVersionNano() >= 0);
    }
}
