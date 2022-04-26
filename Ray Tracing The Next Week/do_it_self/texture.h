//
// Created by jonny on 2022/4/26.
//

#ifndef RAYTRACING_TEXTURE_H
#define RAYTRACING_TEXTURE_H

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "vec3.h"
#include "utility.h"
class texture
{
public:
    virtual color value(double u, double v, const point3& p) const  = 0;
};

class solid_color : public texture {
public:
    solid_color() { color_value = color(0, 0.5, 0); }

    solid_color(const color &c) : color_value(c) {}

    solid_color(double red, double green, double blue) : color_value(color(red, green, blue)){}

    virtual color value(double u, double v, const point3 &p) const
    {
        return color_value;
    }

protected:
    color color_value;
};

class image_texture : public texture
{
public:
    const static int bytes_per_pixel = 3;
    image_texture():data(nullptr), width(0), height(0), bytes_per_scanline(0){}
    image_texture(const char* filename)
    {
        auto components_per_pixel = bytes_per_pixel;
        data = stbi_load(filename, &width, &height, &components_per_pixel, components_per_pixel);
        if(!data)
        {
            std::cerr << "ERROR: Could not load texture image file '" << filename << "'.\n";
            width = height = 0;
        }
        bytes_per_scanline = bytes_per_pixel * width;
    }

    ~image_texture()
    {
        delete data;
    }

    virtual color value(double u, double v, const point3 &p) const override
    {
        if(!data)
        {
            return color(0, 1, 1);
        }
        u = clamp(u, 0.0, 1.0);
        v = 1.0- clamp(v, 0.0, 1.0);

        auto i = static_cast<int>(u * width);
        auto j = static_cast<int>(v * height);

        if(i >= width) i = width - 1;
        if(j >= height) j = height - 1;

        auto pixel = data + j * bytes_per_scanline + i * bytes_per_pixel;

        const auto color_scale = 1.0 / 255;

        return color(pixel[0] * color_scale, pixel[1] * color_scale, pixel[2]*color_scale);
    }
protected:
    unsigned char* data;
    int width, height;
    int bytes_per_scanline;
};

#endif //RAYTRACING_TEXTURE_H
