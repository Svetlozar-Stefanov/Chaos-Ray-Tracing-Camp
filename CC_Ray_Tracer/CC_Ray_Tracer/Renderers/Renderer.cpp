#include "Renderer.h"
#include <chrono>
#include <thread>


using std::chrono::high_resolution_clock;
using std::chrono::microseconds;
using std::chrono::duration_cast;

using std::thread;

stack<Bucket> buckets;
mutex m;

void Renderer::render(const Scene& scene, const std::string& fileName)
{
    //Rendering
    std::cout << "Rendering: " << fileName << '\n';
    high_resolution_clock::time_point start = high_resolution_clock::now();

    int width = scene.getWidth();
    int height = scene.getHeight();

    unsigned char* pixels = new unsigned char[width * height * COLOR_CHANNELS];

    const unsigned int threadCount = thread::hardware_concurrency();

    buckets = generateBuckets(BUCKET_SIZE, width, height);
    vector<thread> threads;

    for (int i = 0; i < threadCount; i++)
    {
        threads.push_back(thread(&Renderer::renderBuckets, this, scene, pixels, width));
    }

    for (auto& th : threads)
    {
        th.join();
    }
    threads.clear();

    //renderRegion(scene, pixels, 0, 0, height, width, width);

    formatRender(fileName, width, height, pixels);
    delete[] pixels;

    high_resolution_clock::time_point stop = high_resolution_clock::now();

    microseconds duration = duration_cast<microseconds>(stop - start);
    const double seconds = duration.count() / 1'000'000.0;
    cout << "Execution time: " << seconds << "seconds.\n";
}

void Renderer::renderBuckets(const Scene& scene, unsigned char* pixels, int width)
{
    while (!buckets.empty())
    {
        if (m.try_lock())
        {
            Bucket bucket = buckets.top();
            buckets.pop();
            m.unlock();
            renderRegion(scene, pixels, bucket.colIdx, bucket.rowIdx, bucket.rHeight, bucket.rWidth, width);
        }
    }
}

void Renderer::renderRegion(const Scene& scene, unsigned char* pixels, int colIdx, int rowIdx, int rHeight, int rWidth, int width)
{
    for (int y = colIdx; y < rHeight; ++y)
    {
        for (int x = rowIdx; x < rWidth; ++x)
        {
            Color pixelColor(0, 0, 0);

            pixelColor = colorAtPixel(x, y, scene);

            pixels[(y * width * COLOR_CHANNELS) + x * COLOR_CHANNELS] = MAX_COLOR_COMP * toRange(pixelColor.x(), 0, 0.999);
            pixels[(y * width * COLOR_CHANNELS) + x * COLOR_CHANNELS + 1] = MAX_COLOR_COMP * toRange(pixelColor.y(), 0, 0.999);
            pixels[(y * width * COLOR_CHANNELS) + x * COLOR_CHANNELS + 2] = MAX_COLOR_COMP * toRange(pixelColor.z(), 0, 0.999);
        }
    }
}

stack<Bucket> Renderer::generateBuckets(int bucketSize, int width, int height)
{
    stack<Bucket> buckets;
    for (int colIdx = 0; colIdx < height; colIdx += bucketSize)
    {
        for (int rowIdx = 0; rowIdx < width; rowIdx += bucketSize)
        {
            int rHeight = colIdx + bucketSize;
            int rWidth = rowIdx + bucketSize;
            if (rHeight > height)
            {
                rHeight = height;
            }
            if (rWidth > width)
            {
                rWidth = width;
            }
            buckets.push({ colIdx, rowIdx, rHeight, rWidth });
        }
    }
    return buckets;
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
        pixelColor = diffuseShade(hitPoint, intersection, scene);
    }
    else if (material->getType() == MaterialType::Reflective)
    {
        pixelColor = reflectiveShade(ray, intersection, scene);
    }
    else if (material->getType() == MaterialType::Refractive)
    {
        pixelColor = refractiveShade(ray, hitPoint, intersection, scene);
    }
    else if (material->getType() == MaterialType::Constant)
    {
        pixelColor = material->getAlbedo();
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

Color Renderer::refractiveShade(const Ray& ray, const Vector3& interPoint, const Intersection& intersection, const Scene& scene) const
{
    const Material* material = intersection.getMaterial();
    float ior1 = 1.0f;
    float ior2 = material->getIOR();

    Vector3 normal = intersection.getNormal();
    if (material->isSmooth())
    {
        normal = intersection.getIntersectionNormal();
    }

    if (dot(ray.getDirection(), normal) > 0)
    {
        normal = normal * -1;
        float temp = ior1;
        ior1 = ior2;
        ior2 = temp;
    }

    float cosAlpha = -dot(ray.getDirection(), normal);
    float sinAlpha = sqrt(1.0f - (cosAlpha * cosAlpha));

    Ray reflectionRay(interPoint + (normal * SHADOW_BIAS),
        (ray.getDirection() - (normal * 2 * dot(ray.getDirection(), normal))),
        RayType::Reflection, ray.getDepth() + 1);
    Intersection tempRefl;

    Color col;
    if (sinAlpha < ior2 / ior1)
    {
        float sinBeta = (sinAlpha * ior1) / ior2;
        float cosBeta = sqrt(1 - (sinBeta * sinBeta));

        Vector3 C = (ray.getDirection() + normal * cosAlpha).getNormalized();
        Vector3 B = C * sinBeta;
        Vector3 A = normal * -cosBeta;
        Vector3 R = A + B;

        Ray refractionRay(interPoint + ((normal * -1) * REFRACTION_BIAS), R, RayType::Refraction, ray.getDepth() + 1);
        Intersection tempRefr;
        Color refractionColor = traceRay(refractionRay, tempRefr, scene);
        Color reflectionColor = traceRay(reflectionRay, tempRefl, scene);

        float fresnel = 0.5f * (1.0f - cosAlpha) * (1.0f - cosAlpha) * (1.0f - cosAlpha) * (1.0f - cosAlpha) * (1.0f - cosAlpha);
        col = reflectionColor * fresnel + refractionColor * (1.0f - fresnel);
    }
    else
    {
        col = traceRay(reflectionRay, tempRefl, scene);
    }

    return col;
}