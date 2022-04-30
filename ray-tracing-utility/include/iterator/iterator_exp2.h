#ifndef __RTU__ITERATOR__ITERATOR_EXP2_H__
#define __RTU__ITERATOR__ITERATOR_EXP2_H__

#include "iterator.h"
#include <algorithm>
#include <cctype>
#include <vector> 

namespace ray_tracing_utility
{
    namespace iterator
    {
        class IteratorExp2 : public Iterator
        {
        private: 

            uint32_t width;
            uint32_t height;
            uint32_t size;
            int tx = -1;
            int ty = -1;
            std::vector<uint32_t> lix;
            std::vector<uint32_t> liy;

        public:

            inline static uint32_t tile_size(uint32_t x, uint32_t y);
            inline static uint32_t no_of_tiles(uint32_t size, uint32_t tile_size);
            inline static std::vector<uint32_t> distribute_indices(uint32_t number);
            inline static std::vector<uint32_t> distribute_indices_reversed(uint32_t number);

            inline IteratorExp2(uint32_t width, uint32_t height);
            virtual ~IteratorExp2() = default;
            std::tuple<uint32_t, uint32_t> get_size() const override;
            std::tuple<uint32_t, uint32_t, uint32_t> next() override;
        };

        IteratorExp2::IteratorExp2(uint32_t width, uint32_t height)
            : width(width)
            , height(height)
            , size(tile_size(width, height))
        {}

        std::tuple<uint32_t, uint32_t> IteratorExp2::get_size() const {
            return { width, height };
        }

        uint32_t IteratorExp2::tile_size(uint32_t x, uint32_t y)
        {
            auto max_s = std::max(x, y);
            auto p2 = 0;
            while (max_s > 0)
            {
                p2 += 1;
                max_s >>= 1;
            }
            return 1 << (p2 - 1);
        }

        uint32_t IteratorExp2::no_of_tiles(uint32_t size, uint32_t tile_size)
        {
            return (size - 1) / tile_size + 1;
        }

        std::vector<uint32_t> IteratorExp2::distribute_indices(uint32_t number)
        {
            std::vector<uint32_t> indices(number);
            auto m = (number - 1) / 2;
            for (uint32_t i = 0; i < number; ++i)
                indices[i] = i % 2 == 0 ? m - i / 2 : m + 1 + i / 2;
            return indices;
        }
        
        std::vector<uint32_t> IteratorExp2::distribute_indices_reversed(uint32_t number)
        {
            std::vector<uint32_t> indices(number);
            auto m = number / 2;
            for (uint32_t i = 0; i < number; ++i)
                indices[i] = i % 2 == 0 ? m + i / 2 : m - 1 - i / 2;
            return indices;
        }

        std::tuple<uint32_t, uint32_t, uint32_t> IteratorExp2::next()
        {
            if (tx < 0 && ty < 0)
            {
                tx = 0, ty = 0;
                return { tx, ty, size };
            }

            while (size > 0)
            {
                if (tx <= 0 && ty <= 0)
                {
                    lix = distribute_indices(no_of_tiles(width, size));
                    liy = distribute_indices_reversed(no_of_tiles(height, size));
                    tx = 0, ty = 0;
                }
                auto x = lix[tx], y = liy[ty], s = size;
                tx += 1;
                if (tx >= lix.size())
                {
                    tx = 0;
                    ty += 1;
                }
                if (ty >= liy.size())
                {
                    ty = 0;
                    size >>= 1;
                }
                if (size > 128 || x % 2 != 0 || y % 2 != 0)
                    return { x*s, y*s, s };
            }
            
            return { -1, -1, 0 };
        }
    }
}

#endif