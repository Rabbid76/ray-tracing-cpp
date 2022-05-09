#include "json/rapidjson_deserializer.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "core/camera.h"
#include "core/configuration.h"
#include "core/scene.h"
#include "core/shape.h"
#include "core/shape_list.h"
#include "environment/sky.h"
#include "geometry/sphere.h"
#include "material/lambertian_material.h"
#include "material/metal_material_test.h"
#include "texture/constant_texture.h"
#include "utility/std_helper.h"
#include <string>

using namespace ray_tracing_core;
using namespace ray_tracing_utility;
using namespace ray_tracing_utility::json;

RapidjsonSceneDeserializer::ObjectDecoderMap RapidjsonSceneDeserializer::object_decoder_map =
{
    { "ConstantTexture", &RapidjsonSceneDeserializer::read_constant_texture },
    { "LambertianMaterial", &RapidjsonSceneDeserializer::read_lambertian_material },
    { "MetalMaterial", &RapidjsonSceneDeserializer::read_metal_material },
    { "Sphere", &RapidjsonSceneDeserializer::read_sphere },
    { "Shape", &RapidjsonSceneDeserializer::read_shape },
    { "Collection", &RapidjsonSceneDeserializer::read_collection },
    { "Sky", &RapidjsonSceneDeserializer::read_sky },
    { "CameraLookAt", &RapidjsonSceneDeserializer::read_camera_look_at },
    { "Configuration", &RapidjsonSceneDeserializer::read_configuration },
};

RapidjsonSceneDeserializer::RapidjsonSceneDeserializer(
    core::SceneObjectContainer *scene_objects, 
    double aspect)
    : aspect(aspect)
    , texture_map(&scene_objects->textures)
    , material_map(&scene_objects->materials)
    , geoemtry_map(&scene_objects->geometries)
    , shape_map(&scene_objects->shape_nodes)
    , environment_map(&scene_objects->skys)
    , camera_map(&scene_objects->cameras)
    , configuration_map(&scene_objects->configurations)
{}

core::Scene* RapidjsonSceneDeserializer::read_scene_from_json(const std::string &serialized_json)
{
    rapidjson::Document json_document;
    json_document.Parse(serialized_json.c_str(), serialized_json.size());
    if (json_document.HasParseError()) {
        throw std::runtime_error(utility::formatter() <<
              "json parse error" << json_document.GetParseError() <<
              "(" << json_document.GetErrorOffset() << ")");
    }

    read_scene_objects_array(json_document["objects"]);
    auto configuration = json_document.HasMember("configuration_id")
        ? *configuration_map.get(read_id(json_document["configuration_id"]))
        : core::Configuration();
    auto camera = camera_map.get(read_id(json_document["camera_id"]));
    auto sky = environment_map.get(read_id(json_document["sky_id"]));
    auto world = shape_map.get(read_id(json_document["root_node_id"]));
    auto *scene = new core::Scene(configuration, *camera, *sky, *world);
    return scene;
}

template<typename T>
std::string RapidjsonSceneDeserializer::to_string(const T& object)
{
    rapidjson::StringBuffer string_buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(string_buffer);
    object.Accept(writer);
    return string_buffer.GetString();
}

std::string RapidjsonSceneDeserializer::read_id(const rapidjson::Value& value) {
    return value.IsNumber()
        ? std::to_string(value.GetInt64())
        : value.GetString();
}

std::vector<double> RapidjsonSceneDeserializer::read_array_of_values(const rapidjson::Value& value)
{
    if (value.IsNumber())
        return { value.GetDouble() };
    std::vector<double> array_values;
    const auto& color_channels = value.GetArray();
    std::transform(color_channels.begin(), color_channels.end(),
        std::back_inserter(array_values), [](const rapidjson::Value& array_value) -> double
        {
            return array_value.GetDouble();
        });
    return array_values;
}

core::Color RapidjsonSceneDeserializer::read_color(const rapidjson::Value& color_value)
{
    auto array_values = read_array_of_values(color_value);
    if (array_values.size() == 1)
        return core::Color(static_cast<float>(array_values[0]));
    if (array_values.size() != 3)
        throw std::runtime_error(utility::formatter() << "\"" << to_string(color_value) << "\"" << " is not a RGB color");
    return {
        static_cast<float>(array_values[0]), static_cast<float>(array_values[1]), static_cast<float>(array_values[2])};
}

std::tuple<core::Color, math::AlphaValue> RapidjsonSceneDeserializer::read_color_and_opacity(
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

math::Vector3D RapidjsonSceneDeserializer::read_vector(const rapidjson::Value& color_value) {
    auto array_values = read_array_of_values(color_value);
    if (array_values.size() == 1)
        return math::Vector3D(array_values[0]);
    if (array_values.size() != 3)
        throw std::runtime_error(utility::formatter() << "\"" << to_string(color_value) << "\"" << " is not vector");
    return {array_values[0], array_values[1], array_values[2]};
}

math::Point3D RapidjsonSceneDeserializer::read_point(const rapidjson::Value& color_value) {
    auto array_values = read_array_of_values(color_value);
    if (array_values.size() == 1)
        return math::Point3D(array_values[0]);
    if (array_values.size() != 3)
        throw std::runtime_error(utility::formatter() << "\"" << to_string(color_value) << "\"" << " is not point");
    return {array_values[0], array_values[1], array_values[2]};
}

void RapidjsonSceneDeserializer::read_scene_objects_array(const rapidjson::Value& objects_value)
{
    for (auto& scene_object : objects_value.GetArray())
        read_scene_object(scene_object);
}

void RapidjsonSceneDeserializer::read_scene_object(const rapidjson::Value& object_value) {
    auto scene_object = object_value.GetObject();
    auto& type_value = scene_object["type"];
    std::string type = type_value.GetString();
    auto decoder_it = object_decoder_map.find(type);
    if (decoder_it == object_decoder_map.end())
        throw std::runtime_error(utility::formatter() << R"(Unknown "type": ")" << type << "\"");
    (this->*decoder_it->second)(scene_object);
}

void RapidjsonSceneDeserializer::read_constant_texture(const rapidjson::Document::ConstObject& scene_object)
{
    auto id = read_id(scene_object["id"]);
    auto [color, opacity] = read_color_and_opacity(scene_object, "color", "opacity");
    texture_map.add(id, new texture::ConstantTexture(color, opacity));
}

void RapidjsonSceneDeserializer::read_lambertian_material(const rapidjson::Document::ConstObject& scene_object)
{
    auto id = read_id(scene_object["id"]);
    auto albedo = texture_map.get(read_id(scene_object["albedo"]));
    material_map.add(id, new material::LambertianMaterial(albedo));
}

void RapidjsonSceneDeserializer::read_metal_material(const rapidjson::Document::ConstObject& scene_object)
{
    auto id = read_id(scene_object["id"]);
    auto fuzz = scene_object["fuzz"].GetDouble();
    auto albedo = texture_map.get(read_id(scene_object["albedo"]));
    material_map.add(id, new material::MetalMaterial(fuzz, albedo));
}

void RapidjsonSceneDeserializer::read_sphere(const rapidjson::Document::ConstObject& scene_object)
{
    auto id = read_id(scene_object["id"]);
    auto center = read_point(scene_object["center"]);
    auto radius = scene_object["radius"].GetDouble();
    geoemtry_map.add(id, new geometry::Sphere(center, radius));
}

void RapidjsonSceneDeserializer::read_shape(const rapidjson::Document::ConstObject& scene_object)
{
    auto id = read_id(scene_object["id"]);
    auto geometry = geoemtry_map.get(read_id(scene_object["geometry"]));
    auto material = material_map.get(read_id(scene_object["material"]));
    shape_map.add(id, new core::Shape(geometry, material));
}

void RapidjsonSceneDeserializer::read_collection(const rapidjson::Document::ConstObject& scene_object)
{
    auto id = read_id(scene_object["id"]);
    const auto& array_of_shapes_ids = scene_object["shapes"].GetArray();
    std::vector<const core::ShapeNode*> shapes;
    std::transform(array_of_shapes_ids.begin(), array_of_shapes_ids.end(), std::back_inserter(shapes),
        [&](const rapidjson::Value& array_value) -> const core::ShapeNode*
        {
            return shape_map.get(read_id(array_value));
        });
    shape_map.add(id, new core::ShapeList(shapes));
}

void RapidjsonSceneDeserializer::read_sky(const rapidjson::Document::ConstObject& scene_object)
{
    auto id = read_id(scene_object["id"]);
    auto nadir = read_color(scene_object["nadir"]);
    auto zenith = read_color(scene_object["zenith"]);
    environment_map.add(id, new environment::Sky(environment::Sky::new_sky(nadir, zenith)));
}

void RapidjsonSceneDeserializer::read_camera_look_at(const rapidjson::Document::ConstObject& scene_object)
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

void RapidjsonSceneDeserializer::read_configuration(const rapidjson::Document::ConstObject& scene_object)
{
    auto id = read_id(scene_object["id"]);
    auto maximum_depth = static_cast<uint32_t>(scene_object["maximum_depth"].GetInt());
    configuration_map.add(id, new core::Configuration{ maximum_depth });
}
