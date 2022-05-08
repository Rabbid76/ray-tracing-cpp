#ifndef __RTC__CORE__SCENE_FACTORY__H__
#define __RTC__CORE__SCENE_FACTORY__H__

#include "core/configuration.h"
#include <string>

namespace ray_tracing_core
{
	namespace core
	{
		class Scene;

		class SceneFactory
		{
		protected:
			Configuration configuration;
			double aspect = 1.0;
		
		public:
			inline SceneFactory &set_configuration(const Configuration &new_configuration);
			inline SceneFactory &set_aspect(double new_aspect);

			virtual Scene *new_scene(const std::string &argument = "") const = 0;
		};

		SceneFactory &SceneFactory::set_configuration(const Configuration &new_configuration)
		{
			configuration = new_configuration;
			return *this;
		}

		SceneFactory& SceneFactory::set_aspect(double new_aspect)
		{
			aspect = new_aspect;
			return *this;
		}
	}
}

#endif