#include "texture_test.h"
#include "checker_texture_test.h"
#include "constant_texture_test.h"

namespace ray_tracing_core_unit_test::texture
{
    void texture_unit_test(void)
    {
        constant_texture_channels_test();
        checker_texture_channels_test();
    }
}
