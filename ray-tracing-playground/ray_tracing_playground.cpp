#include "core/configuration.h"
#include "core/test_scene_factory.h"
#include "renderer/renderer_async.h"
#include "viewer/viewer_cimg.h"
#include <exception>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define __STDC_LIB_EXT1__
#include <stb/stb_image_write.h>
//#include <boost/property_tree/ptree.hpp>
//#include <boost/property_tree/detail/info_parser_error.hpp>
//#include <boost/property_tree/detail/info_parser_read.hpp>

using namespace ray_tracing_core;
using namespace ray_tracing_utility;

int main()
{
    std::fstream scene_json;
    scene_json.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        scene_json.open("scenes/test_scene.json");
    }
    catch (std::exception e)
    {
        std::cout << "error reading scene" << std::endl;
        throw;
    }

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
    renderer::RendererAsync renderer(2);
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

