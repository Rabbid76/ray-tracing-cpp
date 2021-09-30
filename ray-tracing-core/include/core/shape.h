#ifndef __RTC__CORE__SHAPE_H__
#define __RTC__CORE__SHAPE_H__

#include "geometry/geometry.h"
#include "material/material.h"

namespace ray_tracing_core
{
	namespace core
	{
		class Shape
		{
		private:

			const geometry::Geometry *geometry_node;
			const material::Material *material_node;

		public:

			inline Shape(const geometry::Geometry* geometry, const material::Material* material);

			inline const geometry::Geometry& geometry(void) const;
			inline const material::Material& material(void) const;

			bool hit(const math::Ray& ray_in, const math::DistanceRange &distance_range, HitRecord& hit_record);
		};

		Shape::Shape(const geometry::Geometry* geometry, const material::Material* material)
			: geometry_node{ geometry }
			, material_node{ material }
		{}

		const geometry::Geometry& Shape::geometry(void) const
		{
			return *geometry_node;
		}

		const material::Material& Shape::material(void) const
		{
			return *material_node;
		}
	}
}

#endif
