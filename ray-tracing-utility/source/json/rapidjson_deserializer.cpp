#include "json/rapidjson_deserializer.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "core/bvh_tree.h"
#include "core/camera.h"
#include "core/configuration.h"
#include "core/scene.h"
#include "core/shape.h"
#include "core/shape_list.h"
#include "environment/sky.h"
#include "geometry/rectangle.h"
#include "geometry/sphere.h"
#include "geometry/transform.h"
#include "material/blend_materials.h"
#include "material/dielectric_material.h"
#include "material/lambertian_material.h"
#include "material/metal_material.h"
#include "math/checker_blend_function.h"
#include "math/image_channel_blend_function.h"
#include "math/perlin_noise_blend_function.h"
#include "math/test_blend_function.h"
#include "texture/blend_textures.h"
#include "texture/constant_texture.h"
#include "texture/image_texture.h"
#include "utility/std_helper.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <string>

using namespace ray_tracing_core;
using namespace ray_tracing_utility;
using namespace ray_tracing_utility::json;

RapidjsonSceneDeserializer::RapidjsonSceneDeserializer(
    core::SceneObjectContainer *scene_objects, 
    double aspect)
    : aspect(aspect)
    , scene_objects(scene_objects)
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

    if (json_document.HasMember("objects"))
        read_scene_objects_array(json_document["objects"]);
    auto configuration = json_document.HasMember("configuration_id")
        ? *read_configuration(json_document["configuration_id"])
        : core::Configuration();
    auto camera = read_camera(json_document["camera_id"]);
    auto sky = read_environment(json_document["sky_id"]);
    const auto &root_ids_value = json_document["root_node_ids"];
    core::ShapeNode* world = root_ids_value.IsArray()
        ? read_collection(root_ids_value.GetArray())
        : shape_map.get(read_id(root_ids_value));
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

std::tuple<double, double> RapidjsonSceneDeserializer::read_range(const rapidjson::Value& range_value)
{
    if (range_value.IsNumber())
    {
        auto decimal_value = range_value.GetDouble();
        return {decimal_value, decimal_value};
    }
    auto array_values = read_array_of_values(range_value);
    if (array_values.size() != 2)
        throw std::runtime_error(utility::formatter() << "\"" << to_string(range_value) << "\"" << " is not a range");
    return {array_values[0], array_values[1]};
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

math::Vector2D RapidjsonSceneDeserializer::read_vector_2d(const rapidjson::Value& color_value) {
    auto array_values = read_array_of_values(color_value);
    if (array_values.size() == 1)
        return math::Vector2D(array_values[0]);
    if (array_values.size() != 2)
        throw std::runtime_error(utility::formatter() << "\"" << to_string(color_value) << "\"" << " is not 2D vector");
    return { array_values[0], array_values[1] };
}

math::Vector3D RapidjsonSceneDeserializer::read_vector_3d(const rapidjson::Value& color_value) {
    auto array_values = read_array_of_values(color_value);
    if (array_values.size() == 1)
        return math::Vector3D(array_values[0]);
    if (array_values.size() != 3)
        throw std::runtime_error(utility::formatter() << "\"" << to_string(color_value) << "\"" << " is not 3D vector");
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
    if (auto texture = read_texture(object_value))
        return;
    if (auto material = read_material(object_value))
        return;
    if (auto geometry = read_geometry(object_value))
        return;
    if (auto node = read_node(object_value))
        return;
    if (auto node = read_environment(object_value))
        return;
    if (auto node = read_camera(object_value))
        return;
    if (auto node = read_configuration(object_value))
        return;

    auto scene_object = object_value.GetObject();
    std::string type = scene_object["type"].GetString();
    throw std::runtime_error(utility::formatter() << R"(Unknown "type": ")" << type << "\"");
}

ray_tracing_core::math::BlendFunction* RapidjsonSceneDeserializer::read_blend_function(const rapidjson::Value& object_value)
{
    static std::map<std::string, ray_tracing_core::math::BlendFunction* (RapidjsonSceneDeserializer::*)(const rapidjson::Document::ConstObject&)>
        decoder_map =
    {
        { "CheckerBlendFunction", &RapidjsonSceneDeserializer::read_checker_blend_function },
        { "ImageChannelBlendFunction", &RapidjsonSceneDeserializer::read_image_channel_blend_function },
        { "PerlinNoiseBlendFunction", &RapidjsonSceneDeserializer::read_perlin_noise_blend_function },
        { "TestBlendFunction", &RapidjsonSceneDeserializer::read_test_blend_function },
    };

    if (!object_value.IsObject())
        return blend_function_map.get(read_id(object_value));

    auto scene_object = object_value.GetObject();
    auto decoder_it = decoder_map.find(scene_object["type"].GetString());
    if (decoder_it == decoder_map.end())
        return nullptr;

    auto blend_function = (this->*decoder_it->second)(scene_object);
    if (scene_objects)
        scene_objects->blend_functions.push_back(std::shared_ptr<math::BlendFunction>(blend_function));
    if (scene_object.HasMember("id"))
    {
        auto id = read_id(scene_object["id"]);
        blend_function_map.add(id, blend_function);
    }
    return blend_function;
}

texture::Texture* RapidjsonSceneDeserializer::read_texture(const rapidjson::Value& object_value)
{
    static std::map<std::string, texture::Texture* (RapidjsonSceneDeserializer::*)(const rapidjson::Document::ConstObject&)>
        decoder_map =
    {
        { "ConstantTexture", &RapidjsonSceneDeserializer::read_constant_texture },
        { "BlendTextures", &RapidjsonSceneDeserializer::read_blend_textures },
        { "ImageTextures", &RapidjsonSceneDeserializer::read_image_texture },
    };

    if (!object_value.IsObject())
        return texture_map.get(read_id(object_value));

    auto scene_object = object_value.GetObject();
    auto decoder_it = decoder_map.find(scene_object["type"].GetString());
    if (decoder_it == decoder_map.end())
        return nullptr;

    auto texture = (this->*decoder_it->second)(scene_object);
    if (scene_objects)
        scene_objects->textures.push_back(std::shared_ptr<texture::Texture>(texture));
    if (scene_object.HasMember("id"))
    {
        auto id = read_id(scene_object["id"]);
        texture_map.add(id, texture);
    }
    return texture;
}

material::Material* RapidjsonSceneDeserializer::read_material(const rapidjson::Value& object_value)
{
    static std::map<std::string, material::Material* (RapidjsonSceneDeserializer::*)(const rapidjson::Document::ConstObject&)>
        decoder_map =
    {
        { "BlendMaterials", &RapidjsonSceneDeserializer::read_blend_materials },
        { "LambertianMaterial", &RapidjsonSceneDeserializer::read_lambertian_material },
        { "MetalMaterial", &RapidjsonSceneDeserializer::read_metal_material },
        { "DielectricMaterial", &RapidjsonSceneDeserializer::read_dielectric_material },
    };

    if (!object_value.IsObject())
        return material_map.get(read_id(object_value));

    auto scene_object = object_value.GetObject();
    auto decoder_it = decoder_map.find(scene_object["type"].GetString());
    if (decoder_it == decoder_map.end())
        return nullptr;

    auto material = (this->*decoder_it->second)(scene_object);
    if (scene_objects)
        scene_objects->materials.push_back(std::shared_ptr<material::Material>(material));
    if (scene_object.HasMember("id"))
    {
        auto id = read_id(scene_object["id"]);
        material_map.add(id, material);
    }
    return material;
}

geometry::Geometry* RapidjsonSceneDeserializer::read_geometry(const rapidjson::Value& object_value)
{
    static std::map<std::string, geometry::Geometry* (RapidjsonSceneDeserializer::*)(const rapidjson::Document::ConstObject&)>
        decoder_map =
    {
        { "Rectangle", &RapidjsonSceneDeserializer::read_rectangle },
        { "Sphere", &RapidjsonSceneDeserializer::read_sphere },
        { "Transform", &RapidjsonSceneDeserializer::read_transform },
    };

    if (!object_value.IsObject())
        return geometry_map.get(read_id(object_value));

    auto scene_object = object_value.GetObject();
    auto decoder_it = decoder_map.find(scene_object["type"].GetString());
    if (decoder_it == decoder_map.end())
        return nullptr;

    auto geometry = (this->*decoder_it->second)(scene_object);
    if (scene_objects)
        scene_objects->geometries.push_back(std::shared_ptr<geometry::Geometry>(geometry));
    if (scene_object.HasMember("id"))
    {
        auto id = read_id(scene_object["id"]);
        geometry_map.add(id, geometry);
    }
    return geometry;
}

core::ShapeNode* RapidjsonSceneDeserializer::read_node(const rapidjson::Value& object_value)
{
    static std::map<std::string, core::ShapeNode* (RapidjsonSceneDeserializer::*)(const rapidjson::Document::ConstObject&)>
        decoder_map =
    {
        { "Shape", &RapidjsonSceneDeserializer::read_shape },
        { "Collection", &RapidjsonSceneDeserializer::read_collection },
    };

    if (!object_value.IsObject())
        return shape_map.get(read_id(object_value));

    auto scene_object = object_value.GetObject();
    auto decoder_it = decoder_map.find(scene_object["type"].GetString());
    if (decoder_it == decoder_map.end())
        return nullptr;

    auto node = (this->*decoder_it->second)(scene_object);
    if (scene_objects)
        scene_objects->shape_nodes.push_back(std::shared_ptr<core::ShapeNode>(node));
    if (scene_object.HasMember("id"))
    {
        auto id = read_id(scene_object["id"]);
        shape_map.add(id, node);
    }
    return node;
}

environment::Sky* RapidjsonSceneDeserializer::read_environment(const rapidjson::Value& object_value)
{
    static std::map<std::string, environment::Sky* (RapidjsonSceneDeserializer::*)(const rapidjson::Document::ConstObject&)>
        decoder_map =
    {
        { "Sky", &RapidjsonSceneDeserializer::read_sky },
    };

    if (!object_value.IsObject())
        return environment_map.get(read_id(object_value));

    auto scene_object = object_value.GetObject();
    auto decoder_it = decoder_map.find(scene_object["type"].GetString());
    if (decoder_it == decoder_map.end())
        return nullptr;

    auto node = (this->*decoder_it->second)(scene_object);
    if (scene_objects)
        scene_objects->skys.push_back(std::shared_ptr<environment::Sky>(node));
    if (scene_object.HasMember("id"))
    {
        auto id = read_id(scene_object["id"]);
        environment_map.add(id, node);
    }
    return node;
}

core::Camera* RapidjsonSceneDeserializer::read_camera(const rapidjson::Value& object_value)
{
    static std::map<std::string, core::Camera* (RapidjsonSceneDeserializer::*)(const rapidjson::Document::ConstObject&)>
        decoder_map =
    {
        { "CameraLookAt", &RapidjsonSceneDeserializer::read_camera_look_at },
    };

    if (!object_value.IsObject())
        return camera_map.get(read_id(object_value));

    auto scene_object = object_value.GetObject();
    auto decoder_it = decoder_map.find(scene_object["type"].GetString());
    if (decoder_it == decoder_map.end())
        return nullptr;

    auto node = (this->*decoder_it->second)(scene_object);
    if (scene_objects)
        scene_objects->cameras.push_back(std::shared_ptr<core::Camera>(node));
    if (scene_object.HasMember("id"))
    {
        auto id = read_id(scene_object["id"]);
        camera_map.add(id, node);
    }
    return node;
}

core::Configuration* RapidjsonSceneDeserializer::read_configuration(const rapidjson::Value& object_value)
{
    static std::map<std::string, core::Configuration* (RapidjsonSceneDeserializer::*)(const rapidjson::Document::ConstObject&)>
        decoder_map =
    {
        { "Configuration", &RapidjsonSceneDeserializer::read_configuration },
    };

    if (!object_value.IsObject())
        return configuration_map.get(read_id(object_value));

    auto scene_object = object_value.GetObject();
    auto decoder_it = decoder_map.find(scene_object["type"].GetString());
    if (decoder_it == decoder_map.end())
        return nullptr;

    auto node = (this->*decoder_it->second)(scene_object);
    if (scene_objects)
        scene_objects->configurations.push_back(std::shared_ptr<core::Configuration>(node));
    if (scene_object.HasMember("id"))
    {
        auto id = read_id(scene_object["id"]);
        configuration_map.add(id, node);
    }
    return node;
}

ray_tracing_core::math::BlendFunction* RapidjsonSceneDeserializer::read_checker_blend_function(const rapidjson::Document::ConstObject& scene_object)
{
    auto scale = read_vector_3d(scene_object["scale"]);
    return new math::CheckerBlendFunction(scale);
}

ray_tracing_core::math::BlendFunction* RapidjsonSceneDeserializer::read_image_channel_blend_function(const rapidjson::Document::ConstObject& scene_object)
{
    static std::map<std::string, math::ImageChannelBlendFunction::Channel>
        decoder_map =
    {
        { "Red", math::ImageChannelBlendFunction::Channel::Red },
        { "Green", math::ImageChannelBlendFunction::Channel::Green },
        { "Blue", math::ImageChannelBlendFunction::Channel::Blue },
        { "Alpha", math::ImageChannelBlendFunction::Channel::Alpha },
    };

    math::ImageChannelBlendFunction::Channel channel = math::ImageChannelBlendFunction::Channel::Alpha;
    if (scene_object.HasMember("channle"))
    {
        auto it = decoder_map.find(scene_object["channle"].GetString());
        if (it != decoder_map.end())
            channel = it->second;
    }
    texture::Texture* blend_texture = read_texture(scene_object["blend_texture"]);
    return new math::ImageChannelBlendFunction(channel, blend_texture);
}

ray_tracing_core::math::BlendFunction* RapidjsonSceneDeserializer::read_perlin_noise_blend_function(const rapidjson::Document::ConstObject& scene_object)
{
    static std::map<std::string, math::PerlinNoiseBlendFunction::Type>
        decoder_map =
    {
        { "Turb", math::PerlinNoiseBlendFunction::Type::Turb },
        { "SinX", math::PerlinNoiseBlendFunction::Type::SinX },
        { "SinY", math::PerlinNoiseBlendFunction::Type::SinY },
        { "SinZ", math::PerlinNoiseBlendFunction::Type::SinZ },
    };

    auto scale = read_vector_3d(scene_object["scale"]);
    auto noise_type = math::PerlinNoiseBlendFunction::Type::Default;
    if (scene_object.HasMember("noise_type"))
    {
        auto it = decoder_map.find(scene_object["noise_type"].GetString());
        if (it != decoder_map.end())
            noise_type = it->second;
    }
    return new math::PerlinNoiseBlendFunction(noise_type, scale);
}

ray_tracing_core::math::BlendFunction* RapidjsonSceneDeserializer::read_test_blend_function(const rapidjson::Document::ConstObject& scene_object)
{
    static std::map<std::string, math::TestBlendFunction::Type>
        decoder_map =
    {
        { "Default", math::TestBlendFunction::Type::Default },
        { "IcosahedronHoles", math::TestBlendFunction::Type::IcosahedronHoles },
    };

    auto blend_type = math::TestBlendFunction::Type::Default;
    if (scene_object.HasMember("blend_type"))
    {
        auto it = decoder_map.find(scene_object["blend_type"].GetString());
        if (it != decoder_map.end())
            blend_type = it->second;
    }
    return new math::TestBlendFunction(blend_type);
}

texture::Texture* RapidjsonSceneDeserializer::read_blend_textures(const rapidjson::Document::ConstObject& scene_object)
{
    auto blend_function = read_blend_function(scene_object["blend_function"]);
    texture::Texture* texture0 = scene_object.HasMember("texture0")
        ? read_texture(scene_object["texture0"])
        : nullptr;
    texture::Texture* texture1 = scene_object.HasMember("texture1")
        ? read_texture(scene_object["texture1"])
        : nullptr;
    return new texture::BlendTextures(blend_function, texture0, texture1);
}

texture::Texture* RapidjsonSceneDeserializer::read_constant_texture(const rapidjson::Document::ConstObject& scene_object)
{
    auto [color, opacity] = read_color_and_opacity(scene_object, "color", "opacity");
    return new texture::ConstantTexture(color, opacity);
}

texture::Texture* RapidjsonSceneDeserializer::read_image_texture(const rapidjson::Document::ConstObject& scene_object)
{
    texture::ImageTexture::Type type = texture::ImageTexture::Type::RGBA;
    if (scene_object.HasMember("image_type"))
    {
        auto image_type = scene_object["image_type"].GetString();
        type = std::string(image_type) == "RGB" ? texture::ImageTexture::Type::RGB : texture::ImageTexture::Type::RGBA;
    }
    
    auto filename = scene_object["filename"].GetString();
    int cx, cy, ch;
    stbi_uc* image_data = stbi_load(filename, &cx, &cy, &ch, type == texture::ImageTexture::Type::RGB ? 3 : 4);
    if (image_data == nullptr)
        throw std::runtime_error(utility::formatter() << "error laoding texture: " << filename);

    auto texture = new texture::ImageTexture(type, (uint32_t)cx, (uint32_t)cy, (const uint8_t*)image_data);
    stbi_image_free(image_data);
    return texture;
}

material::Material* RapidjsonSceneDeserializer::read_blend_materials(const rapidjson::Document::ConstObject& scene_object)
{
    material::BlendMaterials::Materials materials;
    for (auto &material_value : scene_object["materials"].GetArray())
    {
        auto weight = material_value["weight"].GetDouble();
        auto material = read_material(material_value["id"]);
        materials.emplace_back(weight, material);
    }
    return new material::BlendMaterials(materials);
}

material::Material* RapidjsonSceneDeserializer::read_lambertian_material(const rapidjson::Document::ConstObject& scene_object)
{
    auto albedo = read_texture(scene_object["albedo"]);
    return new material::LambertianMaterial(albedo);
}

material::Material* RapidjsonSceneDeserializer::read_metal_material(const rapidjson::Document::ConstObject& scene_object)
{
    auto fuzz = scene_object["fuzz"].GetDouble();
    auto albedo = read_texture(scene_object["albedo"]);
    return new material::MetalMaterial(fuzz, albedo);
}

material::Material* RapidjsonSceneDeserializer::read_dielectric_material(const rapidjson::Document::ConstObject& scene_object)
{
    auto refraction_index = read_range(scene_object["refraction_index"]);
    auto albedo = read_texture(scene_object["albedo"]);
    return new material::DielectricMaterial(refraction_index, albedo);
}

geometry::Geometry* RapidjsonSceneDeserializer::read_rectangle(const rapidjson::Document::ConstObject& scene_object)
{
    static std::map<std::string, math::Rectangle::Orientation>
        decoder_map =
    {
        { "XY", math::Rectangle::Orientation::XY },
        { "XZ", math::Rectangle::Orientation::XZ },
        { "YZ", math::Rectangle::Orientation::YZ },
    };

    math::Rectangle::Orientation orientation = math::Rectangle::Orientation::XY;
    if (scene_object.HasMember("orientation"))
    {
        auto it = decoder_map.find(scene_object["orientation"].GetString());
        if (it != decoder_map.end())
            orientation = it->second;
    }
    auto k = scene_object.HasMember("k")
        ? scene_object["k"].GetDouble()
        : 0;
    auto minimum = scene_object.HasMember("minimum")
        ? read_vector_2d(scene_object["minimum"])
        : math::Vector2D(-1, -1);
    auto maximum = scene_object.HasMember("maximum")
        ? read_vector_2d(scene_object["maximum"])
        : math::Vector2D(1, 1);
    return new geometry::Rectangle(orientation, k, minimum, maximum);
}

geometry::Geometry* RapidjsonSceneDeserializer::read_transform(const rapidjson::Document::ConstObject& scene_object)
{
    auto geometry = read_geometry(scene_object["geometry"]);
    auto translate = scene_object.HasMember("translate")
        ? read_vector_3d(scene_object["translate"])
        : math::Vector3D(0);
    auto angle = scene_object.HasMember("angle")
        ? scene_object["angle"].GetDouble()
        : 0;
    auto axis = scene_object.HasMember("axis")
        ? read_vector_3d(scene_object["axis"])
        : math::Vector3D(1, 0, 0);
    auto scale = scene_object.HasMember("scale")
        ? read_vector_3d(scene_object["scale"])
        : math::Vector3D(1);
    return new geometry::Transform(geometry, translate, angle, axis, scale);
}

geometry::Geometry* RapidjsonSceneDeserializer::read_sphere(const rapidjson::Document::ConstObject& scene_object)
{
    auto center = read_point(scene_object["center"]);
    auto radius = scene_object["radius"].GetDouble();
    return new geometry::Sphere(center, radius);
}

core::ShapeNode* RapidjsonSceneDeserializer::read_shape(const rapidjson::Document::ConstObject& scene_object)
{
    auto geometry = read_geometry(scene_object["geometry"]);
    auto material = read_material(scene_object["material"]);
    return new core::Shape(geometry, material);
}

core::ShapeNode* RapidjsonSceneDeserializer::read_collection(const rapidjson::Document::ConstObject& scene_object)
{
    const auto& array_of_shapes_ids = scene_object["shapes"].GetArray();
    return read_collection(array_of_shapes_ids);
}

core::ShapeNode* RapidjsonSceneDeserializer::read_collection(const rapidjson::Document::ConstArray& aray_object)
{
    std::vector<const core::ShapeNode*> shapes;
    std::transform(aray_object.begin(), aray_object.end(), std::back_inserter(shapes),
        [&](const rapidjson::Value& array_value) -> const core::ShapeNode*
        {
            return read_node(array_value);
        });
    return new core::BvhTree(shapes);
}

environment::Sky* RapidjsonSceneDeserializer::read_sky(const rapidjson::Document::ConstObject& scene_object)
{
    auto nadir = read_color(scene_object["nadir"]);
    auto zenith = read_color(scene_object["zenith"]);
    return new environment::Sky(environment::Sky::new_sky(nadir, zenith));
}

core::Camera* RapidjsonSceneDeserializer::read_camera_look_at(const rapidjson::Document::ConstObject& scene_object)
{
    double adepture = 0;
    math::Distance focus_distance = 1;
    math::TimeRange time_range = math::TimeRange{ 0, 0 };

    math::Point3D look_from = read_point(scene_object["look_from"]);
    const math::Point3D look_at = read_point(scene_object["look_at"]);
    const math::Vector3D up_vector = read_vector_3d(scene_object["up"]);
    double field_of_view_y = scene_object["fov"].GetDouble();
    return new core::Camera(
        core::Camera::new_camera_from_look_at(
            look_from, look_at, up_vector, field_of_view_y, aspect, adepture, focus_distance, time_range));
}

core::Configuration* RapidjsonSceneDeserializer::read_configuration(const rapidjson::Document::ConstObject& scene_object)
{
    auto maximum_depth = static_cast<uint32_t>(scene_object["maximum_depth"].GetInt());
    return new core::Configuration{ maximum_depth };
}
