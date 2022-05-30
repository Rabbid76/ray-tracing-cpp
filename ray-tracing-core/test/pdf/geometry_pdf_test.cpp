#include "geometry_pdf_test.h"
#include "geometry/sphere.h"
#include "pdf/geometry_pdf.h"
#include <rtc_test_assert.h>
#include <vector>
#include <tuple>

namespace ray_tracing_core_unit_test::pdf
{
    using namespace ray_tracing_core::pdf;
    using namespace ray_tracing_core;

    void geometry_pdf_test(void)
    {
        const int test_iterations = 10;

        std::vector<std::tuple<math::Distance, math::Vector3D, math::Vector3D, math::Vector3D, geometry::Sphere>> test_data
        {
            { 1.1879486677893731, math::Vector3D(1, 0, 0),
              math::Vector3D(-1, 0, 0), math::Vector3D(1, 0, 0),
              geometry::Sphere(math::Point3D(2, 0, 0), 1)
            }
        };

        for (auto [expected_value, expected_vector, normal_vector, direction_vector, geoemtry] : test_data)
        {
            for (int iteration = 0; iteration < test_iterations; ++iteration)
            {
                GeometryPDF geometry_pdf(math::Point3D(0), &geoemtry);
                auto actual_value = geometry_pdf.value(direction_vector);
                auto actual_vector = geometry_pdf.generate();
                TEST_ASSERT_EQUAL_DELTA(expected_value, actual_value, 0.001);
                assert_equal_vector(expected_vector, actual_vector, 2);
            }
        }
    }
}