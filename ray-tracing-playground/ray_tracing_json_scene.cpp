#include "core/configuration.h"
#include "core/scene_objects_container.h"
#include "json/scene_deserializer.h"
#include "renderer/renderer_async.h"
#include "utility/std_helper.h"
#include "viewer/viewer_cimg.h"
#include "duration_measurement.h"
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
#include "rapidjson/document.h"

using namespace ray_tracing_core;
using namespace ray_tracing_utility;

std::tuple<uint32_t, uint32_t, uint32_t, uint32_t> get_configuration(const std::string &configuration_filepath);

int main(int argc, char *argv[])
{
    std::string scene_filepath = "scenes/test_scene.json";
    if (argc > 1)
        scene_filepath = argv[1];
    auto dot_pos = scene_filepath.find_last_of(".");
    auto slash_pos = scene_filepath.find_last_of("/\\");
    std::string scene_name = scene_filepath.substr(slash_pos+1, dot_pos-slash_pos-1);
    auto [cx, cy, samples, threads] = get_configuration(argc > 2 ? argv[2] : "");

    const double aspect = static_cast<double>(cx) / static_cast<double>(cy);
    std::shared_ptr<core::Scene> scene;
    std::fstream scene_json;
    scene_json.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        scene_json.open(scene_filepath);
        auto json_source = std::string(std::istreambuf_iterator<char>(scene_json), std::istreambuf_iterator<char>());
        scene = std::shared_ptr<core::Scene>(json::SceneDeserializer()
            .set_aspect(aspect)
            .new_scene(json_source));
    }
    catch (const std::exception &e)
    {
        std::cout << "error reading scene: " << e.what() << std::endl;
        throw;
    }

    renderer::RendererAsync renderer(threads);
    std::cout << "start rendering" << std::endl;
    DurationMeasurement duration_measurement;
    renderer.render(*scene, { cx, cy }, samples);

    viewer::ViewerCImg()
        .set_image_store_callback([&](const renderer::Renderer& renderer)
            {
                if (!renderer.is_finished())
                    return;
                std::cout << "duration: " << duration_measurement.storeTimePoint().getTotalDuration() << " s" << std::endl;
                std::string filename = "rendering/" + scene_name + ".png";
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

std::tuple<uint32_t, uint32_t, uint32_t, uint32_t> get_configuration(const std::string &configuration_filepath) {
    uint32_t cx = 400;
    uint32_t cy = 200;
    uint32_t samples = 200;
    uint32_t threads = 2;
    if (!configuration_filepath.empty())
    {
        std::fstream configuration_json;
        configuration_json.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            configuration_json.open(configuration_filepath);
            auto json_source = std::string(std::istreambuf_iterator<char>(configuration_json), std::istreambuf_iterator<char>());
            rapidjson::Document json_document;
            json_document.Parse(json_source.c_str(), json_source.size());
            if (json_document.HasParseError()) {
                throw std::runtime_error(utility::formatter() <<
                                                              "json parse error" << json_document.GetParseError() <<
                                                              "(" << json_document.GetErrorOffset() << ")");
            }
            if (json_document.HasMember("width"))
                cx = static_cast<uint32_t>(json_document["width"].GetInt());
            if (json_document.HasMember("height"))
                cy = static_cast<uint32_t>(json_document["height"].GetInt());
            if (json_document.HasMember("samples"))
                samples = static_cast<uint32_t>(json_document["samples"].GetInt());
            if (json_document.HasMember("threads"))
                threads = static_cast<uint32_t>(json_document["threads"].GetInt());
        }
        catch (const std::exception &e)
        {
            std::cout << "error reading configuration: " << e.what() << std::endl;
        }
    }
    return { cx, cy, samples, threads };
}
