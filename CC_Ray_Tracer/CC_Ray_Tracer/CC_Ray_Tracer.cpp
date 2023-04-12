#include <iostream>
#include <vector>
#include <fstream>

#include "Utils/Color.h"
#include "Utils/ray.h"
#include "Utils/3DShapes/shape.h"
#include "Utils/3DShapes/triangle.h"

using std::vector;
using std::ofstream;

//Image
const int imageWidth = 1280;
const int imageHeight = 720;
const int aspectRatio = 16 / 9;


const int maxColorComponent = 255;
    
//Camera
Vector3 origin(0, 0, 0);

Ray generateRay(int x, int y)
{
    float p_x = x + 0.5;
    float p_y = y + 0.5;

    p_x /= (float)imageWidth;
    p_y /= (float)imageHeight;

    p_x = (2.0 * p_x) - 1;
    p_y = 1 - (2.0 * p_y);

    p_x *= (float)imageWidth * aspectRatio / imageHeight;

    Vector3 ray_dir(p_x, p_y, -1);
    normalize(ray_dir);

    return Ray(origin, ray_dir);
}

//Scene
vector<Shape*> sceneActors;

int main()
{
    //Materials
    Material red;
    red.materialColor = Color(0.8, 0.0, 0.0);
    Material green;
    green.materialColor = Color(0.0, 0.8, 0.0);
    Material blue;
    blue.materialColor = Color(0.0, 0.0, 0.8);
    Material rnd;
    rnd.materialColor = rand_color();


    Vector3 top(0, 2, -3);
    Vector3 left(0.2, 0, -1);
    Vector3 right(2, 0, -2);
    Vector3 back(-2, -0.3, -4);


    sceneActors.push_back(new Triangle(
        top, left, right, green));

    sceneActors.push_back(new Triangle(
        top, right, back , blue));

    sceneActors.push_back(new Triangle(
        top, back, left, red));

    sceneActors.push_back(new Triangle(
        right, left, back, blue));

    ofstream file("ray.ppm", std::ios::out | std::ios::binary);
    file << "P3\n";
    file << imageWidth << " " << imageHeight << "\n";
    file << maxColorComponent << "\n";
    for (int y = 0; y < imageHeight; ++y)
    {
        for (int x = 0; x < imageWidth; ++x)
        {
            Color pixelColor;

            Ray ray = generateRay(x, y);
            Intersection intersection;
            bool hit = false;

            for (int i = sceneActors.size() - 1; i >= 0; i--)
            {
                if (sceneActors[i]->intersects(ray, intersection))
                {
                    hit = true;
                }
            }

            /*for (auto actor : scene_actors)
            {
                if (actor->intersects(ray, intersection))
                {
                    hit = true;
                }
            }*/


            if (hit)
            {
                pixelColor = intersection.material.materialColor;
            }
            else
            {
                pixelColor = Color(0.35, 0.8, 0.95);
            }

            
            file << (int)(maxColorComponent * pixelColor.x()) << " "
                << (int)(maxColorComponent * pixelColor.y()) << " "
                << (int)(maxColorComponent * pixelColor.z()) << "\t";
        }
        file << "\n";
    }
    file.close();
}