#ifndef __RTU__JSON__SCENE__DESERIALIZER_H__
#define __RTU__JSON__SCENE__DESERIALIZER_H__

#include "core/scene_factory.h"
#include <string>

namespace ray_tracing_utility::json
{
	class SceneDeserializer
		: public ray_tracing_core::core::SceneFactory
	{
	public:
		ray_tracing_core::core::Scene* new_scene(const std::string& serialized_json) const override;
	};
}

#endif