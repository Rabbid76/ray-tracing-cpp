#include "iterator/iterator_exp2.h"
#include "renderer/renderer_async.h"
#include <algorithm>
#include <chrono>
#include <exception>
#include <iostream>

using namespace ray_tracing_utility;
using namespace ray_tracing_utility::renderer;
using namespace ray_tracing_core;
using namespace std::chrono_literals;

RendererAsync::RendererAsync(uint32_t number_of_threads)
	: number_of_threads(number_of_threads)
{}

RendererAsync::~RendererAsync()
{}

void RendererAsync::render(core::Scene& scene, std::tuple<uint32_t, uint32_t> size, uint32_t samples)
{
    render_scene = &scene;
    buffer_size = size;
    render_samples = samples;
    auto [cx, cy] = size;
    buffer = std::vector<BufferData>(cx * cy, { math::ColorRGB(0), 0 });
    render_fragments = cx * cy * samples;
	
	for (uint32_t i = 0; i < number_of_threads; ++i)
    {
        asyncs.push_back(std::async(std::launch::async, [&]
        {
            try
            {
                render();
            }
            catch (std::exception e)
            {
                std::cout << e.what() << std::endl;
            }
        }));
    }
}

bool RendererAsync::is_finished() const
{
    return !std::any_of(asyncs.begin(), asyncs.end(), [](const std::future<void> &async)
    {
        return async.wait_for(0ms) == std::future_status::timeout;
    });
}

double RendererAsync::percentage_done() const
{
	return static_cast<double>(actual_render_fragments) / static_cast<double>(render_fragments);
}

void RendererAsync::render()
{
    auto [cx, cy] = buffer_size;
    math::RandomGenerator randomGenerator;
    while (true)
    {
        auto count = actual_sample_count.fetch_add(1);
        if (count >= render_samples)
            break;

        iterator::IteratorExp2 iterator(cx, cy);
        while (true)
        {
            auto [x, y, size] = iterator.next();
            if (size == 0)
                break;

            double u = (static_cast<double>(x) + randomGenerator.random_size()) / static_cast<double>(cx);
            double v = (static_cast<double>(y) + randomGenerator.random_size()) / static_cast<double>(cy);
            auto fragment_color = render_scene->ray_trace_color(u, v);
            fragment_color = glm::clamp(fragment_color, 0.0f, 1.0f);
            uint32_t i = ((cy - y - 1) * cx) + x;
            buffer[i].first += fragment_color;
            buffer[i].second += 1;
            actual_render_fragments++;
        }
    }
}