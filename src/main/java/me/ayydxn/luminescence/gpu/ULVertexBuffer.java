package me.ayydxn.luminescence.gpu;

import me.ayydxn.luminescence.util.NativeEnum;
import me.ayydxn.luminescence.util.UsedByNative;

import java.nio.ByteBuffer;

public class ULVertexBuffer
{
    private final Format format;
    private final ByteBuffer data;

    public ULVertexBuffer(Format format, ByteBuffer data)
    {
        this.format = format;
        this.data = data;
    }

    public Format getFormat()
    {
        return this.format;
    }

    public ByteBuffer getData()
    {
        return this.data;
    }

    public enum Format implements NativeEnum
    {
        /** Equivalent to kVertexBufferFormat_2f_4ub_2f */
        PATH(0),

        /** kVertexBufferFormat_2f_4ub_2f_2f_28f */
        QUAD(1);

        private final int value;

        Format(int value)
        {
            this.value = value;
        }

        @Override
        public int getValue()
        {
            return this.value;
        }

        @UsedByNative("GPUDriverCallbackAdapter.cpp")
        public static Format fromNativeValue(int value)
        {
            return NativeEnum.resolve(Format.class, value, PATH);
        }
    }
}
