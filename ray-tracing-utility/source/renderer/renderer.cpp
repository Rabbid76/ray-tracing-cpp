#include "renderer/renderer.h"
#include <cmath>

using namespace ray_tracing_utility;
using namespace ray_tracing_utility::renderer;

std::vector<uint8_t> Renderer::get_rgba8() const
{
    auto [cx, cy] = get_buffer_size();
    auto& render_buffer = get_render_buffer();
    std::vector<uint8_t> pixel_data(cx * cy * 4);
    for (uint32_t i = 0; i < cx * cy; ++i)
    {
        auto fragment_color = render_buffer[i].first / static_cast<float>(render_buffer[i].second);
        pixel_data[i * 4] = static_cast<uint8_t>(std::lround(std::sqrt(fragment_color[0]) * 255.0));
        pixel_data[i * 4 + 1] = static_cast<uint8_t>(std::lround(std::sqrt(fragment_color[1]) * 255.0));
        pixel_data[i * 4 + 2] = static_cast<uint8_t>(std::lround(std::sqrt(fragment_color[2]) * 255.0));
        pixel_data[i * 4 + 3] = 255;
    }
    return pixel_data;
}