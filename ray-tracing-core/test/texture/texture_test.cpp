#include "texture_test.h"
#include "blend_textures_test.h"
#include "constant_texture_test.h"

namespace ray_tracing_core_unit_test::texture
{
    void texture_unit_test(void)
    {
        constant_texture_channels_test();
        blend_textures_channels_test();
    }
}
