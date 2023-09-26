#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../Utils/stb_image/stb_image.h"
#include "../Utils/stb_image/stb_image_write.h"
#include "JPGRenderer.h"

void JPGRenderer::formatRender(const std::string& fileName, int width, int height, unsigned char* pixels)
{
    if (!stbi_write_jpg(fileName.c_str(), width, height, COLOR_CHANNELS, pixels, 90))
    {
        std::cerr << "\nCould not create image.\n";
        return;
    }
    cout << fileName << " done.\n";
}
