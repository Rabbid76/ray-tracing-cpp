#ifndef __RTU__JSON__RAPIDJSON_DESERIALIZER_H__
#define __RTU__JSON__RAPIDJSON_DESERIALIZER_H__

#include "core/scene.h"
#include "core/scene_objects_container.h"
#include "math/blend_function.h"
#include "math/types.h"
#include "rapidjson/document.h"
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace ray_tracing_utility::json
{
    template<class T>
    class SceneObjectMap
    {
    private:
        std::map<std::string, T*> object_map;
    public:
        SceneObjectMap()
        {}

        void add(const std::string& id, T* object)
        {
            object_map.emplace(id, object);
        }

        T* get(const std::string& id) const
        {
            auto it = object_map.find(id);
            return it != object_map.end() ? it->second : nullptr;
        }
    };

	class RapidjsonSceneDeserializer
	{
    public:
        using ObjectDecoderMap = std::map<std::string, void (RapidjsonSceneDeserializer::*)(const rapidjson::Document::ConstObject&)>;

    private:
        double aspect = 1.0;
        ray_tracing_core::core::SceneObjectContainer* scene_objects;
        SceneObjectMap<ray_tracing_core::math::BlendFunction> blend_function_map;
        SceneObjectMap<ray_tracing_core::texture::Texture> texture_map;
        SceneObjectMap<ray_tracing_core::material::Material> material_map;
        SceneObjectMap<ray_tracing_core::geometry::Geometry> geometry_map;
        SceneObjectMap<ray_tracing_core::core::ShapeNode> shape_map;
        SceneObjectMap<ray_tracing_core::environment::Sky> environment_map;
        SceneObjectMap<ray_tracing_core::core::Camera> camera_map;
        SceneObjectMap<ray_tracing_core::core::Configuration> configuration_map;

    public:
        RapidjsonSceneDeserializer(ray_tracing_core::core::SceneObjectContainer *scene_objects, double aspect);
        ray_tracing_core::core::Scene* read_scene_from_json(const std::string &serialized_json);

    private:
        template<typename T>
        static std::string to_string(const T& object);
        static std::string read_id(const rapidjson::Value& value);
        static std::tuple<double, double> read_range(const rapidjson::Value& range_value);
        static std::vector<double> read_array_of_values(const rapidjson::Value& value);
        ray_tracing_core::core::Color read_color(const rapidjson::Value& color_value);
        std::tuple<ray_tracing_core::core::Color, ray_tracing_core::math::AlphaValue> read_color_and_opacity(
            const rapidjson::Document::ConstObject& object, const std::string& color_key, const std::string& opacity_key);
        ray_tracing_core::math::Vector2D read_vector_2d(const rapidjson::Value& color_value);
        ray_tracing_core::math::Vector3D read_vector_3d(const rapidjson::Value& color_value);
        ray_tracing_core::math::Point3D read_point(const rapidjson::Value& color_value);
        void read_scene_objects_array(const rapidjson::Value& scene_objects);
        void read_scene_object(const rapidjson::Value& scene_object);
        ray_tracing_core::math::BlendFunction* read_blend_function(const rapidjson::Value& object_value);
        ray_tracing_core::texture::Texture* read_texture(const rapidjson::Value& object_value);
        ray_tracing_core::material::Material* read_material(const rapidjson::Value& object_value);
        ray_tracing_core::geometry::Geometry* read_geometry(const rapidjson::Value& object_value);
        ray_tracing_core::core::ShapeNode* read_node(const rapidjson::Value& object_value);
        ray_tracing_core::environment::Sky* read_environment(const rapidjson::Value& object_value);
        ray_tracing_core::core::Camera* read_camera(const rapidjson::Value& object_value);
        ray_tracing_core::core::Configuration* read_configuration(const rapidjson::Value& object_value);
        ray_tracing_core::math::BlendFunction* read_checker_blend_function(const rapidjson::Document::ConstObject& scene_object);
        ray_tracing_core::math::BlendFunction* read_image_channel_blend_function(const rapidjson::Document::ConstObject& scene_object);
        ray_tracing_core::math::BlendFunction* read_perlin_noise_blend_function(const rapidjson::Document::ConstObject& scene_object);
        ray_tracing_core::math::BlendFunction* read_test_blend_function(const rapidjson::Document::ConstObject& scene_object);
        ray_tracing_core::texture::Texture* read_constant_texture(const rapidjson::Document::ConstObject& scene_object);
        ray_tracing_core::texture::Texture* read_blend_textures(const rapidjson::Document::ConstObject& scene_object);
        ray_tracing_core::texture::Texture* read_image_texture(const rapidjson::Document::ConstObject& scene_object);
        ray_tracing_core::material::Material* read_blend_materials(const rapidjson::Document::ConstObject& scene_object);
        ray_tracing_core::material::Material* read_lambertian_material(const rapidjson::Document::ConstObject& scene_object);
        ray_tracing_core::material::Material* read_metal_material(const rapidjson::Document::ConstObject& scene_object);
        ray_tracing_core::material::Material* read_dielectric_material(const rapidjson::Document::ConstObject& scene_object);
        ray_tracing_core::material::Material* read_diffuse_light(const rapidjson::Document::ConstObject& scene_object);
        ray_tracing_core::geometry::Geometry* read_sphere(const rapidjson::Document::ConstObject& scene_object);
        ray_tracing_core::geometry::Geometry* read_rectangle(const rapidjson::Document::ConstObject& scene_object);
        ray_tracing_core::geometry::Geometry* read_transform(const rapidjson::Document::ConstObject& scene_object);
        ray_tracing_core::core::ShapeNode* read_shape(const rapidjson::Document::ConstObject& scene_object);
        ray_tracing_core::core::ShapeNode* read_collection(const rapidjson::Document::ConstObject& scene_object);
        ray_tracing_core::core::ShapeNode* read_collection(const rapidjson::Document::ConstArray &aray_object);
        ray_tracing_core::environment::Sky* read_sky(const rapidjson::Document::ConstObject& scene_object);
        ray_tracing_core::core::Camera* read_camera_look_at(const rapidjson::Document::ConstObject& scene_object);
        ray_tracing_core::core::Configuration* read_configuration(const rapidjson::Document::ConstObject& scene_object);
    };
}

#endif