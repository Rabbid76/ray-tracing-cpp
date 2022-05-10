#include <core/shape_list.h>
#include <stdexcept>

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
            bool hit = false;
            for (auto shape : shapes)
            {
                HitRecord current_hit_record;
                bool current_hit = shape->hit(ray_in, distance_range, current_hit_record);
                if (!current_hit)
                    continue;
                if (!hit || current_hit_record.hit_point.distance < hit_record.hit_point.distance)
                {
                    hit = true;
                    hit_record = current_hit_record;
                }
            }
            return hit;
        }

        math::Distance ShapeList::probability_density_function_value(
            const math::Point3D& origin, const math::Vector3D& direction) const
        {
            // self.list.iter().map(|node| node.pdf_value(o, v)).sum()
            throw std::runtime_error("not implemented");
            return 0;
        }

        math::Vector3D ShapeList::random(const math::Point3D& origin) const
        {
            // self.list[random::generate_from_range(0..self.list.len())].random(o)
            throw std::runtime_error("not implemented");
            return math::Vector3D(0);
        }
    }
}