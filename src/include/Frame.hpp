#pragma once
#include "Ayanami.hpp"

#include "Spec.hpp"
#include "Image.hpp"
#include "World.hpp"
#include "Camera.hpp"
#include "Renderer.hpp"
#include "Timer.hpp"
#include "Tracker.hpp"
#include "Utils.hpp"

class Frame {
public:
    Frame() {}
    Frame(int width, int height, int num_samples, int max_depth) : width(width), height(height), samples_per_pixel(num_samples), rec_depth(max_depth) {
        data = new uint32_t[width * height];
    }

    // Segfaults :)
    // ~Frame() {
    //     delete[] data;
    // }

    void render(Camera& camera, const World& world);
    uint8_t* data_as_uint8() {
        uint8_t* _data = new uint8_t[width * height * 3];

        size_t pixel_index = 0;
        for (int i = 0; i < width * height; i++) {
            _data[pixel_index]     = (data[i] & 0x000000ff);
            _data[pixel_index + 1] = (data[i] & 0x0000ff00) >> 8;
            _data[pixel_index + 2] = (data[i] & 0x00ff0000) >> 16;
            pixel_index += 3;
        }
        return _data;
    }

public:
    uint32_t* data;
    int width, height, samples_per_pixel, rec_depth;
};