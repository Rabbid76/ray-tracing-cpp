#include <core/shape_list.h>

namespace ray_tracing_core
{
    namespace core
    {
        ShapeList::ShapeList(const std::vector<const ShapeNode*>& shape_list)
            : shapes(shape_list)
        {}

        math::AxisAlignedBoundingBox ShapeList::bounding_box(void) const
        {
            auto box = shapes[0]->bounding_box();
            for (auto box_it = shapes.begin() + 1; box_it < shapes.end(); ++box_it)
                box |= (*box_it)->bounding_box();
            return box;
        }

        bool ShapeList::hit(const math::Ray& ray_in, const math::DistanceRange& distance_range, HitRecord& hit_record) const
        {
            for (auto shape : shapes)
            {
                if (shape->hit(ray_in, distance_range, hit_record))
                    return true;
            }
            return false;
        }

        math::Distance ShapeList::probability_density_function_value(
            const math::Point3D& origin, const math::Vector3D& direction) const
        {
            return 0;
        }

        math::Vector3D ShapeList::random(const math::Point3D& origin) const
        {
            return math::Vector3D(0);
        }
    }
}