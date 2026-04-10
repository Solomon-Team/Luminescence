package me.ayydxn.luminescence.platform;

public interface ULFileSystem
{
    boolean fileExists(String path);

    /** Falls back to "application/unknown" */
    String getFileMimeType(String path);

    /** Falls back to utf-8 */
    String getFileCharset(String path);

    byte[] openFile(String path);
}
