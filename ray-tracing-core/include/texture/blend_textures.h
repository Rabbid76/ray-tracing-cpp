#ifndef __RTC__TEXTURE__BLEND_TEXTURES_H__
#define __RTC__TEXTURE__BLEND_TEXTURES_H__

#include "texture.h"
#include "math/blend_function.h"
#include <cmath>

namespace ray_tracing_core::texture
{
    class BlendTextures
            : public Texture
    {
    private:
        const math::BlendFunction *blend_function;
        const Texture *textures[2];

    public:
        inline BlendTextures(const math::BlendFunction* blend_function, const Texture *odd_texture, const Texture *even_texture);
        ~BlendTextures() override = default;
        inline std::tuple<core::Color, math::AlphaValue> channels(
                const core::TextureCoordinate& texture_coordinate, const math::Point3D& position) const override;
        inline bool depends_on_texture_coordinates() const override;
        inline bool has_alpha_channel() const override;
    };

    BlendTextures::BlendTextures(const math::BlendFunction* blend_function, const Texture *odd_texture, const Texture *even_texture)
        : blend_function(blend_function)
        , textures{odd_texture, even_texture}
    {}

    std::tuple<core::Color, math::AlphaValue> BlendTextures::channels(
            const core::TextureCoordinate& texture_coordinate, const math::Point3D& position) const
    {
        auto blend_weight = blend_function->evaluate(texture_coordinate, position);
        if (blend_weight <= 0.0f)
            return textures[0]->channels(texture_coordinate, position);
        else if (blend_weight >= 1.0f)
            return textures[1]->channels(texture_coordinate, position);
        
        auto [color0, alpha0] = textures[0]->channels(texture_coordinate, position);
        auto [color1, alpha1] = textures[1]->channels(texture_coordinate, position);
        return
        {
            (color0 * (1.0f - blend_weight)) + (color1 * blend_weight),
            (alpha0 * (1.0f - blend_weight)) + (alpha1 * blend_weight),
        };
    }

    bool BlendTextures::depends_on_texture_coordinates() const
    {
        return blend_function->depends_on_texture_coordinates();
    }

    bool BlendTextures::has_alpha_channel() const
    {
        return textures[0]->has_alpha_channel() || textures[1]->has_alpha_channel();
    }
}


#endif