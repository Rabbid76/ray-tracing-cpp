#ifndef __RTC__PDF__GEOMETRY_PDF_H__
#define __RTC__PDF__GEOMETRY_PDF_H__

#include "geometry/geometry.h"
#include "math/types.h"
#include "pdf/probability_density_function.h"

namespace ray_tracing_core::pdf
{
    using namespace ray_tracing_core;

    class GeometryPDF
            : public ProbabilityDensityFunction
    {
    private:
        math::Point3D origin;
        const geometry::Geometry *geometry = nullptr;

    public:
        GeometryPDF() = default;
        inline GeometryPDF(const math::Point3D &origin, const geometry::Geometry *geometry);
        ~GeometryPDF() override = default;
        inline void set_geometry(const geometry::Geometry *new_geometry);
        inline void set_origin(const math::Point3D &new_origin);
        inline math::Distance value(const math::Vector3D &direction) const override;
        inline math::Vector3D generate() const override;

    };

    GeometryPDF::GeometryPDF(const math::Point3D &origin, const geometry::Geometry *geometry)
            : origin(origin)
            , geometry(geometry)
    {}

    void GeometryPDF::set_geometry(const geometry::Geometry *new_geometry)
    {
        geometry = new_geometry;
    }

    void GeometryPDF::set_origin(const math::Point3D &new_origin)
    {
        origin = new_origin;
    }

    math::Distance GeometryPDF::value(const math::Vector3D &direction) const
    {
        return geometry->probability_density_function_value(origin, direction);
    }

    math::Vector3D GeometryPDF::generate() const
    {
        return geometry->random(origin);
    }
}

#endif