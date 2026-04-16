package me.ayydxn.luminescence.gpu;

import me.ayydxn.luminescence.geometry.IntRect;

public class ULGPUState
{
    public int viewportWidth;
    public int viewportHeight;
    public float[] transform; // 4x4 matrix, 16 floats, column-major
    public boolean enableTexturing;
    public boolean enableBlend;
    public ULShaderType shaderType;
    public int renderBufferId;
    public int texture1Id;
    public int texture2Id;
    public int texture3Id;
    public float[] uniformScalar; // 8 floats
    public float[] uniformVector; // array of 8 Vector4fs
    public int clipSize;
    public float[] clip; // 128 floats (8 × mat4x4)
    public boolean enableScissor;
    public IntRect scissorRect;
}
