#include "core/configuration.h"
#include "core/scene_objects_container.h"
#include "core/test_scene_factory.h"
#include "renderer/renderer_async.h"
#include "utility/std_helper.h"
#include "viewer/viewer_cimg.h"
#include <exception>
#include <stdexcept>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <map>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define __STDC_LIB_EXT1__
#include <stb/stb_image_write.h>
#include "rapidjson/document.h"

using namespace ray_tracing_core;
using namespace ray_tracing_utility;


class SceneReader
{
public:
    using ObjectDecoderMap = std::map<std::string, void (SceneReader::*)(const rapidjson::Document::ConstObject&)>;

private:
    static ObjectDecoderMap object_decoder_map;
    core::SceneObjectContainer scene_objects;
   
public:
    void read_scene_from_json(const rapidjson::Document &doucment);

private: 
    void read_scene_objects_array(const rapidjson::Value &scene_objects);
    void read_scene_object(const rapidjson::Value &scene_object);
    void read_constant_texture(const rapidjson::Document::ConstObject &scene_object);
    void read_lambertian_material(const rapidjson::Document::ConstObject& scene_object);
    void read_sphere(const rapidjson::Document::ConstObject& scene_object);
    void read_shape(const rapidjson::Document::ConstObject& scene_object);
    void read_collection(const rapidjson::Document::ConstObject& scene_object);
    void read_sky(const rapidjson::Document::ConstObject& scene_object);
    void read_camera_look_at(const rapidjson::Document::ConstObject& scene_object);
    void read_configuration(const rapidjson::Document::ConstObject& scene_object);
};

SceneReader::ObjectDecoderMap SceneReader::object_decoder_map =
{
    { "ConstantTexture", &SceneReader::read_constant_texture },
    { "LambertianMaterial", &SceneReader::read_lambertian_material },
    { "Sphere", &SceneReader::read_sphere },
    { "Shape", &SceneReader::read_shape },
    { "Collection", &SceneReader::read_collection },
    { "Sky", &SceneReader::read_sky },
    { "CameraLookAt", &SceneReader::read_camera_look_at },
    { "Configuration", &SceneReader::read_configuration },
};

int main()
{
    SceneReader reader;
    rapidjson::Document scene_document;
    std::fstream scene_json;
    scene_json.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        scene_json.open("scenes/test_scene.json");
        auto json_source = std::string(std::istreambuf_iterator<char>(scene_json), std::istreambuf_iterator<char>());
        scene_document.Parse(json_source.c_str(), json_source.size());
        if (scene_document.HasParseError()) {
            throw std::runtime_error(utility::formatter() <<
                "json parse error" << scene_document.GetParseError() <<
                "(" << scene_document.GetErrorOffset() << ")");
        }
        reader.read_scene_from_json(scene_document);
    }
    catch (std::exception e)
    {
        std::cout << "error reading scene: " << e.what() << std::endl;
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

void SceneReader::read_scene_from_json(const rapidjson::Document& doucment)
{
    read_scene_objects_array(doucment["objects"]);
}

void SceneReader::read_scene_objects_array(const rapidjson::Value &objects_value)
{
    for (auto &scene_object : objects_value.GetArray()) 
        read_scene_object(scene_object);
}

void SceneReader::read_scene_object(const rapidjson::Value &object_value) {
    auto scene_object = object_value.GetObject();
    auto &type_value = scene_object["type"];
    std::string type = type_value.GetString();
    auto decoder_it = object_decoder_map.find(type);
    if (decoder_it == object_decoder_map.end())
        throw std::runtime_error(utility::formatter() << "Unknown \"type\": \"" << type << "\"");
    (this->*decoder_it->second)(scene_object);
}

void SceneReader::read_constant_texture(const rapidjson::Document::ConstObject& scene_object)
{
    auto& id_value = scene_object["id"];
}

void SceneReader::read_lambertian_material(const rapidjson::Document::ConstObject& scene_object)
{
    auto& id_value = scene_object["id"];
}

void SceneReader::read_sphere(const rapidjson::Document::ConstObject& scene_object)
{
    auto& id_value = scene_object["id"];
}

void SceneReader::read_shape(const rapidjson::Document::ConstObject& scene_object)
{
    auto& id_value = scene_object["id"];
}

void SceneReader::read_collection(const rapidjson::Document::ConstObject& scene_object)
{
    auto& id_value = scene_object["id"];
}

void SceneReader::read_sky(const rapidjson::Document::ConstObject& scene_object)
{
    auto& id_value = scene_object["id"];
}

void SceneReader::read_camera_look_at(const rapidjson::Document::ConstObject& scene_object)
{
    auto& id_value = scene_object["id"];
}

void SceneReader::read_configuration(const rapidjson::Document::ConstObject& scene_object)
{
    auto& id_value = scene_object["id"];
}
