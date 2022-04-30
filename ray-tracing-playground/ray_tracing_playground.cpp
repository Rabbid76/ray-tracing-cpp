#include "core/configuration.h"
#include "core/scene.h"
#include "core/test_scene_factory.h"
#include "iterator/iterator_exp2.h"
#include <math/random.h>
#include <cmath>
#include <memory>
#include <vector>
#include <iostream>
#include <string>
#include <thread>
#include <future>
#include <chrono>
#include <CImg/CImg.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION 
#define __STDC_LIB_EXT1__
#include <stb/stb_image_write.h>

using namespace ray_tracing_core;
using namespace ray_tracing_utility;
using namespace cimg_library;
using namespace std::chrono_literals;

using BufferData = std::pair<math::ColorRGB, uint32_t>;

void render(core::Scene *scene, iterator::Iterator &&iterator, uint32_t samples, BufferData *render_buffer);
void transfer_image_data(const std::vector<BufferData> &render_buffer, CImg<uint8_t>& image);
void save(CImg<uint8_t> &image, const std::string& filename);

int main()
{
    std::cout << "start" << std::endl;

    const uint32_t cx = 400;
    const uint32_t cy = 200;
    const uint32_t samples = 10;
    const double aspect = static_cast<double>(cx) / static_cast<double>(cy);
    auto scene = std::unique_ptr<core::Scene>(core::TestSceneFactory()
        .set_configuration(core::Configuration
            {
                .maximum_depth = 100
            })
        .set_aspect(aspect)
        .new_scene());

    std::cout << "render" << std::endl;
    CImg<uint8_t> image(cx, cy, 1, 4, 0);
    std::vector<BufferData> render_buffer(cx*cy, {math::ColorRGB(0), 0});
    render(scene.get(), iterator::IteratorExp2(cx, cy), 1, render_buffer.data());
    auto render_future_1 = std::async(std::launch::async, [&] {
        render(scene.get(), iterator::IteratorExp2(cx, cy), samples / 2, render_buffer.data());
    });
    auto render_future_2 = std::async(std::launch::async, [&] {
        render(scene.get(), iterator::IteratorExp2(cx, cy), samples / 2, render_buffer.data());
    });
   

    CImgDisplay image_display(image, "scene");
    image_display.move(100, 100);
    while (!image_display.is_closed() && 
            render_future_1.wait_for(0ms) == std::future_status::timeout &&
            render_future_2.wait_for(0ms) == std::future_status::timeout)
    {
        transfer_image_data(render_buffer, image);
        auto iamge_data = image.data();
        image_display.wait(100);
        image_display.display(image);
    }
    transfer_image_data(render_buffer, image);

    std::cout << "write" << std::endl;
    save(image, "playground.png");
    
    std::cout << "end" << std::endl;
    image.display(image_display, true, nullptr, false);
    return 0;
}

void render(core::Scene* scene, iterator::Iterator &&iterator, uint32_t samples, BufferData* render_buffer)
{
    auto [cx, cy] = iterator.get_size();
    math::RandomGenerator randomGenerator;
    while (true)
    {
        auto [x, y, size] = iterator.next();
        if (size == 0)
            break;

        math::ColorRGB fragment_color(0);
        for (uint32_t s = 0; s < samples; ++s) {
            double u = (static_cast<double>(x) + randomGenerator.random_size()) / static_cast<double>(cx);
            double v = (static_cast<double>(y) + randomGenerator.random_size()) / static_cast<double>(cy);
            fragment_color += scene->ray_trace_color(u, v);
        }
        uint32_t i = ((cy - y - 1) * cx) + x;
        render_buffer[i].first += fragment_color;
        render_buffer[i].second += samples;
    }
}

void transfer_image_data(const std::vector<BufferData>& render_buffer, CImg<uint8_t>& image)
{
    uint32_t cx = static_cast<uint32_t>(image.width());
    uint32_t cy = static_cast<uint32_t>(image.height());
    auto image_data = image.data();
    for (uint32_t i = 0; i < cx * cy; ++i)
    {
        auto fragment_color = render_buffer[i].first / static_cast<float>(render_buffer[i].second);
        image_data[i] = static_cast<uint8_t>(std::lround(std::sqrt(fragment_color[0]) * 255.0));
        image_data[cx * cy + i] = static_cast<uint8_t>(std::lround(std::sqrt(fragment_color[1]) * 255.0));
        image_data[cx * cy * 2 + i] = static_cast<uint8_t>(std::lround(std::sqrt(fragment_color[2]) * 255.0));
        image_data[cx * cy * 3 + i] = 255;
    }
}

void save(CImg<uint8_t> &image, const std::string& filename)
{
    uint32_t cx = static_cast<uint32_t>(image.width());
    uint32_t cy = static_cast<uint32_t>(image.height());
    //image.save_png("filename");
    std::vector<uint8_t> pixel_data(cx * cy * 4);
    for (uint32_t i = 0; i < cx * cy; ++i)
    {
        pixel_data[i * 4] = image.data()[i];
        pixel_data[i * 4 + 1] = image.data()[cx * cy + i];
        pixel_data[i * 4 + 2] = image.data()[cx * cy * 2 + i];
        pixel_data[i * 4 + 3] = image.data()[cx * cy * 3 + i];
    }
    stbi_write_png(filename.c_str(), static_cast<int>(cx), static_cast<int>(cy), 4, pixel_data.data(), static_cast<int>(cx) * 4);
}
