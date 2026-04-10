package me.ayydxn.luminescence.util;

import org.jetbrains.annotations.ApiStatus;

/**
 * An interface for Java enums that map directly to native Ultralight C/C++ enum values.
 * <p>
 * Using this interface instead of {@link Enum#ordinal()} ensures that the binding remains stable
 * even if the order of enum constants is changed in Java or if Ultralight defines non-sequential integer values.
 * <p>
 * <h3>Example Usage:</h3>
 * <blockquote><pre>
 * public enum SomeUltralightEnum implements NativeEnum
 * {
 *     MY_FLAG(0),
 *     MY_OTHER_FLAG(1);
 *
 *     private final int value;
 *
 *     SomeUltralightEnum(int value) { this.value = value; }
 *
 *    {@literal @}Override
 *      public int getValue() { return value; }
 * }
 * </pre></blockquote>
 *
 * @author Ayydxn
 */
@ApiStatus.Internal
public interface NativeEnum
{
    int getValue();

    /**
     * Resolves a native integer value to its corresponding Java Enum constant.
     *
     * @param <E>          The type of the Enum implementing NativeEnum.
     * @param enumClass    The Class object of the enum to search.
     * @param nativeValue  The integer value received from the C++ layer.
     * @param defaultValue The fallback constant if the native value is unrecognized.
     * @return The matched Enum constant, or the defaultValue if no match is found.
     *
     * @apiNote This is a general-purpose resolver intended for use by JNI trampolines when implementing callbacks.
     */
    static <E extends Enum<E> & NativeEnum> E resolve(Class<E> enumClass, int nativeValue, E defaultValue)
    {
        for (E enumConstant : enumClass.getEnumConstants())
        {
            if (enumConstant.getValue() == nativeValue)
                return enumConstant;
        }

        return defaultValue;
    }
}
