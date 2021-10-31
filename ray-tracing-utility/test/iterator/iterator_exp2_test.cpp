#include "iterator_exp2_test.h"
#include <iterator/iterator_exp2.h>
#include <rtu_test_assert.h>
#include <vector>
#include <tuple>

namespace ray_tracing_utility_unit_test
{
    namespace iterator
    {
        using namespace ray_tracing_utility;
        using namespace ray_tracing_utility::iterator;

        void iterator_exp2_tile_size_test()
        {
            std::vector<std::tuple<uint32_t, uint32_t, uint32_t>> test_data
            {
                { 64, 127, 1 },
                { 128, 127, 128 },
                { 32, 63, 63 },
            };

            for (auto [expected_size, width, height] : test_data)
            {
                auto actual_size = IteratorExp2::tile_size(width, height);
                TEST_ASSERT_EQUAL(expected_size, actual_size);
            }
        }

        void iterator_exp2_no_of_tiles_test()
        {
            std::vector<std::tuple<uint32_t, uint32_t, uint32_t>> test_data
            {
                { 1, 1, 1},
                { 2, 2, 1},
                { 2, 3, 2},
                { 2, 32, 31 }
            };

            for (auto [expected_no_of_tiles, size, tile_size] : test_data)
            {
                auto actual_no_of_tiles = IteratorExp2::no_of_tilies(size, tile_size);
                TEST_ASSERT_EQUAL(expected_no_of_tiles, actual_no_of_tiles);
            }
        }
    }
}