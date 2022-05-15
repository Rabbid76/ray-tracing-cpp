#ifndef __RTC__CORE__BVH_TREE_H__
#define __RTC__CORE__BVH_TREE_H__

#include "shape_node.h"
#include <vector>

namespace ray_tracing_core::core
{
    class BvhTree
        : public ShapeNode
    {
    public:

        class Node
        {
        public:
            Node* children[2]{ nullptr, nullptr };
            const ShapeNode* leaf = nullptr;
            uint32_t sort_axis = 0;
            math::AxisAlignedBoundingBox box;
            
            ~Node()
            {
                delete children[0];
                delete children[1];
            }
        };

    private:
        Node* root;
        uint32_t depth_of_tree;

    public:
        BvhTree(const std::vector<const ShapeNode*>& shape_list);
        ~BvhTree() override;
        math::AxisAlignedBoundingBox bounding_box(void) const override;
        bool hit(const math::Ray& ray_in, const math::DistanceRange& distance_range, HitRecord& hit_record) const override;
        math::Distance probability_density_function_value(
            const math::Point3D& origin, const math::Vector3D& direction) const override;
        math::Vector3D random(const math::Point3D& origin) const override;
    
    private:
        Node* construct_tree(std::vector<const ShapeNode*> shape_set);
    };
}

#endif