package me.ayydxn.luminescence.util;

import me.ayydxn.luminescence.BaseLuminescenceTest;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;

import java.nio.ByteBuffer;
import java.nio.charset.StandardCharsets;

import static org.junit.jupiter.api.Assertions.*;

public class ULBufferTest extends BaseLuminescenceTest
{
    @Test
    @DisplayName("Create from Copy: Verify Data Integrity And Ownership")
    void testCreateFromCopy()
    {
        byte[] originalBufferData = "Luminescence Engine".getBytes(StandardCharsets.UTF_8);

        try (ULBuffer buffer = ULBuffer.createFromCopy(originalBufferData))
        {
            assertEquals(originalBufferData.length, buffer.getSize());
            assertTrue(buffer.ownsData(), "Buffer created from copy should own its data");

            byte[] returnedData = buffer.toByteArray();
            assertArrayEquals(originalBufferData, returnedData);
        }
    }

    @Test
    @DisplayName("Create from Direct: Verify Shared Memory Access")
    void testCreateFromDirect()
    {
        String bufferContents = "Shared Memory Test";
        byte[] bytes = bufferContents.getBytes(StandardCharsets.UTF_8);

        ByteBuffer directBuffer = ByteBuffer.allocateDirect(bytes.length);
        directBuffer.put(bytes);
        directBuffer.flip();

        try (ULBuffer buffer = ULBuffer.create(directBuffer))
        {
            assertEquals(bytes.length, buffer.getSize());
            assertFalse(buffer.ownsData(), "Direct buffer should NOT own the data (Java owns it)");

            assertArrayEquals(bytes, buffer.toByteArray());
        }
    }

    @Test
    @DisplayName("Handles Null And Non-Direct Buffers")
    @SuppressWarnings("resource")
    void testValidation()
    {
        assertThrows(IllegalArgumentException.class, () -> ULBuffer.create(null));

        ByteBuffer heapBuffer = ByteBuffer.allocate(10);
        assertThrows(IllegalArgumentException.class, () -> ULBuffer.create(heapBuffer));

        assertThrows(IllegalArgumentException.class, () -> ULBuffer.createFromCopy(null));
    }

    @Test
    @DisplayName("Ensure Handle Is Zeroed After Close")
    void testClose()
    {
        ULBuffer buffer = ULBuffer.createFromCopy(new byte[]{1, 2, 3});
        assertNotEquals(0, buffer.getHandle());

        buffer.close();
        assertEquals(0, buffer.getHandle());

        // Ensure double close doesn't crash
        assertDoesNotThrow(buffer::close);
    }
}
