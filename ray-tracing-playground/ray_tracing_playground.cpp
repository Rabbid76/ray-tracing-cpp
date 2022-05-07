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
#include <algorithm>
#include <iterator>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define __STDC_LIB_EXT1__
#include <stb/stb_image_write.h>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "core/camera.h"
#include "core/configuration.h"
#include "core/shape.h"
#include "core/shape_list.h"
#include "environment/sky.h"
#include "geometry/sphere.h"
#include "material/lambertian_material.h"
#include "texture/constant_texture.h"

using namespace ray_tracing_core;
using namespace ray_tracing_utility;

template<class T>
class SceneObjectMap
{
private:
    std::map<std::string, T*> object_map;
    std::vector<std::shared_ptr<T>>* object_container;
public:
    SceneObjectMap(std::vector<std::shared_ptr<T>>* object_container)
        : object_container(object_container)
    {}

    void add(const std::string& id, T* object)
    {
        if (object_container)
            object_container->push_back(std::shared_ptr<T>(object));
        object_map.emplace(id, object);
    }

    T* get(const std::string& id) const
    {
        auto it = object_map.find(id);
        return it != object_map.end() ? it->second : nullptr;
    }
};

class SceneReader
{
public:
    using ObjectDecoderMap = std::map<std::string, void (SceneReader::*)(const rapidjson::Document::ConstObject&)>;

private:
    static ObjectDecoderMap object_decoder_map;
    double aspect = 1.0;
    core::SceneObjectContainer scene_objects;
    SceneObjectMap<texture::Texture> texture_map;
    SceneObjectMap<material::Material> material_map;
    SceneObjectMap<geometry::Geometry> geoemtry_map;
    SceneObjectMap<core::ShapeNode> shape_map;
    SceneObjectMap<environment::Sky> environment_map;
    SceneObjectMap<core::Camera> camera_map;
    SceneObjectMap<core::Configuration> configuration_map;
   
public:
    SceneReader(double aspect);
    std::shared_ptr<core::Scene> read_scene_from_json(const rapidjson::Document &doucment);

private: 
    void add_texture(const std::string& id, texture::Texture* texture);
    template<typename T>
    std::string to_string(const T &object);
    std::string read_id(const rapidjson::Value& value);
    std::vector<double> read_array_of_values(const rapidjson::Value &value);
    core::Color read_color(const rapidjson::Value &color_value);
    std::tuple<core::Color, math::AlphaValue> read_color_and_opacity(
        const rapidjson::Document::ConstObject& object, const std::string &color_key, const std::string &opacity_key);
    math::Vector3D read_vector(const rapidjson::Value& color_value);
    math::Point3D read_point(const rapidjson::Value& color_value);
    void read_scene_objects_array(const rapidjson::Value &scene_objects);
    void read_scene_object(const rapidjson::Value &scene_object);
    void read_constant_texture(const rapidjson::Document::ConstObject &scene_object);
    void read_lambertian_material(const rapidjson::Document::ConstObject &scene_object);
    void read_sphere(const rapidjson::Document::ConstObject &scene_object);
    void read_shape(const rapidjson::Document::ConstObject &scene_object);
    void read_collection(const rapidjson::Document::ConstObject &scene_object);
    void read_sky(const rapidjson::Document::ConstObject &scene_object);
    void read_camera_look_at(const rapidjson::Document::ConstObject &scene_object);
    void read_configuration(const rapidjson::Document::ConstObject &scene_object);
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
    const uint32_t cx = 400;
    const uint32_t cy = 200;
    const uint32_t samples = 10;
    const double aspect = static_cast<double>(cx) / static_cast<double>(cy);
    std::shared_ptr<core::Scene> scene;
    SceneReader reader(aspect);
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
        scene = reader.read_scene_from_json(scene_document);
    }
    catch (std::exception e)
    {
        std::cout << "error reading scene: " << e.what() << std::endl;
        throw;
    }
 
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

SceneReader::SceneReader(double aspect)
    : aspect(aspect)
    , scene_objects()
    , texture_map(&scene_objects.textures)
    , material_map(&scene_objects.materials)
    , geoemtry_map(&scene_objects.geometries)
    , shape_map(&scene_objects.shape_nodes)
    , environment_map(&scene_objects.skys)
    , camera_map(&scene_objects.cameras)
    , configuration_map(&scene_objects.configurations)
{}

std::shared_ptr<core::Scene> SceneReader::read_scene_from_json(const rapidjson::Document& doucment)
{
    read_scene_objects_array(doucment["objects"]);
    auto configuration = doucment.HasMember("configuration_id")
        ? *configuration_map.get(read_id(doucment["configuration_id"]))
        : core::Configuration();
    auto camera = camera_map.get(read_id(doucment["camera_id"]));
    auto sky = environment_map.get(read_id(doucment["sky_id"]));
    auto world = shape_map.get(read_id(doucment["root_node_id"]));
    std::shared_ptr<core::Scene> scene = std::make_unique<core::Scene>(configuration, *camera, *sky, *world);
    return scene;
}

template<typename T>
std::string SceneReader::to_string(const T &object)
{
    rapidjson::StringBuffer string_buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(string_buffer);
    object.Accept(writer);
    return string_buffer.GetString();
}

std::string SceneReader::read_id(const rapidjson::Value& value) {
    return value.IsNumber()
        ? std::to_string(value.GetInt64())
        : value.GetString();
}

std::vector<double> SceneReader::read_array_of_values(const rapidjson::Value &value)
{
    if (value.IsNumber())
        return { value.GetDouble() };
    std::vector<double> array_values;
    const auto& color_channels = value.GetArray();
    std::transform(color_channels.begin(), color_channels.end(), 
        std::back_inserter(array_values), [](const rapidjson::Value &array_value) -> double
        {
            return array_value.GetDouble();
        });
    return array_values;
}

core::Color SceneReader::read_color(const rapidjson::Value &color_value)
{
    auto array_values = read_array_of_values(color_value);
    if (array_values.size() == 1)
        return core::Color(static_cast<float>(array_values[0]));
    if (array_values.size() != 3)
        throw std::runtime_error(utility::formatter() << "\"" << to_string(color_value) << "\"" << " is not a RGB color");
    return core::Color(
        static_cast<float>(array_values[0]), static_cast<float>(array_values[1]), static_cast<float>(array_values[2]));
}

std::tuple<core::Color, math::AlphaValue> SceneReader::read_color_and_opacity(
    const rapidjson::Document::ConstObject& object, 
    const std::string& color_key, 
    const std::string& opacity_key) 
{
    auto& color_value = object[color_key.c_str()];
    auto array_values = read_array_of_values(color_value);
    core::Color color;
    if (array_values.size() == 1)
        color = core::Color(static_cast<float>(array_values[0]));
    else if (array_values.size() == 3 || array_values.size() == 4)
        color = core::Color(
            static_cast<float>(array_values[0]), static_cast<float>(array_values[1]), static_cast<float>(array_values[2]));
    else
        throw std::runtime_error(utility::formatter() << "\"" << to_string(color_value) << "\"" << " is not a RGB color");
    math::AlphaValue opacity = 1.0f;
    if (array_values.size() == 4)
        opacity *= static_cast<float>(array_values[3]);
    if (object.HasMember(opacity_key.c_str()))
        opacity *= object[opacity_key.c_str()].GetFloat();
    return { color, opacity };
}

math::Vector3D SceneReader::read_vector(const rapidjson::Value& color_value) {
    auto array_values = read_array_of_values(color_value);
    if (array_values.size() == 1)
        return math::Vector3D(array_values[0]);
    if (array_values.size() != 3)
        throw std::runtime_error(utility::formatter() << "\"" << to_string(color_value) << "\"" << " is not vector");
    return math::Vector3D(array_values[0], array_values[1], array_values[2]);
}

math::Point3D SceneReader::read_point(const rapidjson::Value& color_value) {
    auto array_values = read_array_of_values(color_value);
    if (array_values.size() == 1)
        return math::Point3D(array_values[0]);
    if (array_values.size() != 3)
        throw std::runtime_error(utility::formatter() << "\"" << to_string(color_value) << "\"" << " is not point");
    return math::Point3D(array_values[0], array_values[1], array_values[2]);
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
    auto id = read_id(scene_object["id"]);
    auto [color, opacity] = read_color_and_opacity(scene_object, "color", "opacity");
    texture_map.add(id, new texture::ConstantTexture(color, opacity));
}

void SceneReader::read_lambertian_material(const rapidjson::Document::ConstObject& scene_object)
{
    auto id = read_id(scene_object["id"]);
    auto albedo = texture_map.get(read_id(scene_object["albedo"]));
    material_map.add(id, new material::LambertianMaterial(albedo));
}

void SceneReader::read_sphere(const rapidjson::Document::ConstObject& scene_object)
{
    auto id = read_id(scene_object["id"]);
    auto center = read_point(scene_object["center"]);
    auto radius = scene_object["radius"].GetDouble();
    geoemtry_map.add(id, new geometry::Sphere(center, radius));
}

void SceneReader::read_shape(const rapidjson::Document::ConstObject& scene_object)
{
    auto id = read_id(scene_object["id"]);
    auto geometry = geoemtry_map.get(read_id(scene_object["geometry"]));
    auto material = material_map.get(read_id(scene_object["material"]));
    shape_map.add(id, new core::Shape(geometry, material));
}

void SceneReader::read_collection(const rapidjson::Document::ConstObject& scene_object)
{
    auto id = read_id(scene_object["id"]);
    const auto &array_of_shapes_ids = scene_object["shapes"].GetArray();
    std::vector<const core::ShapeNode*> shapes;
    std::transform(array_of_shapes_ids.begin(), array_of_shapes_ids.end(), std::back_inserter(shapes),
        [&](const rapidjson::Value& array_value) -> const core::ShapeNode*
        {
            return shape_map.get(read_id(array_value));
        });
    shape_map.add(id, new core::ShapeList(shapes));
}

void SceneReader::read_sky(const rapidjson::Document::ConstObject& scene_object)
{
    auto id = read_id(scene_object["id"]);
    auto nadir = read_color(scene_object["nadir"]);
    auto zenith = read_color(scene_object["zenith"]);
    environment_map.add(id, new environment::Sky(environment::Sky::new_sky(nadir, zenith)));
}

void SceneReader::read_camera_look_at(const rapidjson::Document::ConstObject& scene_object)
{
    double adepture = 0;
    math::Distance focus_distance = 1;
    math::TimeRange time_range = math::TimeRange{ 0, 0 };

    auto id = read_id(scene_object["id"]);
    math::Point3D look_from = read_point(scene_object["look_from"]);
    const math::Point3D look_at = read_point(scene_object["look_at"]);
    const math::Vector3D up_vector = read_vector(scene_object["up"]);
    double field_of_view_y = scene_object["fov"].GetDouble();
    camera_map.add(id, new core::Camera(
        core::Camera::new_camera_from_look_at(
            look_from, look_at, up_vector, field_of_view_y, aspect, adepture, focus_distance, time_range)));
}

void SceneReader::read_configuration(const rapidjson::Document::ConstObject& scene_object)
{
    auto id = read_id(scene_object["id"]);
    auto maximum_depth = static_cast<uint32_t>(scene_object["maximum_depth"].GetInt());
    configuration_map.add(id, new core::Configuration{ maximum_depth });
}
