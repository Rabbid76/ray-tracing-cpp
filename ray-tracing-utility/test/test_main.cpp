#include "rtu_test_assert.h"
#include "iterator/iterator_test.h"
#include <string>

void unit_test(std::string selected_test);

int main(int noargs, char** args)
{
    const char* selected_test = noargs > 1 ? args[1] : "";
    unit_test(selected_test);
    return 0;
}

void unit_test(std::string selected_test)
{
    using namespace ray_tracing_utility_unit_test;

    if (selected_test.empty() || selected_test.starts_with("iterator"))
        iterator::iterator_unit_test();
}