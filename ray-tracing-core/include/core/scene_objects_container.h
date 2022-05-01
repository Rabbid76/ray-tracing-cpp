#ifndef __RTC__CORE__SCENE_OBJECTS_CONTAINER__H__
#define __RTC__CORE__SCENE_OBJECTS_CONTAINER__H__

#include "core/camera.h"
#include "core/shape_node.h"
#include "environment/sky.h"
#include "geometry/geometry.h"
#include "material/material.h"
#include "texture/texture.h"
#include <memory>
#include <vector>

namespace ray_tracing_core
{
	namespace core
	{
		class SceneObjectContainer
		{
		public:
			std::vector<std::shared_ptr<texture::Texture>> textures;
			std::vector<std::shared_ptr<material::Material>> materials;
			std::vector<std::shared_ptr<geometry::Geometry>> geometries;
			std::vector<std::shared_ptr<core::ShapeNode>> shape_nodes;
			std::vector<std::shared_ptr<environment::Sky>> skys;
			std::vector<std::shared_ptr<core::Camera>> cameras;
		};
	}
}

#endif