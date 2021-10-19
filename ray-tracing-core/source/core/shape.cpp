#include "core/shape.h"

namespace ray_tracing_core
{
    namespace core
    {
        math::AxisAlignedBoundingBox Shape::bounding_box(void) const
        {
            return geometry().bounding_box();
        }

        bool Shape::hit(const math::Ray& ray_in, const math::DistanceRange& distance_range, HitRecord& hit_record) const
        {
            if (!geometry_node->hit(ray_in, distance_range, material_node->has_texture(), hit_record))
                return false;
            if (material_node->has_mask() && !material_node->hit(hit_record))
                return false;
            hit_record.material = material_node;
            return true;
        }

        math::Distance Shape::probability_density_function_value(const math::Point3D& origin, const math::Vector3D& direction) const
        {
            return geometry().probability_density_function_value(origin, direction);
        }

        math::Vector3D Shape::random(const math::Point3D& origin) const
        {
            return geometry().random(origin);
        }
    }
}
