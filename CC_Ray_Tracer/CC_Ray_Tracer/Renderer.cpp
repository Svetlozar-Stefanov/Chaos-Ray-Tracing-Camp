#include "Renderer.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "Utils/stb_image/stb_image_write.h"
#include "Utils/Math/UtilFunctions.h"

const int MAX_COLOR_COMP = 256;

const int COLOR_CHANNELS = 3;

void Renderer::renderToPPM(const Scene& scene, const string& fileName)
{
    //Rendering
    std::cout << "Rendering: " << fileName << '\n';

    ofstream file(fileName, std::ios::out | std::ios::binary);
    file << "P3\n";

    int width = scene.getWidth();
    int height = scene.getHeight();

    file << width << " " << height << "\n";
    file << MAX_COLOR_COMP << "\n";
    for (int y = 0; y < height; ++y)
    {
        cout << (int)(((float)y / height) * 100) << "%" << "\r";
        for (int x = 0; x < width; ++x)
        {
            Color pixelColor(0,0,0);

            pixelColor = scene.colorAtPixel(x, y);

            file << (int)(MAX_COLOR_COMP * pixelColor.x()) << " "
                << (int)(MAX_COLOR_COMP * pixelColor.y()) << " "
                << (int)(MAX_COLOR_COMP * pixelColor.z()) << "\t";
        }
        file << "\n";
    }
    cout << fileName << " done.\n";
    file.close();
}

void Renderer::renderToJPG(const Scene& scene, const string& fileName)
{
    //Rendering
    std::cout << "Rendering: " << fileName << '\n';

    int width = scene.getWidth();
    int height = scene.getHeight();

    unsigned char* pixels = new unsigned char[width * height * COLOR_CHANNELS];

    for (int y = 0; y < height; ++y)
    {
        cout << (int)(((float)y / height) * 100) << "%" << "\r";
        for (int x = 0; x < width; ++x)
        {
            Color pixelColor(0, 0, 0);

            pixelColor = scene.colorAtPixel(x, y);

            pixels[(y * width * COLOR_CHANNELS) + x * COLOR_CHANNELS    ] = MAX_COLOR_COMP * toRange(pixelColor.x(), 0, 0.999);
            pixels[(y * width * COLOR_CHANNELS) + x * COLOR_CHANNELS + 1] = MAX_COLOR_COMP * toRange(pixelColor.y(), 0, 0.999);
            pixels[(y * width * COLOR_CHANNELS) + x * COLOR_CHANNELS + 2] = MAX_COLOR_COMP * toRange(pixelColor.z(), 0, 0.999);
        }
    }

    if (!stbi_write_jpg(fileName.c_str(), width, height, COLOR_CHANNELS, pixels, 90))
    {
        std::cerr << "\nCould not create image.\n";
        delete[] pixels;
        return;
    }
    delete[] pixels;
    cout << fileName << " done.\n";
}