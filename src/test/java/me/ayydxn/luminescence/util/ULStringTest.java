package me.ayydxn.luminescence.util;

import me.ayydxn.luminescence.BaseLuminescenceTest;
import me.ayydxn.luminescence.internal.UltralightNativeLoader;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.*;

public class ULStringTest extends BaseLuminescenceTest
{
    @Test
    @DisplayName("Verify Basic Creation and Data Retrieval")
    void testBasicCreation()
    {
        String input = "Hello Luminescence!";
        try (ULString ulString = ULString.create(input))
        {
            assertEquals(input, ulString.getData(), "Native string data should match Java input");
            assertFalse(ulString.isEmpty());
            assertEquals(input.length(), ulString.getLength());
        }
    }

    @Test
    @DisplayName("Verify Empty String Behavior")
    void testEmptyString()
    {
        try (ULString ulString = ULString.create(""))
        {
            assertTrue(ulString.isEmpty());
            assertEquals(0, ulString.getLength());
            assertEquals("", ulString.getData());
        }
    }

    @Test
    @DisplayName("Verify Assignment Logic (Copying Contents)")
    void testAssignment()
    {
        try (ULString baseString = ULString.create("Initial"); ULString updatedString = ULString.create("New Content"))
        {
            baseString.assignString(updatedString);
            assertEquals("New Content", baseString.getData(), "Base string should now contain updated content");

            baseString.assignCString("Direct Assignment");
            assertEquals("Direct Assignment", baseString.getData());
        }
    }

    @Test
    @DisplayName("Verify createFromCopy Creates A Distinct Object")
    void testCopyConstructor()
    {
        try (ULString originalString = ULString.create("Original"))
        {
            try (ULString copyString = ULString.createFromCopy(originalString))
            {
                assertEquals(originalString.getData(), copyString.getData());
                assertNotEquals(originalString.getHandle(), copyString.getHandle(), "Copy should have a unique native handle");
            }
        }
    }

    @Test
    @DisplayName("Verify Handle Nullification After Close")
    void testCloseSafety()
    {
        ULString ulString = ULString.create("Temporary");
        long capturedHandle = ulString.getHandle();
        assertNotEquals(0, capturedHandle);

        ulString.close();
        assertEquals(0, ulString.getHandle(), "Handle must be zeroed out after close to prevent use-after-free");

        // Ensure double-close doesn't crash
        assertDoesNotThrow(ulString::close);
    }

    @Test
    @DisplayName("Verify Unicode Round-Trip (UTF-16 Path)")
    void testUnicodeRoundTrip()
    {
        String input = "こんにちは 🌐 مرحبا";

        try (ULString ulString = ULString.createUTF16(input))
        {
            assertEquals(input, ulString.getData());
            assertEquals(input.length(), ulString.getLength());
        }
    }

    @Test
    @DisplayName("Verify Null Input Is Rejected Cleanly With IllegalArgumentException")
    @SuppressWarnings("resource")
    void testNullRejection()
    {
        assertThrows(IllegalArgumentException.class, () -> ULString.create(null));
    }
}
