#include "configuration_test.h"
#include <core/configuration.h>
#include <rtc_core_test_assert.h>

namespace ray_tracing_core_unit_test
{
    namespace core
    {
        using namespace ray_tracing_core;
        using namespace ray_tracing_core::core;

        void configuration_test()
        {
            Configuration configuration
            {
                .maximum_depth = 100
            };
            TEST_ASSERT_EQUAL(100, configuration.maximum_depth);
        }
    }
}