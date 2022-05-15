#include "core/bvh_tree.h"
#include "math/random.h"
#include <algorithm>
#include <stdexcept>

using namespace ray_tracing_core;
using namespace ray_tracing_core::core;

BvhTree::BvhTree(const std::vector<const ShapeNode*>& shape_list)
    : root(construct_tree(shape_list))
{}

BvhTree::~BvhTree()
{
    delete root;
}

BvhTree::Node* BvhTree::construct_tree(std::vector<const ShapeNode*> shape_set)
{
    auto node = new Node;
    if (shape_set.size() > 1)
    {
        node->sort_axis = math::RandomGenerator().random_axis();
        auto shapes = shape_set;
        std::sort(shapes.begin(), shapes.end(),
            [&](const ShapeNode* a, const ShapeNode* b) -> bool
            {
                auto box_a = a->bounding_box();
                auto box_b = b->bounding_box();
                return box_a.maximum_point[node->sort_axis] < box_b.maximum_point[node->sort_axis];
            });

        auto center = shapes.size() / 2;
        node->children[0] = construct_tree(std::vector<const ShapeNode*>(shapes.begin(), shapes.begin() + center));
        node->children[1] = construct_tree(std::vector<const ShapeNode*>(shapes.begin() + center, shapes.end()));
        node->box = node->children[0]->box;
        node->box |= node->children[1]->box;
    }
    else
    {
        node->leaf = shape_set.front();
        node->box = node->leaf->bounding_box();
    }
    return node;
}

math::AxisAlignedBoundingBox BvhTree::bounding_box(void) const
{
    return root->box;
}

//#define NON_RECURSIVE

bool BvhTree::hit(const math::Ray& ray_in, const math::DistanceRange& distance_range, HitRecord& hit_record) const
{
#ifdef NON_RECURSIVE
    std::vector<Node*> stack;
    stack.reserve(10);
    stack.push_back(root);
    math::DistanceRange reduced_distance_range = distance_range;
    bool detected_hit = false;
    while (!stack.empty())
    {
        auto node = stack.back();
        stack.pop_back();

        if (!node->box.hit(ray_in, reduced_distance_range))
            continue;

        if (node->leaf)
        {
            if (node->leaf->hit(ray_in, reduced_distance_range, hit_record))
            {
                detected_hit = true;
                reduced_distance_range = math::DistanceRange{ std::get<0>(reduced_distance_range), hit_record.hit_point.distance };
            }
            continue;
        }

        uint32_t i = ray_in.direction[node->sort_axis] >= 0.0 ? 0 : 1;
        stack.push_back(node->children[(i + 1) % 2]);
        stack.push_back(node->children[i]);
    }
    return detected_hit;
#else
    return hit(root, ray_in, distance_range, hit_record);
#endif
}

bool BvhTree::hit(Node* node, const math::Ray& ray_in, const math::DistanceRange& distance_range, HitRecord& hit_record) const
{
    if (!node->box.hit(ray_in, distance_range))
        return false;
    if (node->leaf)
        return node->leaf->hit(ray_in, distance_range, hit_record);

    bool detected_hit = false;
    uint32_t i = ray_in.direction[node->sort_axis] >= 0.0 ? 0 : 1;
    auto node1 = node->children[i];
    auto node2 = node->children[(i + 1) % 2];
    if (hit(node1, ray_in, distance_range, hit_record))
    {
        detected_hit = true;
        math::DistanceRange reduced_range{ std::get<0>(distance_range), hit_record.hit_point.distance };
        hit(node2, ray_in, reduced_range, hit_record);
    }
    else
    {
        detected_hit = hit(node2, ray_in, distance_range, hit_record);
    }
    return detected_hit;
}

math::Distance BvhTree::probability_density_function_value(
    const math::Point3D& origin, const math::Vector3D& direction) const
{
    throw std::runtime_error("not implemented");
    return 0;
}

math::Vector3D BvhTree::random(const math::Point3D& origin) const
{
    throw std::runtime_error("not implemented");
    return math::Vector3D(0);
}