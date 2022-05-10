#include "core/configuration.h"
#include "core/scene_objects_container.h"
#include "json/scene_deserializer.h"
#include "renderer/renderer_async.h"
#include "utility/std_helper.h"
#include "viewer/viewer_cimg.h"
#include <iostream>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <string>
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
    const uint32_t samples = 10;
    const uint32_t threads = 2;
    const double aspect = static_cast<double>(cx) / static_cast<double>(cy);
    std::shared_ptr<core::Scene> scene;
    std::fstream scene_json;
    scene_json.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        scene_json.open("scenes/test_scene.json");
        auto json_source = std::string(std::istreambuf_iterator<char>(scene_json), std::istreambuf_iterator<char>());
        scene = std::shared_ptr<core::Scene>(json::SceneDeserializer()
            .set_aspect(aspect)
            .new_scene(json_source));
    }
    catch (std::exception e)
    {
        std::cout << "error reading scene: " << e.what() << std::endl;
        throw;
    }
 
    renderer::RendererAsync renderer(threads);
    renderer.render(*scene, { cx, cy }, samples);

    viewer::ViewerCImg()
        .set_image_store_callback([](const renderer::Renderer &renderer)
            {
                if (!renderer.is_finished())
                    return;  
                std::string filename = "rendering/playground.png";
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
