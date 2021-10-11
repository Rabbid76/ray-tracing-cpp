#ifndef __TEST_ASSERT_H__
#define __TEST_ASSERT_H__

// https://gitlab.kitware.com/cmake/community/-/wikis/doc/ctest/Testing-With-CTest
// https://cmake.org/cmake/help/latest/command/add_test.html

#include <assert.h>

#define TEST_ASSERT(condition) assert( condition )
#define TEST_ASSERT_FAIL(condition) assert( !(condition) )
#define TEST_ASSERT_EQUAL(value_a, value_b) assert( (value_a) == (value_b) )
#define TEST_ASSERT_EQUAL_DELTA(value_a, value_b, delta) \
    assert( (((value_a) - (value_b)) <= (delta)) && (((value_b) - (value_a)) <= (delta)))

#endif