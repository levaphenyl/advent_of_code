#include "unity.h"
#include "../src/day1.c"


/* Constructor/destructor for tests.
   Leave empty if not explicitly needed. */
void setUp(void) {}
void tearDown(void) {}


void test_mod_fuel(void)
{
        TEST_ASSERT_EQUAL_UINT(0, mod_fuel(1));
        TEST_ASSERT_EQUAL_UINT(0, mod_fuel(6));
        TEST_ASSERT_EQUAL_UINT(2, mod_fuel(12));
        TEST_ASSERT_EQUAL_UINT(2, mod_fuel(14));
        TEST_ASSERT_EQUAL_UINT(654, mod_fuel(1969));
        TEST_ASSERT_EQUAL_UINT(33583, mod_fuel(100756));
}

void test_mod_fuel_rec(void)
{
        TEST_ASSERT_EQUAL_UINT(0, mod_fuel_rec(1));
        TEST_ASSERT_EQUAL_UINT(2, mod_fuel_rec(14));
        TEST_ASSERT_EQUAL_UINT(966, mod_fuel_rec(1969));
        TEST_ASSERT_EQUAL_UINT(50346, mod_fuel_rec(100756));
}

void test_file_parse_sum(void)
{
        unsigned simple = file_parse_sum(
                "2019/test/data/input-day1.txt",
                mod_fuel
        );
        TEST_ASSERT_EQUAL_UINT(6, simple);
}


int main(void)
{
        UNITY_BEGIN();
        RUN_TEST(test_mod_fuel);
        RUN_TEST(test_mod_fuel_rec);
        RUN_TEST(test_file_parse_sum);
        return UNITY_END();
}
