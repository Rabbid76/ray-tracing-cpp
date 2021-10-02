#include "core/shape.h"

namespace ray_tracing_core
{
	namespace core
	{
		bool Shape::hit(const math::Ray& ray_in, const math::DistanceRange &distance_range, HitRecord& hit_record)
		{
			if (!geometry_node->hit(ray_in, distance_range, material_node->has_texture(), hit_record))
				return false;
			if (material_node->has_mask() && !material_node->hit(hit_record))
				return false;
			hit_record.material = material_node;
			return true;
		}
	}
}
