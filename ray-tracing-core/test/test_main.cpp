#include <test_assert.h>

void unit_test(void);

int main(int, char**)
{
    unit_test();
    return 0;
}

void unit_test(void)
{
    int xi = 1;
    TEST_ASSERT(1 == xi);
    TEST_ASSERT_EQUAL(1, xi);

    double xd = 0.9999;
    TEST_ASSERT_EQUAL_DELTA(1, xd, 0.01);
    //TEST_ASSERT_EQUAL_DELTA(1.0, xd, 0.000001);
}