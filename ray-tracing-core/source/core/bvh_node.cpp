#include "core/bvh_node.h"
#include "math/random.h"
#include <algorithm>
#include <stdexcept>

using namespace ray_tracing_core;
using namespace ray_tracing_core::core;

BvhNode::BvhNode(const std::vector<const ShapeNode*>& shape_list)
{
    sort_axis = math::RandomGenerator().random_axis();
    auto shapes = shape_list;
    std::sort(shapes.begin(), shapes.end(),
        [&](const ShapeNode* a, const ShapeNode* b) -> bool
        {
            auto box_a = a->bounding_box();
            auto box_b = b->bounding_box();
            return box_a.maximum_point[sort_axis] < box_b.maximum_point[sort_axis];
        });

    auto center = shapes.size() / 2;
    std::vector<const ShapeNode*> left_shapes(shapes.begin(), shapes.begin()+ center);
    std::vector<const ShapeNode*> right_shapes(shapes.begin() + center, shapes.end());
    left_managed = left_shapes.size() > 1;
    left = left_managed ? new BvhNode(left_shapes) : left_shapes.front();
    right_managed = right_shapes.size() > 1;
    right = right_managed ? new BvhNode(right_shapes) : right_shapes.front();
    left_box = left->bounding_box();
    right_box = right->bounding_box();
    box = left_box;
    box |= right_box;
}

BvhNode::~BvhNode()
{
    if (left_managed)
        delete left;
    if (right_managed)
        delete right;
}

math::AxisAlignedBoundingBox BvhNode::bounding_box(void) const
{
    return box;
}

bool BvhNode::hit(const math::Ray& ray_in, const math::DistanceRange& distance_range, HitRecord& hit_record) const
{
    bool hit = false;
    if (ray_in.direction[sort_axis] >= 0.0)
    {
        if (left_box.hit(ray_in, distance_range) && left->hit(ray_in, distance_range, hit_record))
        {
            hit = true;
            math::DistanceRange reduced_range{ std::get<0>(distance_range), hit_record.hit_point.distance };
            if (right_box.hit(ray_in, reduced_range))
                right->hit(ray_in, reduced_range, hit_record);
        }
        else if (right_box.hit(ray_in, distance_range))
        {
            hit = right->hit(ray_in, distance_range, hit_record);
        }
    }
    else
    {
        if (right_box.hit(ray_in, distance_range) && right->hit(ray_in, distance_range, hit_record))
        {
            hit = true;
            math::DistanceRange reduced_range{ std::get<0>(distance_range), hit_record.hit_point.distance };
            if (left_box.hit(ray_in, reduced_range))
                left->hit(ray_in, reduced_range, hit_record);
        }
        else if (left_box.hit(ray_in, distance_range))
        {
            hit = left->hit(ray_in, distance_range, hit_record);
        }
    }
    return hit;
}

math::Distance BvhNode::probability_density_function_value(
    const math::Point3D& origin, const math::Vector3D& direction) const
{
    // self.list.iter().map(|node| node.pdf_value(o, v)).sum()
    throw std::runtime_error("not implemented");
    return 0;
}

math::Vector3D BvhNode::random(const math::Point3D& origin) const
{
    // self.list[random::generate_from_range(0..self.list.len())].random(o)
    throw std::runtime_error("not implemented");
    return math::Vector3D(0);
}