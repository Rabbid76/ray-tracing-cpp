#ifndef __RTC__CORE__CONFIGURATION_H__
#define __RTC__CORE__CONFIGURATION_H__

#include <cstdint>

namespace ray_tracing_core
{
    namespace core
    {
        class Configuration
        {
        public:

            uint32_t maximum_depth = 50;
        };
    }
}

#endif