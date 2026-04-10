package me.ayydxn.luminescence.platform;

import me.ayydxn.luminescence.util.NativeEnum;
import me.ayydxn.luminescence.util.UsedByNative;

/**
 * The library uses this to display log messages for debugging during development.
 * <p>
 * This is intended to be implemented by users and defined before creating the Renderer.
 *
 * @author Ayydxn
 */
public interface ULLogger
{
    /**
     * The callback invoked when the library wants to print a message to the log.
     */
    void logMessage(Level logLevel, String message);

    enum Level implements NativeEnum
    {
        ERROR(0),
        WARNING(1),
        INFO(2);

        private final int value;

        Level(int value)
        {
            this.value = value;
        }

        @Override
        public int getValue()
        {
            return this.value;
        }

        @UsedByNative("LoggerCallbackAdapter.cpp")
        public static Level fromNativeValue(int value)
        {
            return NativeEnum.resolve(Level.class, value, INFO);
        }
    }
}
