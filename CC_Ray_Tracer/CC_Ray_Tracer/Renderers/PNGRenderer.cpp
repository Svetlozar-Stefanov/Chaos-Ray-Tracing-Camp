#include "../Utils/stb_image/stb_image.h"
#include "../Utils/stb_image/stb_image_write.h"
#include "PNGRenderer.h"

void PNGRenderer::formatRender(const std::string& fileName, int width, int height, unsigned char* pixels)
{
    if (!stbi_write_png(fileName.c_str(), width, height, COLOR_CHANNELS, pixels, 0))
    {
        std::cerr << "\nCould not create image.\n";
        return;
    }
    cout << fileName << " done.\n";
}
