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
                auto actual_no_of_tiles = IteratorExp2::no_of_tiles(size, tile_size);
                TEST_ASSERT_EQUAL(expected_no_of_tiles, actual_no_of_tiles);
            }
        }

        void iterator_exp2_distribute_indices_test()
        {
            std::vector<std::tuple<std::vector<uint32_t>, uint32_t>> test_data
            {
                { { 0 }, 1 },
                { { 0, 1 }, 2 },
                { { 1, 2, 0 }, 3 },
                { { 1, 2, 0, 3 }, 4 }
            };

            for (auto [expected_indices, number] : test_data)
            {
                auto actual_indices = IteratorExp2::distribute_indices(number);
                TEST_ASSERT_EQUAL(expected_indices.size(), actual_indices.size());
                for (uint32_t i = 0; i < expected_indices.size(); ++i)
                    TEST_ASSERT_EQUAL(expected_indices[i], actual_indices[i]);
            }
        }

        void iterator_exp2_distribute_indices_reversed_test()
        {
            std::vector<std::tuple<std::vector<uint32_t>, uint32_t>> test_data
            {
                { { 0 }, 1 },
                { { 1, 0 }, 2 },
                { { 1, 0, 2 }, 3 },
                { { 2, 1, 3, 0 }, 4 }
            };

            for (auto [expected_indices, number] : test_data)
            {
                auto actual_indices = IteratorExp2::distribute_indices_reversed(number);
                TEST_ASSERT_EQUAL(expected_indices.size(), actual_indices.size());
                for (uint32_t i = 0; i < expected_indices.size(); ++i)
                    TEST_ASSERT_EQUAL(expected_indices[i], actual_indices[i]);
            }
        }

        void iterator_exp2_next_test()
        {
            std::vector<std::tuple<std::vector<std::tuple<uint32_t, uint32_t, uint32_t>>, uint32_t, uint32_t>> test_data
            {
                { {{0, 0, 2}, {0, 1, 1}, {1, 1, 1}, {1, 0, 1} }, 2, 2 },
            };

            for (auto [expected_indices, width, height] : test_data)
            {
                auto iterator = IteratorExp2(width, height);
                for (auto [expected_ix, expected_iy, expected_size] : expected_indices)
                {
                    auto [actual_ix, actual_iy, actual_size] = iterator.next();
                    TEST_ASSERT_EQUAL(expected_ix, actual_ix);
                    TEST_ASSERT_EQUAL(expected_iy, actual_iy);
                    TEST_ASSERT_EQUAL(expected_size, actual_size);
                }
                auto [actual_ix, actual_iy, actual_size] = iterator.next();
                TEST_ASSERT_EQUAL(-1, actual_ix);
                TEST_ASSERT_EQUAL(-1, actual_iy);
                TEST_ASSERT_EQUAL(0, actual_size);
            }
        }
    }
}