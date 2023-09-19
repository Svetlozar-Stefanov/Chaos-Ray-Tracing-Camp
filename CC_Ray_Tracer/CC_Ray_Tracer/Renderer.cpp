#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "Renderer.h"

using std::string;
using std::cout;
using std::vector;
using std::ofstream;

const int MAX_COLOR_COMP = 256;
const int COLOR_CHANNELS = 3;
const float SHADOW_BIAS = 0.1;
const float MAX_DEPTH = 5;

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

            pixelColor = colorAtPixel(x, y, scene);

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

            pixelColor = colorAtPixel(x, y, scene);

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

void Renderer::renderToPNG(const Scene& scene, const string& fileName)
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

            pixelColor = colorAtPixel(x, y, scene);

            pixels[(y * width * COLOR_CHANNELS) + x * COLOR_CHANNELS] = MAX_COLOR_COMP * toRange(pixelColor.x(), 0, 0.999);
            pixels[(y * width * COLOR_CHANNELS) + x * COLOR_CHANNELS + 1] = MAX_COLOR_COMP * toRange(pixelColor.y(), 0, 0.999);
            pixels[(y * width * COLOR_CHANNELS) + x * COLOR_CHANNELS + 2] = MAX_COLOR_COMP * toRange(pixelColor.z(), 0, 0.999);
        }
    }

    if (!stbi_write_png(fileName.c_str(), width, height, COLOR_CHANNELS, pixels, 0))
    {
        std::cerr << "\nCould not create image.\n";
        delete[] pixels;
        return;
    }
    delete[] pixels;
    cout << fileName << " done.\n";
}

Color Renderer::colorAtPixel(int x, int y, const Scene& scene) const
{
    Color pixelColor(0, 0, 0);
    Ray ray = scene.getCamera().generateRay(x, y);
    Intersection intersection;

    pixelColor = traceRay(ray, intersection, scene);

    return pixelColor;
}

Color Renderer::traceRay(const Ray& ray, Intersection& intersection, const Scene& scene) const
{
    Color pixelColor(0, 0, 0);
    bool hit = scene.intersects(ray, intersection);

    if (hit && ray.getDepth() < MAX_DEPTH)
    {
        pixelColor += shade(ray, intersection, scene);
    }
    else
    {
        pixelColor = scene.getBackgroundColor();
    }

    return pixelColor;
}

Color Renderer::shade(const Ray& ray, const Intersection& intersection, const Scene& scene) const
{
    Color pixelColor(0, 0, 0);
    Vector3 hitPoint = ray.getPoint(intersection.getT());
    const Material* material = intersection.getMaterial();

    if (material->getType() == MaterialType::Diffuse)
    {
        pixelColor += diffuseShade(hitPoint, intersection, scene);
    }
    else if (material->getType() == MaterialType::Reflective)
    {
        pixelColor += reflectiveShade(ray, intersection, scene);
    }

    return pixelColor;
}

Color Renderer::diffuseShade(const Vector3& interPoint, const Intersection& intersection, const Scene& scene) const
{
    Color contribution(0, 0, 0);
    const Material* material = intersection.getMaterial();

    for (const LightSource& light : scene.getLights())
    {
        Vector3 lightDirection = light.getPosition() - interPoint;
        float lightRadius = lightDirection.getLength();
        normalize(lightDirection);

        Vector3 normal = intersection.getNormal();
        if (material->isSmooth())
        {
            normal = intersection.getIntersectionNormal();
        }

        float cosLaw = std::max(0.0f, dot(lightDirection, normal));
        float sphereArea = 4 * PI * lightRadius * lightRadius;

        Ray ray(interPoint + normal * SHADOW_BIAS, lightDirection, RayType::Shadow, 0);
        Intersection temp((interPoint - light.getPosition()).getLength());

        if (!scene.intersects(ray, temp))
        {
            contribution += Color(
                light.getIntensity() / sphereArea * material->getAlbedo().x() * cosLaw,
                light.getIntensity() / sphereArea * material->getAlbedo().y() * cosLaw,
                light.getIntensity() / sphereArea * material->getAlbedo().z() * cosLaw);
        }
    }

    return contribution;
}

Color Renderer::reflectiveShade(const Ray& ray, const Intersection& intersection, const Scene& scene) const
{
    const Material* material = intersection.getMaterial();
    Vector3 hitPoint = ray.getPoint(intersection.getT());

    Vector3 normal = intersection.getNormal();
    if (material->isSmooth())
    {
        normal = intersection.getIntersectionNormal();
    }
    Vector3 direction = ray.getDirection() - (normal * 2 * dot(ray.getDirection(), normal));
    normalize(direction);

    Ray reflectiveRay(hitPoint + normal * SHADOW_BIAS, direction, RayType::Reflection, ray.getDepth() + 1);
    Intersection temp;
    Color col = traceRay(reflectiveRay, temp, scene);
    return Color(
        col.x() * material->getAlbedo().x(),
        col.y() * material->getAlbedo().y(),
        col.z() * material->getAlbedo().z());
}