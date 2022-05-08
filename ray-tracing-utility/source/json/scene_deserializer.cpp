#include "json/scene_deserializer.h"
#include "json/rapidjson_deserializer.h"
#include "utility/std_helper.h"
#include <iostream>
#include <stdexcept>
#include <string>

using namespace ray_tracing_core;
using namespace ray_tracing_utility;
using namespace ray_tracing_utility::json;

namespace
{
    class JsonScene
        : public core::Scene
    {
    private:
        core::SceneObjectContainer scene_objects;

    public:
        core::SceneObjectContainer* get_scene_objects()
        {
            return &scene_objects;
        }

        void set_from_scene(core::Scene* scene)
        {
            configuration = scene->get_configuration();
            camera = scene->get_camera();
            sky = scene->get_sky();
            world = scene->get_world();
        }
    };
}


core::Scene* SceneDeserializer::new_scene(const std::string& serialized_json) const
{
    auto json_scene = new JsonScene;
    RapidjsonSceneDeserializer reader(json_scene->get_scene_objects(), aspect);
    rapidjson::Document scene_document;
    try
    {
        scene_document.Parse(serialized_json.c_str(), serialized_json.size());
        if (scene_document.HasParseError()) {
            throw std::runtime_error(utility::formatter() <<
                "json parse error" << scene_document.GetParseError() <<
                "(" << scene_document.GetErrorOffset() << ")");
        }
        auto scene = reader.read_scene_from_json(scene_document);
        json_scene->set_from_scene(scene);
        delete scene;
    }
    catch (std::exception e)
    {
        std::cout << "error reading scene: " << e.what() << std::endl;
        throw;
    }
	return json_scene;
}