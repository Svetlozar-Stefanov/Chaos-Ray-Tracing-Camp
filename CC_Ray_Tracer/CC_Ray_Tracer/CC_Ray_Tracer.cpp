#include <iostream>
#include <vector>
#include <fstream>

#include "Utils/color.h"
#include "Utils/ray.h"

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

int main()
{
    ofstream file("ray.ppm", std::ios::out | std::ios::binary);
    file << "P3\n";
    file << image_width << " " << image_height << "\n";
    file << max_color_comp << "\n";
    for (int y = 0; y < image_height; y++)
    {
        for (int x = 0; x < image_width; x++)
        {
            color c;

            ray ray = gen_ray(x, y);

            c = ray.Direction();


            
            file << (int)(max_color_comp * abs(c.x())) << " " 
                << (int)(max_color_comp * abs(c.y())) << " " 
                << (int)(max_color_comp * -c.z()) << "\t";
        }
        file << "\n";
    }
    file.close();
}