#ifndef __RTU__ITERATOR__ITERATOR_EXP2_H__
#define __RTU__ITERATOR__ITERATOR_EXP2_H__

#include <algorithm>
#include <cctype>
#include <vector> 

namespace ray_tracing_utility
{
    namespace iterator
    {
        class IteratorExp2
        {
        private: 

            uint32_t width;
            uint32_t height;

        public:

            inline IteratorExp2(uint32_t width, uint32_t height);

            inline static uint32_t tile_size(uint32_t x, uint32_t y);
            inline static uint32_t no_of_tilies(uint32_t size, uint32_t tile_size);
            inline static std::vector<uint32_t> distribute_indices(uint32_t number);
            inline static std::vector<uint32_t> distribute_indices_reversed(uint32_t number);
        };

        IteratorExp2::IteratorExp2(uint32_t width, uint32_t height)
            : width(width)
            , height(height)
        {}

        uint32_t IteratorExp2::tile_size(uint32_t x, uint32_t y)
        {
            auto max_s = std::max(x, y);
            auto p2 = 0;
            while (max_s > 0)
            {
                p2 += 1;
                max_s >>= 1;
            }
            return 1 << p2 - 1;
        }

        uint32_t IteratorExp2::no_of_tilies(uint32_t size, uint32_t tile_size)
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
    }
}

#endif