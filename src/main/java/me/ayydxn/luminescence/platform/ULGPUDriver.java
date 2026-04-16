package me.ayydxn.luminescence.platform;

import me.ayydxn.luminescence.bitmap.ULBitmap;
import me.ayydxn.luminescence.gpu.ULCommand;
import me.ayydxn.luminescence.gpu.ULIndexBuffer;
import me.ayydxn.luminescence.gpu.ULRenderBuffer;
import me.ayydxn.luminescence.gpu.ULVertexBuffer;
import me.ayydxn.luminescence.util.UsedByNative;

public interface ULGPUDriver
{
    void beginSynchronize();

    void endSynchronize();

    @UsedByNative("GPUDriverCallbackAdapter.cpp")
    int nextTextureId();

    void createTexture(int textureId, ULBitmap bitmap);

    void updateTexture(int textureId, ULBitmap bitmap);

    void destroyTexture(int textureId);

    @UsedByNative("GPUDriverCallbackAdapter.cpp")
    int nextRenderBufferId();

    void createRenderBuffer(int renderBufferId, ULRenderBuffer buffer);

    void destroyRenderBuffer(int renderBufferId);

    @UsedByNative("GPUDriverCallbackAdapter.cpp")
    int nextGeometryId();

    void createGeometry(int geometryId, ULVertexBuffer vertices, ULIndexBuffer indices);

    void updateGeometry(int geometryId, ULVertexBuffer vertices, ULIndexBuffer indices);

    void destroyGeometry(int geometryId);

    void updateCommandList(ULCommand[] commands);
}
