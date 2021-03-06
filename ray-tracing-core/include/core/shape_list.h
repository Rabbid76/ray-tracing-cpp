#ifndef __RTC__CORE__SHAPE_LIST_H__
#define __RTC__CORE__SHAPE_LIST_H__

#include "shape_node.h"
#include <vector>

namespace ray_tracing_core::core
{
    class ShapeList
        : public ShapeNode
    {
    private:

        std::vector<const ShapeNode*> shapes;

    public:

        ShapeList(const std::vector<const ShapeNode*> &shape_list);
        ~ShapeList() override = default;
        math::AxisAlignedBoundingBox bounding_box(void) const override;
        bool hit(const math::Ray& ray_in, const math::DistanceRange& distance_range, HitRecord& hit_record) const override;
        math::Distance probability_density_function_value(
            const math::Point3D & origin, const math::Vector3D & direction) const override;
        math::Vector3D random(const math::Point3D & origin) const override;
    };
}

#endif 