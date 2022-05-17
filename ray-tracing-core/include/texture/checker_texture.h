#ifndef __RTC__TEXTURE__CHECKER_TEXTURE_H__
#define __RTC__TEXTURE__CHECKER_TEXTURE_H__

#include "texture.h"
#include <cmath>

namespace ray_tracing_core::texture
{
    class CheckerTexture
            : public Texture
    {
    private:
        math::Vector3D scale;
        const Texture *textures[2];

    public:
        inline CheckerTexture(const math::Vector3D scale, const Texture *odd_texture, const Texture *even_texture);
        ~CheckerTexture() override = default;
        inline std::tuple<core::Color, math::AlphaValue> channels(
                const core::TextureCoordinate& texture_coordinate, const math::Point3D& position) const override;
        inline bool depends_on_texture_coordinates() const override;
        inline bool has_alpha_channel() const override;
    };

    CheckerTexture::CheckerTexture(const math::Vector3D scale, const Texture *odd_texture, const Texture *even_texture)
        : scale(scale)
        , textures{odd_texture, even_texture}
    {}

    std::tuple<core::Color, math::AlphaValue> CheckerTexture::channels(
            const core::TextureCoordinate& texture_coordinate, const math::Point3D& position) const
    {
        // cos(0) == 1
        auto cosines = std::cos(scale.x * position.x) * std::cos(scale.y * position.y) * std::cos(scale.z * position.z);
        return cosines >= 0.0
            ? textures[0]->channels(texture_coordinate, position)
            : textures[1]->channels(texture_coordinate, position);
    }

    bool CheckerTexture::depends_on_texture_coordinates() const
    {
        return false;
    }

    bool CheckerTexture::has_alpha_channel() const
    {
        return textures[0]->has_alpha_channel() || textures[1]->has_alpha_channel();
    }
}


#endif