package me.ayydxn.luminescence.util;

import java.lang.annotation.ElementType;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;

/**
 * Annotation used to indicate that a method/field is accessed via JNI.
 * <p>
 * This prevents minifiers from stripping it.
 */
@Retention(RetentionPolicy.CLASS)
@Target({ ElementType.METHOD, ElementType.FIELD, ElementType.TYPE, ElementType.CONSTRUCTOR })
public @interface UsedByNative
{
    String value() default "";
}
