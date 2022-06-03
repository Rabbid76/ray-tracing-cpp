#include "core/configuration.h"
#include "core/test_scene_factory.h"
#include "renderer/renderer_async.h"
#include "viewer/viewer_cimg.h"
#include "duration_measurement.h"
#include <iostream>
#include <memory>
#ifdef WIN32
#define __STDC_LIB_EXT1__
#endif
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

using namespace ray_tracing_core;
using namespace ray_tracing_utility;

int main()
{
    const uint32_t cx = 400;
    const uint32_t cy = 200;
    const uint32_t samples = 200;
    const uint32_t threads = 2;
    const double aspect = static_cast<double>(cx) / static_cast<double>(cy);
    auto scene = std::unique_ptr<core::Scene>(core::TestSceneFactory()
        .set_configuration(core::Configuration
            {
                .maximum_depth = 100
            })
        .set_aspect(aspect)
        .new_scene());

    renderer::RendererAsync renderer(threads);

    std::cout << "start rendering" << std::endl;
    DurationMeasurement duration_measurement;
    renderer.render(*scene, { cx, cy }, samples);
    
    viewer::ViewerCImg()
        .set_image_store_callback([&duration_measurement](const renderer::Renderer& renderer, bool store)
            {
                if (!renderer.is_finished() && ! store)
                    return;
                std::cout << "duration: " << duration_measurement.storeTimePoint().getTotalDuration() << " s" << std::endl;
                std::string filename = "rendering/test_scene.png";
                auto [cx, cy] = renderer.get_buffer_size();
                auto rgba8 = renderer.get_rgba8();
                stbi_write_png(
                    filename.c_str(),
                    static_cast<int>(cx), static_cast<int>(cy), 4,
                    rgba8.data(), static_cast<int>(cx) * 4);
            })
        .preview(renderer);

    return 0;
}