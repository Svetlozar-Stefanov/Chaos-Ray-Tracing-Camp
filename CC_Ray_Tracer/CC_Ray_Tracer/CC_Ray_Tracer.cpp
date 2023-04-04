#include <iostream>
#include <vector>
#include <fstream>

#include "Utils/color.h"
#include "Utils/ray.h"
#include "Utils/3DShapes/shape.h"
#include "Utils/3DShapes/triangle.h"

using std::vector;
using std::ofstream;

//Image
const int image_width = 1280;
const int image_height = 720;
const int aspect_ratio = 16 / 9;


const int max_color_comp = 255;
    
//Camera
vec3 origin(0, 0, 0);

ray gen_ray(int x, int y)
{
    float p_x = x + 0.5;
    float p_y = y + 0.5;

    p_x /= (float)image_width;
    p_y /= (float)image_height;

    p_x = (2.0 * p_x) - 1;
    p_y = 1 - (2.0 * p_y);

    p_x *= (float)image_width * aspect_ratio / image_height;

    vec3 ray_dir(p_x, p_y, -1);
    normalize(ray_dir);

    return ray(origin, ray_dir);
}

//Scene
vector<shape*> scene_actors;



int main()
{
    //Materials
    Material red;
    red.material_color = color(0.8, 0.0, 0.0);
    Material green;
    green.material_color = color(0.0, 0.8, 0.0);
    Material blue;
    blue.material_color = color(0.0, 0.0, 0.8);
    Material rnd;
    rnd.material_color = rand_color();


    vec3 top(0, 2, -3);
    vec3 left(0.2, 0, -1);
    vec3 right(2, 0, -2);
    vec3 back(-2, -0.3, -4);


    scene_actors.push_back(new triangle(
        top, left, right, green));

    scene_actors.push_back(new triangle(
        top, right, back , blue));

    scene_actors.push_back(new triangle(
        top, back, left, red));

    scene_actors.push_back(new triangle(
        right, left, back, blue));

    ofstream file("ray.ppm", std::ios::out | std::ios::binary);
    file << "P3\n";
    file << image_width << " " << image_height << "\n";
    file << max_color_comp << "\n";
    for (int y = 0; y < image_height; ++y)
    {
        for (int x = 0; x < image_width; ++x)
        {
            color pixel_col;

            ray ray = gen_ray(x, y);
            Intersection intersection;
            bool hit = false;

            for (int i = scene_actors.size() - 1; i >= 0; i--)
            {
                if (scene_actors[i]->intersects(ray, intersection))
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
                pixel_col = intersection.material.material_color;
            }
            else
            {
                pixel_col = color(0.35, 0.8, 0.95);
            }

            
            file << (int)(max_color_comp * pixel_col.x()) << " "
                << (int)(max_color_comp * pixel_col.y()) << " "
                << (int)(max_color_comp * pixel_col.z()) << "\t";
        }
        file << "\n";
    }
    file.close();
}