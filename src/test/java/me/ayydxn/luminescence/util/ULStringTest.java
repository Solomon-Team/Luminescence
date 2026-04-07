package me.ayydxn.luminescence.util;

import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.*;

public class ULStringTest
{
    @Test
    @DisplayName("Verify Basic Creation and Data Retrieval")
    void testBasicCreation()
    {
        String input = "Hello Luminescence!";
        try (ULString ulStr = ULString.create(input))
        {
            assertEquals(input, ulStr.getData(), "Native string data should match Java input");
            assertFalse(ulStr.isEmpty());
            assertEquals(input.length(), ulStr.getLength());
        }
    }

    @Test
    @DisplayName("Verify Empty String Behavior")
    void testEmptyString()
    {
        try (ULString ulStr = ULString.create(""))
        {
            assertTrue(ulStr.isEmpty());
            assertEquals(0, ulStr.getLength());
            assertEquals("", ulStr.getData());
        }
    }

    @Test
    @DisplayName("Verify Assignment Logic (Copying Contents)")
    void testAssignment()
    {
        try (ULString base = ULString.create("Initial"); ULString updated = ULString.create("New Content"))
        {

            base.assignString(updated);
            assertEquals("New Content", base.getData(), "Base string should now contain updated content");

            base.assignCString("Direct Assignment");
            assertEquals("Direct Assignment", base.getData());
        }
    }

    @Test
    @DisplayName("Verify createFromCopy Creates A Distinct Object")
    void testCopyConstructor()
    {
        try (ULString original = ULString.create("Original"))
        {
            try (ULString copy = ULString.createFromCopy(original))
            {
                assertEquals(original.getData(), copy.getData());
                assertNotEquals(original.getHandle(), copy.getHandle(), "Copy should have a unique native handle");
            }
        }
    }

    @Test
    @DisplayName("Verify Handle Nullification After Close")
    void testCloseSafety()
    {
        ULString ulStr = ULString.create("Temporary");
        long capturedHandle = ulStr.getHandle();
        assertNotEquals(0, capturedHandle);

        ulStr.close();
        assertEquals(0, ulStr.getHandle(), "Handle must be zeroed out after close to prevent use-after-free");

        // Ensure double-close doesn't crash (should be handled by your nulDestroyString check)
        assertDoesNotThrow(ulStr::close);
    }

    @Test
    @DisplayName("Verify Unicode Round-Trip (UTF-16 Path)")
    void testUnicodeRoundTrip()
    {
        // Exercises the GetStringChars / ulCreateStringUTF16 path in jni_utils.hpp
        // rather than the ASCII fast path
        String input = "こんにちは 🌐 مرحبا";
        try (ULString ulStr = ULString.create(input))
        {
            assertEquals(input, ulStr.getData());
            assertEquals(input.length(), ulStr.getLength());
        }
    }

    @Test
    @DisplayName("Verify Null Input Is Rejected Cleanly")
    void testNullRejection()
    {
        assertThrows(NullPointerException.class, () -> ULString.create(null));
    }
}
