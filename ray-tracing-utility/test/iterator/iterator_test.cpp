#include "iterator_test.h"
#include "iterator_exp2_test.h"

namespace ray_tracing_utility_unit_test
{
    namespace iterator
    {
        void iterator_unit_test()
        {
            iterator_exp2_tile_size_test();
            iterator_exp2_no_of_tiles_test();
            iterator_exp2_distribute_indices_test();
            iterator_exp2_distribute_indices_reversed_test();
        }
    }
}