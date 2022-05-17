#include "core/bvh_tree.h"
#include "math/random.h"
#include <algorithm>
#include <stdexcept>

using namespace ray_tracing_core;
using namespace ray_tracing_core::core;

namespace
{
    class HitDetector
    {
    public:
        math::DistanceRange distance_range;
        HitRecord& hit_record;

        bool hit(BvhTree::Node* node, const math::Ray& ray_in)
        {
            if (!node->box.hit(ray_in, distance_range))
                return false;

            if (node->leaf)
            {
                if (node->leaf->hit(ray_in, distance_range, hit_record))
                {
                    distance_range = { std::get<0>(distance_range), hit_record.hit_point.distance };
                    return true;
                }
                return false;
            }

            uint32_t i = ray_in.direction[node->sort_axis] >= 0.0 ? 0 : 1;
            auto node1 = node->children[i];
            auto node2 = node->children[(i + 1) % 2];
            if (hit(node1, ray_in))
            {
                hit(node2, ray_in);
                return true;
            }
            return hit(node2, ray_in);
        }
    };
}

BvhTree::BvhTree(const std::vector<const ShapeNode*>& shape_list)
    : root(construct_tree(shape_list))
{
    depth_of_tree = 1;
    size_t power = 1;
    while (power < shape_list.size())
    {
        power *= 2;
        depth_of_tree++;
    }
}

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
    Node* stack[65]; // enugh for 32^2 nodes
    auto stack_depht = 0;
    stack[stack_depht++] = root;
    math::DistanceRange reduced_distance_range = distance_range;
    bool detected_hit = false;
    while (stack_depht > 0)
    {
        auto node = stack[--stack_depht];
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
        stack[stack_depht++] = node->children[(i + 1) % 2];
        stack[stack_depht++] = node->children[i];
    }
    return detected_hit;
#else
    return HitDetector{ distance_range, hit_record }.hit(root, ray_in);
#endif
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