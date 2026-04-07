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
}
