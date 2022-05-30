#include "probability_density_function_test.h"
#include "cosine_pdf_test.h"
#include "geometry_pdf_test.h"

namespace ray_tracing_core_unit_test::pdf
{
    void probability_density_function_unit_test(void)
    {
        cosine_pdf_test();
        geometry_pdf_test();
    }
}
