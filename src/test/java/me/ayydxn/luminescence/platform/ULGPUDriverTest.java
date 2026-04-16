package me.ayydxn.luminescence.platform;

import me.ayydxn.luminescence.BaseLuminescenceTest;
import me.ayydxn.luminescence.bitmap.BitmapFormat;
import me.ayydxn.luminescence.gpu.*;
import me.ayydxn.luminescence.bitmap.ULBitmap;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;
import org.mockito.ArgumentCaptor;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.*;

public class ULGPUDriverTest extends BaseLuminescenceTest
{
    private ULGPUDriver mockDriver;

    @BeforeEach
    void setUp()
    {
        mockDriver = mock(ULGPUDriver.class);
    }

    @Test
    @DisplayName("Verify Texture Lifecycle Interaction")
    void testTextureLifecycle()
    {
        int textureId = 1;
        ULBitmap mockBitmap = ULBitmap.create(1, 1, BitmapFormat.BGRA8_UNORM_SRGB);

        try (ULBitmap.LockedPixels lockedPixels = mockBitmap.acquirePixelLock())
        {
            lockedPixels.pixels().put((byte) 1);
            lockedPixels.pixels().put((byte) 1);
            lockedPixels.pixels().put((byte) 1);
            lockedPixels.pixels().put((byte) 1);
        }

        mockDriver.createTexture(textureId, mockBitmap);
        mockDriver.destroyTexture(textureId);

        verify(mockDriver, times(1)).createTexture(eq(textureId), any(ULBitmap.class));
        verify(mockDriver, times(1)).destroyTexture(textureId);
    }

    @Test
    @DisplayName("Verify Geometry Data and Format Mapping")
    void testGeometryCreation()
    {
        int geometryId = 42;
        ByteBuffer vertexData = ByteBuffer.allocateDirect(1024).order(ByteOrder.nativeOrder());
        ByteBuffer indexData = ByteBuffer.allocateDirect(512).order(ByteOrder.nativeOrder());

        ULVertexBuffer vertices = new ULVertexBuffer(ULVertexBuffer.Format.QUAD, vertexData);
        ULIndexBuffer indices = new ULIndexBuffer(indexData);

        mockDriver.createGeometry(geometryId, vertices, indices);

        ArgumentCaptor<ULVertexBuffer> vertexCaptor = ArgumentCaptor.forClass(ULVertexBuffer.class);
        verify(mockDriver).createGeometry(eq(geometryId), vertexCaptor.capture(), any());

        assertEquals(ULVertexBuffer.Format.QUAD, vertexCaptor.getValue().getFormat());
        assertEquals(vertexData, vertexCaptor.getValue().getData());
    }

    @Test
    @DisplayName("Verify GPU State Integrity (Command List)")
    void testUpdateCommandListState()
    {
        ArgumentCaptor<ULCommand[]> commandCaptor = ArgumentCaptor.forClass(ULCommand[].class);

        ULCommand command = new ULCommand();
        command.type = ULCommand.Type.DRAW_GEOMETRY;
        command.gpuState = new ULGPUState();
        command.gpuState.shaderType = ULShaderType.FILL;
        command.gpuState.viewportWidth = 1920;
        command.gpuState.viewportHeight = 1080;
        command.gpuState.transform = new float[16];
        command.gpuState.transform[0] = 1.0f;

        ULCommand[] commands = { command };
        mockDriver.updateCommandList(commands);

        verify(mockDriver).updateCommandList(commandCaptor.capture());

        ULCommand captured = commandCaptor.getValue()[0];
        assertNotNull(captured.gpuState);
        assertEquals(ULShaderType.FILL, captured.gpuState.shaderType);
        assertEquals(1920, captured.gpuState.viewportWidth);
        assertEquals(1.0f, captured.gpuState.transform[0]);
    }
}
