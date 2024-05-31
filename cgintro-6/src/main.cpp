#include <config.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
using namespace glm;

// Workaround for an issue with STB, see https://github.com/nothings/stb/issues/1446
#ifndef __STDC_LIB_EXT1__
#define __STDC_LIB_EXT1__
#define sprintf_s snprintf
#endif
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include <array>
#include <iostream>

constexpr size_t WIDTH = 512;
constexpr size_t HEIGHT = WIDTH;
constexpr size_t CHANNELS = 3;
constexpr size_t SIZE = WIDTH * HEIGHT;
constexpr vec2 SHAPE = vec2(WIDTH, HEIGHT);

std::array<vec3, SIZE> data;

void setPixel(const uvec2& loc, const vec3& value) {
    if (loc.x >= WIDTH || loc.y >= HEIGHT) {
        std::cerr << "Invalid pixel location: " << to_string(loc) << std::endl;
        return;
    }
    data[loc.y * WIDTH + loc.x] = value;
}

void drawDistances(const uvec2& loc, float E0, float E1, float E2) {
    setPixel(loc, clamp(vec3(E0, E1, E2), 0.0f, 1.0f));
}

void saveImage() {
    std::array<unsigned char, SIZE * CHANNELS> buffer;
    for (size_t i = 0; i < SIZE; i++) {
        for (size_t j = 0; j < CHANNELS; j++) {
            buffer[i * CHANNELS + j] = static_cast<unsigned char>(clamp(data[i][j], 0.0f, 1.0f) * 255.0f);
        }
    }
    Config::setWorkingDirectory();
    stbi_write_png("output.png", WIDTH, HEIGHT, CHANNELS, buffer.data(), WIDTH * CHANNELS);
    stbi_write_hdr("output.hdr", WIDTH, HEIGHT, CHANNELS, value_ptr(data[0]));
}

vec2 normal(const vec2& v) {
    return normalize(vec2(v.y, -v.x));
}

int main() {
    vec2 A = vec2(0.1f, 0.9f), B = vec2(0.9f, 0.7f), C = vec2(0.5f, 0.1f);
    A *= SHAPE; B *= SHAPE; C *= SHAPE;

    vec2 N0 = normal(B - A), N1 = normal(C - B), N2 = normal(A - C);

    uvec2 loc = uvec2(0, 0);
    vec2 P = vec2(loc);
    float E0, E1, E2;

    // TODO: Rasterize the triangle using Pineda's algorithm and compute the distances E0, E1, E2 for each pixel
    // Note: Draw E0, E1, E2 to the ouptut image using the drawDistances function

    saveImage();
    return 0;
}