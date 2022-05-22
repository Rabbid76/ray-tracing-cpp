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
            auto material = material_node->material();
            if (!geometry_node->hit(ray_in, distance_range, material->has_texture(), hit_record))
                return false;
            if (material->has_mask() && !material->hit(hit_record))
            {
                auto start_from_inner = math::DistanceRange{ hit_record.hit_point.distance + 0.0001f, std::get<1>(distance_range) };
                if (!geometry_node->hit(ray_in, start_from_inner, material->has_texture(), hit_record))
                    return false;
                if (!material->hit(hit_record))
                    return false;
            }
            hit_record.material = material;
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
