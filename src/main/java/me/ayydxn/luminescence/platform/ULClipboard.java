package me.ayydxn.luminescence.platform;

/**
 * The library uses this to read and write data to the system's clipboard.
 *
 * @author Ayydxn
 */
public interface ULClipboard
{
    /**
     * The callback invoked when the library wants to clear the system's clipboard.
     */
    void clear();

    /**
     * The callback invoked when the library wants to read from the system's clipboard.
     */
    String readPlainText();

    /**
     * The callback invoked when the library wants to write to the system's clipboard.
     */
    void writePlainText(String text);
}
