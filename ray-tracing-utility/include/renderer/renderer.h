#ifndef __RTU__RENDER__RENDERER_H__
#define __RTU__RENDER__RENDERER_H__

#include "math/types.h"
#include "core/scene.h"
#include <cstdint>
#include <tuple>
#include <vector>

namespace ray_tracing_utility::renderer
{
    class Renderer
    {
    public:
        // TODO: "double" buffer
        using BufferData = std::pair<glm::dvec3, uint32_t>;
        std::tuple<uint32_t, uint32_t> buffer_size{ 0, 0 };
        using Buffer = std::vector<BufferData>;

    protected:
        Buffer buffer;

    public:
        virtual ~Renderer() = default;
        const std::tuple<uint32_t, uint32_t>& get_buffer_size() const { return buffer_size; };
        const Buffer &get_render_buffer() const { return buffer; }
        std::vector<uint8_t> get_rgba8() const;
        virtual void render(ray_tracing_core::core::Scene& scene, std::tuple<uint32_t, uint32_t> size, uint32_t samples) = 0;
        virtual bool is_finished() const = 0;
        virtual double percentage_done() const = 0;
    };
}

#endif