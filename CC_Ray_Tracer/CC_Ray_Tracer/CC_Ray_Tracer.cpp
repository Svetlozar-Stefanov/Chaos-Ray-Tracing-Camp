#include <iostream>
#include <vector>
#include <fstream>

#include "Utils/color.h"
#include "Utils/Shapes/rectangle.h"
#include "Utils/Shapes/triangle.h"
#include "Utils/Shapes/circle.h"

using std::vector;
using std::ofstream;

//Image
const int image_width = 1280;
const int image_height = 720;

const int max_color_comp = 255;

//Scene
vector<shape2d*> shapes;

//Task01
void random_rectangles(int r, int c)
{
    int rec_w = image_width / r;
    int rec_h = image_height / c;

    vector<vector<color>> color_map(r);
    for (int i = 0; i < r; i++)
    {
        for (int j = 0; j < c; j++)
        {
            color_map[i].push_back(rand_solid_color());
            while (i > 0 && j > 0 && 
                (color_map[i - 1][j] == color_map[i][j] ||
                color_map[i][j-1] == color_map[i][j]))
            {
                color_map[i][j] = rand_solid_color();
            }
            while (i > 0 && color_map[i-1][j] == color_map[i][j])
            {
                color_map[i][j] = rand_solid_color();
            }
            while (j > 0 && color_map[i][j-1] == color_map[i][j])
            {
                color_map[i][j] = rand_solid_color();
            }
        }
    }

    ofstream file("img_01.ppm", std::ios::out | std::ios::binary);
    file << "P3\n";
    file << image_width << " " << image_height << "\n";
    file << max_color_comp << "\n";
    for (int i = 0; i < image_height; i++)
    {
        int ci = clamp(0, i / rec_h, r - 1);
        for (int j = 0; j < image_width; j++)
        {
            int cj = clamp(0, j/rec_w, c - 1);
            
            color c = color_map[ci][cj];
            file << max_color_comp * c.x() << "\t" << max_color_comp * c.y() << "\t" << max_color_comp * c.z() << "\n";
        }
        file << "\n";
    }
    file.close();
}

//Task02
void load_house()
{
    rectangle* grass = new rectangle(
        point2d(0.0, 0.0),
        point2d(image_width, -200.0),
        point2d(0.0, image_height),
        color(0.0, 0.5, 0.0));
    shapes.push_back(grass);

    rectangle* walls = new rectangle(
        point2d(-300, 0.0),
        point2d(300.0, -400.0),
        point2d(image_width / 2, image_height-50),
        color(0.9, 0.4, 0.0));
    shapes.push_back(walls);

    rectangle* window1 = new rectangle(
        point2d(0, -50),
        point2d(100, 50),
        point2d(image_width / 2 + 100, image_height - 250),
        color(0.0, 0.8, 0.8));
    shapes.push_back(window1);

    rectangle* window2 = new rectangle(
        point2d(0, -50),
        point2d(-100, 50),
        point2d(image_width / 2 - 100, image_height - 250),
        color(0.0, 0.8, 0.8));
    shapes.push_back(window2);

    rectangle* door = new rectangle(
        point2d(-50.0, 0.0),
        point2d(50.0, -250.0),
        point2d(image_width / 2, image_height-50),
        color(0.8, 0.3, 0.0));
        shapes.push_back(door);

    triangle* roof = new triangle(
        point2d(-400.0, 0.0),
        point2d(0.0, -300),
        point2d(400.0, 0),
        point2d(image_width / 2, image_height-400),
        color(0.9, 0.0, 0.0));
    shapes.push_back(roof);

    circle* sun = new circle(200.0, point2d(20, 20), color(1, 1, 0));
    shapes.push_back(sun);

    circle* mouth = new circle(80.0, point2d(60, 70), color(0, 0, 0));
    shapes.push_back(mouth);

    circle* mask = new circle(85.0, point2d(60, 55), color(1, 1, 0));
    shapes.push_back(mask);

    circle* eye1 = new circle(20.0, point2d(50, 50), color(0, 0, 0));
    shapes.push_back(eye1);
    circle* eye2 = new circle(20.0, point2d(110, 50), color(0, 0, 0));
    shapes.push_back(eye2);

    rectangle* frame = new rectangle(
        point2d(-52.0, 0.0),
        point2d(52.0, -10.0),
        point2d(80, 41),
        color(0.0, 0.0, 0.0));
    shapes.push_back(frame);

}

int main()
{
    random_rectangles(16, 16);

    load_house();

    ofstream file("img_02.ppm", std::ios::out | std::ios::binary);
    file << "P3\n";
    file << image_width << " " << image_height << "\n";
    file << max_color_comp << "\n";
    for (int i = 0; i < image_height; i++)
    {
        for (int j = 0; j < image_width; j++)
        {
            color c;
            bool hit = false;
            for(int s = 0; s < shapes.size(); s++)
            {
                if (shapes[s]->contains(point2d(j, i)))
                {
                    hit = true;
                    c = shapes[s]->c;
                }
            }

            if (!hit)
            {
                c = color(0.85, 1, 1);
            }
            file << max_color_comp * c.x() << "\t" << max_color_comp * c.y() << "\t" << max_color_comp * c.z() << "\n";
        }
        file << "\n";
    }
    file.close();
}