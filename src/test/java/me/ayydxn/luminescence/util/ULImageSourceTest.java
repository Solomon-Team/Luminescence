package me.ayydxn.luminescence.util;

import me.ayydxn.luminescence.BaseLuminescenceTest;
import me.ayydxn.luminescence.bitmap.BitmapFormat;
import me.ayydxn.luminescence.bitmap.ULBitmap;
import me.ayydxn.luminescence.config.ULConfig;
import me.ayydxn.luminescence.geometry.ULRect;
import me.ayydxn.luminescence.platform.ULPlatform;
import me.ayydxn.luminescence.platform.impl.StandardULFileSystem;
import me.ayydxn.luminescence.platform.impl.StandardULFontLoader;
import me.ayydxn.luminescence.renderer.ULRenderer;
import org.junit.jupiter.api.*;

import java.io.File;
import java.net.URL;
import java.util.Objects;

import static org.junit.jupiter.api.Assertions.*;

@TestInstance(TestInstance.Lifecycle.PER_CLASS)
@TestMethodOrder(MethodOrderer.OrderAnnotation.class)
class ULImageSourceTest extends BaseLuminescenceTest
{
    private ULRenderer renderer;

    @BeforeAll
    void setup() throws Exception
    {
        ULPlatform.setFileSystem(new StandardULFileSystem());
        ULPlatform.setFontLoader(new StandardULFontLoader());

        URL ultralightResourcesURL = Objects.requireNonNull(ULImageSourceTest.class.getClassLoader().getResource("./ultralight"));

        try (ULConfig config = new ULConfig())
        {
            config.setResourcePathPrefix(new File(ultralightResourcesURL.toURI()) + "/");

            this.renderer = new ULRenderer(config);
        }
    }

    @AfterAll
    void tearDown()
    {
        if (this.renderer != null)
            this.renderer.destroy();
    }

    @Test
    @Order(1)
    @DisplayName("Create Image Source From Bitmap Without Throwing")
    void testCreateFromBitmap()
    {
        ULBitmap bitmap = ULBitmap.create(64, 64, BitmapFormat.BGRA8_UNORM_SRGB);

        assertDoesNotThrow(() ->
        {
            ULImageSource imageSource = ULImageSource.createFromBitmap(bitmap);
            imageSource.destroy();
        });

        bitmap.destroy();
    }

    @Test
    @Order(2)
    @DisplayName("Image Source Created From Bitmap Has A Non-Zero Native Handle")
    void testCreateFromBitmapHandle()
    {
        ULBitmap bitmap = ULBitmap.create(64, 64, BitmapFormat.BGRA8_UNORM_SRGB);
        ULImageSource imageSource = ULImageSource.createFromBitmap(bitmap);

        assertNotEquals(0, imageSource.getHandle(), "Native handle must be non-zero after creation");

        imageSource.destroy();
        bitmap.destroy();
    }

    @Test
    @Order(3)
    @DisplayName("Create Image Source From GPU Texture Without Throwing")
    void testCreateFromTexture()
    {
        int textureID = 1;
        ULRect textureUV = new ULRect(0.0f, 0.0f, 1.0f, 1.0f);

        assertDoesNotThrow(() ->
        {
            ULImageSource imageSource = ULImageSource.createFromTexture(128, 128, textureID, textureUV, null);
            imageSource.destroy();
        });
    }

    @Test
    @Order(4)
    @DisplayName("Create Image Source From GPU Texture With Backing Bitmap Without Throwing")
    void testCreateFromTextureWithBitmap()
    {
        int textureID = 1;
        ULRect textureUV = new ULRect(0.0f, 0.0f, 1.0f, 1.0f);
        ULBitmap bitmap = ULBitmap.create(128, 128, BitmapFormat.BGRA8_UNORM_SRGB);

        assertDoesNotThrow(() ->
        {
            ULImageSource imageSource = ULImageSource.createFromTexture(128, 128, textureID, textureUV, bitmap);
            imageSource.destroy();
        });

        bitmap.destroy();
    }

    @Test
    @Order(5)
    @DisplayName("Image Source Created From Texture Has A Non-Zero Native Handle")
    void testCreateFromTextureHandle()
    {
        ULRect textureUV = new ULRect(0.0f, 0.0f, 1.0f, 1.0f);
        ULImageSource imageSource = ULImageSource.createFromTexture(64, 64, 1, textureUV, null);

        assertNotEquals(0, imageSource.getHandle(), "Native handle must be non-zero after creation");

        imageSource.destroy();
    }

    @Test
    @Order(6)
    @DisplayName("Handle Is Zeroed After Destroy To Prevent use-after-free")
    void testDestroyZeroesHandle()
    {
        ULBitmap bitmap = ULBitmap.create(32, 32, BitmapFormat.BGRA8_UNORM_SRGB);
        ULImageSource imageSource = ULImageSource.createFromBitmap(bitmap);

        assertNotEquals(0, imageSource.getHandle());
        imageSource.destroy();
        assertEquals(0, imageSource.getHandle(), "Handle must be zeroed after destroy");

        bitmap.destroy();
    }

    @Test
    @Order(7)
    @DisplayName("Double Destroy Does Not Crash")
    void testDoubleDestroy()
    {
        ULBitmap bitmap = ULBitmap.create(32, 32, BitmapFormat.BGRA8_UNORM_SRGB);
        ULImageSource imageSource = ULImageSource.createFromBitmap(bitmap);

        imageSource.destroy();

        assertDoesNotThrow(imageSource::destroy, "Double destroy must not crash or throw");

        bitmap.destroy();
    }

    @Test
    @Order(8)
    @DisplayName("Invalidate Does Not Crash On A Live Bitmap-Backed Image Source")
    void testInvalidateBitmapBacked()
    {
        ULBitmap bitmap = ULBitmap.create(64, 64, BitmapFormat.BGRA8_UNORM_SRGB);
        ULImageSource imageSource = ULImageSource.createFromBitmap(bitmap);

        assertDoesNotThrow(imageSource::invalidate);

        imageSource.destroy();
        bitmap.destroy();
    }

    @Test
    @Order(9)
    @DisplayName("Invalidate Does Not Crash On A Live Texture-Backed Image Source")
    void testInvalidateTextureBacked()
    {
        ULRect textureUV = new ULRect(0.0f, 0.0f, 1.0f, 1.0f);
        ULImageSource imageSource = ULImageSource.createFromTexture(64, 64, 1, textureUV, null);

        assertDoesNotThrow(imageSource::invalidate);

        imageSource.destroy();
    }

    @Test
    @Order(10)
    @DisplayName("Add Image Source To Provider Without Throwing")
    void testAddToProvider()
    {
        ULBitmap bitmap = ULBitmap.create(64, 64, BitmapFormat.BGRA8_UNORM_SRGB);
        ULImageSource imageSource = ULImageSource.createFromBitmap(bitmap);

        assertDoesNotThrow(() -> imageSource.addImageSourceToProvider("test_image_add"));

        imageSource.destroy();
        bitmap.destroy();
    }

    @Test
    @Order(11)
    @DisplayName("Remove Image Source From Provider Without Throwing")
    void testRemoveFromProvider()
    {
        ULBitmap bitmap = ULBitmap.create(64, 64, BitmapFormat.BGRA8_UNORM_SRGB);
        ULImageSource imageSource = ULImageSource.createFromBitmap(bitmap);

        imageSource.addImageSourceToProvider("test_image_remove");

        assertDoesNotThrow(() -> imageSource.removeImageSourceFromProvider("test_image_remove"));

        imageSource.destroy();
        bitmap.destroy();
    }

    @Test
    @Order(12)
    @DisplayName("Remove Non-Existent Identifier From Provider Does Not Crash")
    void testRemoveNonExistentFromProvider()
    {
        ULBitmap bitmap = ULBitmap.create(32, 32, BitmapFormat.BGRA8_UNORM_SRGB);
        ULImageSource imageSource = ULImageSource.createFromBitmap(bitmap);

        assertDoesNotThrow(() -> imageSource.removeImageSourceFromProvider("identifier_that_was_never_added"));

        imageSource.destroy();
        bitmap.destroy();
    }

    @Test
    @Order(13)
    @DisplayName("Adding The Same Identifier Twice Does Not Crash")
    void testAddDuplicateIdentifier()
    {
        ULBitmap bitmapA = ULBitmap.create(32, 32, BitmapFormat.BGRA8_UNORM_SRGB);
        ULBitmap bitmapB = ULBitmap.create(32, 32, BitmapFormat.BGRA8_UNORM_SRGB);

        ULImageSource sourceA = ULImageSource.createFromBitmap(bitmapA);
        ULImageSource sourceB = ULImageSource.createFromBitmap(bitmapB);

        sourceA.addImageSourceToProvider("duplicate_key");

        assertDoesNotThrow(() -> sourceB.addImageSourceToProvider("duplicate_key"));

        sourceB.removeImageSourceFromProvider("duplicate_key");

        sourceA.destroy();
        sourceB.destroy();
        bitmapA.destroy();
        bitmapB.destroy();
    }

    @Test
    @Order(14)
    @DisplayName("Provider Round-Trip: Add, Invalidate, Remove Without Crashing")
    void testProviderRoundTrip()
    {
        ULBitmap bitmap = ULBitmap.create(64, 64, BitmapFormat.BGRA8_UNORM_SRGB);
        ULImageSource imageSource = ULImageSource.createFromBitmap(bitmap);

        assertDoesNotThrow(() ->
        {
            imageSource.addImageSourceToProvider("round_trip_image");
            imageSource.invalidate();
            imageSource.removeImageSourceFromProvider("round_trip_image");
        });

        imageSource.destroy();
        bitmap.destroy();
    }
}