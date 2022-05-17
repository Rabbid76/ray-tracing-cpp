#ifndef __RTC__TEXTURE__CONSTANT_TEXTURE_H__
#define __RTC__TEXTURE__CONSTANT_TEXTURE_H__

#include "texture.h"

namespace ray_tracing_core::texture
{
    class ConstantTexture
        : public Texture
    {
    private:
        core::Color color;
        math::AlphaValue alpha_value;

    public:
        inline ConstantTexture(const core::Color& texture_color, const math::AlphaValue texture_alpha_value);
        ~ConstantTexture() override = default;
        inline std::tuple<core::Color, math::AlphaValue> channels(
            const core::TextureCoordinate& texture_coordinate, const math::Point3D& position) const override;
        inline bool depends_on_texture_coordinates() const override;
        inline bool has_alpha_channel() const override;
    };

    ConstantTexture::ConstantTexture(const core::Color& texture_color, math::AlphaValue texture_alpha_value)
        : color(texture_color)
        , alpha_value(texture_alpha_value)
    {}

    std::tuple<core::Color, math::AlphaValue> ConstantTexture::channels(
        const core::TextureCoordinate& texture_coordinate, const math::Point3D& position) const
    {
        return {color, alpha_value};
    }

    bool ConstantTexture::depends_on_texture_coordinates() const
    {
        return false;
    }

    bool ConstantTexture::has_alpha_channel() const
    {
        return alpha_value < 1;
    }
}

#endif
