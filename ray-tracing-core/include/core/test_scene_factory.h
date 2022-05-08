#ifndef __RTC__CORE__TEST_SCENE_FACTORY__H__
#define __RTC__CORE__TEST_SCENE_FACTORY__H__

#include "scene_factory.h"

namespace ray_tracing_core
{
	namespace core
	{
		class TestSceneFactory
			: public SceneFactory
		{
		public:
			Scene* new_scene(const std::string& argument = "") const override;
		};
	}
}

#endif