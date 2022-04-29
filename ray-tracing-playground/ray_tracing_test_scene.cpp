#include "core/configuration.h"
#include "core/scene.h"
#include "core/test_scene_factory.h"
#include <math/random.h>
#include <cmath>
#include <memory>
#include <vector>
#include <iostream>
#include <string>
#define STB_IMAGE_WRITE_IMPLEMENTATION 
#define __STDC_LIB_EXT1__
#include <stb/stb_image_write.h>

using namespace ray_tracing_core;

int main()
{
    std::cout << "start" << std::endl;

    const uint32_t cx = 400;
    const uint32_t cy = 200;
    const uint32_t samples = 100;
    const double aspect = static_cast<double>(cx) / static_cast<double>(cy);
    auto scene = std::unique_ptr<core::Scene>(core::TestSceneFactory()
        .set_configuration(core::Configuration
            {
                .maximum_depth = 100
            })
        .set_aspect(aspect)
        .new_scene());

    std::cout << "render" << std::endl;
    std::vector<uint8_t> pixel_data(cx * cy * 4);
    math::RandomGenerator randomGenerator;
    const uint32_t one_percent = cx * cy / 100;
    for (uint32_t y = 0; y < cy; ++y) {
        for (uint32_t x = 0; x < cx; ++x) {
            math::ColorRGB fragment_color(0);
            for (uint32_t s = 0; s < samples; ++s) {
                double u = (static_cast<double>(x) + randomGenerator.random_size()) / static_cast<double>(cx);
                double v = (static_cast<double>(y) + randomGenerator.random_size()) / static_cast<double>(cy);
                fragment_color += scene->ray_trace_color(u, v);
            }
            fragment_color /= static_cast<double>(samples);

            uint32_t i = ((cy - y - 1) * cx) + x;
            pixel_data[i * 4] = static_cast<uint8_t>(std::lround(std::sqrt(fragment_color[0]) * 255.0));
            pixel_data[i * 4 + 1] = static_cast<uint8_t>(std::lround(std::sqrt(fragment_color[1]) * 255.0));
            pixel_data[i * 4 + 2] = static_cast<uint8_t>(std::lround(std::sqrt(fragment_color[2]) * 255.0));
            pixel_data[i * 4 + 3] = 255;

            uint32_t n = (y * cx + x + 1);
            if ((n % one_percent) == 0)
                std::cout << "\r" << n / one_percent << "% ";
        }
    }
    std::cout << std::endl;

    std::cout << "write" << std::endl;
    stbi_write_png("test_scene.png", static_cast<int>(cx), static_cast<int>(cy), 4, pixel_data.data(), static_cast<int>(cx) * 4);

    std::cout << "end" << std::endl;
    return 0;
}
