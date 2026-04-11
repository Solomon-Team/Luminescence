package me.ayydxn.luminescence.platform.impl;

import me.ayydxn.luminescence.font.ULFontFile;
import me.ayydxn.luminescence.platform.ULFontLoader;
import org.jetbrains.annotations.Nullable;

import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.List;

public class StandardULFontLoader implements ULFontLoader
{
    private final List<Path> fontSearchPaths = new ArrayList<>();
    private final String fallback;

    public StandardULFontLoader()
    {
        this("Arial");
    }

    public StandardULFontLoader(String defaultFontFamily)
    {
        this.fallback = defaultFontFamily;

        this.setupSystemFontPaths();
    }

    @Override
    public String getFallbackFont()
    {
        return this.fallback;
    }

    @Override
    public String getFallbackFontForCharacters(String characters, int weight, boolean isItalic)
    {
        return this.fallback;
    }

    @Override
    public @Nullable ULFontFile load(String family, int weight, boolean isItalic)
    {
        // Basic naming convention search (e.g., Arial_Bold.ttf)
        String[] variations = {
                family + ".ttf",
                family + ".otf",
                family + (weight >= 700 ? "bd" : "") + (isItalic ? "i" : "") + ".ttf"
        };

        for (Path searchRoot : fontSearchPaths)
        {
            for (String fileName : variations)
            {
                Path fontPath = searchRoot.resolve(fileName);

                if (Files.exists(fontPath))
                    return ULFontFile.createFromFilePath(fontPath.toAbsolutePath().toString());
            }
        }

        return null;
    }

    public void addFontSearchPath(Path searchPath)
    {
        this.fontSearchPaths.add(searchPath);
    }

    private void setupSystemFontPaths()
    {
        String os = System.getProperty("os.name").toLowerCase();

        if (os.contains("win"))
        {
            fontSearchPaths.add(Path.of(System.getenv("SystemRoot"), "Fonts"));
        }
        else if (os.contains("mac"))
        {
            fontSearchPaths.add(Path.of("/Library/Fonts"));
            fontSearchPaths.add(Path.of(System.getProperty("user.home"), "Library/Fonts"));
        }
        else
        {
            // Common Linux paths
            fontSearchPaths.add(Path.of("/usr/share/fonts"));
            fontSearchPaths.add(Path.of("/usr/local/share/fonts"));
            fontSearchPaths.add(Path.of(System.getProperty("user.home"), ".local/share/fonts"));
        }
    }
}
