package me.ayydxn.luminescence.gpu;

import me.ayydxn.luminescence.util.NativeEnum;

public class ULCommand
{
    public Type type;
    public ULGPUState gpuState;
    public int geometryID;
    public int indicesCount;
    public int indicesOffset;

    public enum Type implements NativeEnum
    {
        CLEAR_RENDER_BUFFER(0),
        DRAW_GEOMETRY(1);

        private final int value;

        Type(int value)
        {
            this.value = value;
        }

        @Override
        public int getValue()
        {
            return this.value;
        }
    }
}
