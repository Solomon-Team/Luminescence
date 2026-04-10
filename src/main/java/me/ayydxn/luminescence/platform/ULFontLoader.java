package me.ayydxn.luminescence.platform;

import me.ayydxn.luminescence.font.ULFontFile;
import org.jetbrains.annotations.Nullable;

public interface ULFontLoader
{
    String getFallbackFont();

    String getFallbackFontForCharacters(String characters, int weight, boolean isItalic);

    @Nullable
    ULFontFile load(String family, int weight, boolean isItalic);
}
