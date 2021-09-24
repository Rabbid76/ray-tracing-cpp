#ifndef __RTC__CORE__SHAPE_H__
#define __RTC__CORE__SHAPE_H__

#include "geometry/geometry.h"
#include "material/material.h"

namespace ray_tracing_core
{
	namespace Core
	{
		class Shape
		{
		private:

			geometry::Geometry *geometry;
			material::Material *material;
		};
	}
}

#endif
