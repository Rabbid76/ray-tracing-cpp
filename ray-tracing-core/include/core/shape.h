#ifndef __RTC__CORE__SHAPE_H__
#define __RTC__CORE__SHAPE_H__

#include "shape_node.h"
#include <geometry/geometry.h>
#include <material/material.h>

namespace ray_tracing_core
{
    namespace core
    {
        class Shape
            : public ShapeNode
        {
        private:

            const geometry::Geometry* geometry_node;
            const material::Material* material_node;

        public:

            inline Shape(const geometry::Geometry* geometry, const material::Material* material);

            inline const geometry::Geometry& geometry(void) const;
            inline const material::Material& material(void) const;

            virtual math::AxisAlignedBoundingBox bounding_box(void) const override;
            virtual bool hit(const math::Ray& ray_in, const math::DistanceRange& istance_range, HitRecord& hit_record) const override;
            virtual math::Distance probability_density_function_value(const math::Point3D& origin, const math::Vector3D& direction) const override;
            virtual math::Vector3D random(const math::Point3D& origin) const override;
        };

        Shape::Shape(const geometry::Geometry* geometry, const material::Material* material)
            : geometry_node{ geometry }
            , material_node{ material }
        {}

        const geometry::Geometry& Shape::geometry(void) const
        {
            return *geometry_node;
        }

        const material::Material& Shape::material(void) const
        {
            return *material_node;
        }
    }
}

#endif
