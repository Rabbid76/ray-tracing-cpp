#ifndef __RTC__CORE__BVH_NODE_H__
#define __RTC__CORE__BVH_NODE_H__

#include "shape_node.h"
#include <vector>

namespace ray_tracing_core::core
{
    class BvhNode
        : public ShapeNode
    {
    private:

        const ShapeNode* left;
        const ShapeNode* right;
        bool left_managed = false;
        bool right_managed = false;
        uint32_t sort_axis;
        math::AxisAlignedBoundingBox left_box;
        math::AxisAlignedBoundingBox right_box;
        math::AxisAlignedBoundingBox box;

    public:

        BvhNode(const std::vector<const ShapeNode*>& shape_list);
        ~BvhNode() override;
        math::AxisAlignedBoundingBox bounding_box(void) const override;
        bool hit(const math::Ray& ray_in, const math::DistanceRange& distance_range, HitRecord& hit_record) const override;
        math::Distance probability_density_function_value(
            const math::Point3D& origin, const math::Vector3D& direction) const override;
        math::Vector3D random(const math::Point3D& origin) const override;
    };
}

#endif