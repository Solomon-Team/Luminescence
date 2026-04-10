package me.ayydxn.luminescence.font;

public class ULFontFile
{
    private final Source source;
    private final String filePath;
    private final byte[] buffer;

    private ULFontFile(String filePath)
    {
        this.source = Source.FILE_PATH;
        this.filePath = filePath;
        this.buffer = null;
    }

    private ULFontFile(byte[] buffer)
    {
        this.source = Source.BUFFER;
        this.filePath = null;
        this.buffer = buffer;
    }

    public static ULFontFile createFromFilePath(String path)
    {
        if (path == null || path.isBlank())
            throw new IllegalArgumentException("Font file path must not be null or blank.");

        return new ULFontFile(path);
    }

    public static ULFontFile createFromBuffer(byte[] data)
    {
        if (data == null || data.length == 0)
            throw new IllegalArgumentException("Font buffer must not be null or empty.");

        return new ULFontFile(data);
    }

    public Source getSource()
    {
        return this.source;
    }

    public String getFilePath()
    {
        return this.filePath;
    }

    public byte[] getBuffer()
    {
        return this.buffer;
    }

    public enum Source
    {
        FILE_PATH,
        BUFFER
    }

}
