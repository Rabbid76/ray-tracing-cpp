#ifndef __RTU__ITERATOR__ITERATOR_H__
#define __RTU__ITERATOR__ITERATOR_H__

#include <tuple> 
#include <cstdint>
#include <cstdint>

namespace ray_tracing_utility
{
    namespace iterator
    {
        class Iterator
        {
        public:
            virtual ~Iterator() = default;
            virtual std::tuple<uint32_t, uint32_t> get_size() const = 0;
            virtual std::tuple<uint32_t, uint32_t, uint32_t> next() = 0;
        };
    }
}

#endif