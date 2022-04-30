#include "viewer/viewer_cimg.h"
#include <CImg/CImg.h>
#include <cmath>
#include <string>

using namespace ray_tracing_utility;
using namespace ray_tracing_utility::viewer;
using namespace cimg_library;

namespace
{
    void transfer_image_data(const renderer::Renderer::Buffer& render_buffer, CImg<uint8_t>& image)
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
        }
    }
}

void ViewerCImg::preview(const renderer::Renderer &renderer)
{
    auto [cx, cy] = renderer.get_buffer_size();
    CImg<uint8_t> image(cx, cy, 1, 3, 0);
    CImgDisplay image_display(image, "scene");
    image_display.move(100, 100);
    while (!image_display.is_closed() && !renderer.is_finished())
    {
        transfer_image_data(renderer.get_render_buffer(), image);
        auto iamge_data = image.data();
        image_display.wait(100);
        image_display.set_title("%s", std::to_string(renderer.percentage_done()).c_str());
        image_display.display(image);
        if (image_store_callback && (image_display.key(cimg::keyS) || image_display.key(cimg::keyF1)))
            image_store_callback(renderer);
    }
    transfer_image_data(renderer.get_render_buffer(), image);
    if (image_store_callback)
        image_store_callback(renderer);
    image_display.set_title("%s", "finished");
    image.display(image_display, true, nullptr, false);
}