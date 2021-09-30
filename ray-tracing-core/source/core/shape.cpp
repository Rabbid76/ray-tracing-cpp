#include "core/shape.h"

namespace ray_tracing_core
{
	namespace core
	{
		bool Shape::hit(const math::Ray& ray_in, const math::DistanceRange &distance_range, HitRecord& hit_record)
		{
			const bool has_texture = material_node->has_texture();
			const bool has_mask = material_node->has_mask();
			if (!geometry_node->hit(ray_in, distance_range, has_texture || has_mask, hit_record))
				return false;
			if (has_mask && !material_node->hit(hit_record))
				return false;
			hit_record.material = material_node;
			return true;
		}
	}
}
