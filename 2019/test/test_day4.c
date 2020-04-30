#include "unity.h"
#include "../src/day4.h"


/* Constructor/destructor for tests.
   Leave empty if not explicitly needed. */
void setUp(void) {}
void tearDown(void) {}

void test_upow(void)
{
        TEST_ASSERT_EQUAL_UINT(1, upow(213, 0));
        TEST_ASSERT_EQUAL_UINT(8, upow(2, 3));
        TEST_ASSERT_EQUAL_UINT(1000, upow(10, 3));
        TEST_ASSERT_EQUAL_UINT(1024, upow(2, 10));
}

void test_uint2arr(void)
{
        unsigned tmp_res[pwd_sz];
        unsigned zero = 0;
        unsigned res_zero[pwd_sz] = { 0 };
        uint2arr(zero, tmp_res);
        TEST_ASSERT_EQUAL_UINT_ARRAY(res_zero, tmp_res, pwd_sz);
        unsigned ex1 = 122345;
        unsigned res_ex1[pwd_sz] = { 1, 2, 2, 3, 4, 5, };
        uint2arr(ex1, tmp_res);
        TEST_ASSERT_EQUAL_UINT_ARRAY(res_ex1, tmp_res, pwd_sz);
        unsigned ex2 = 333357;
        unsigned res_ex2[pwd_sz] = { 3, 3, 3, 3, 5, 7, };
        uint2arr(ex2, tmp_res);
        TEST_ASSERT_EQUAL_UINT_ARRAY(res_ex2, tmp_res, pwd_sz);
}

void test_is_compliant(void)
{
        unsigned yes1[pwd_sz] = { 1, 2, 2, 3, 4, 5, };
        unsigned yes2[pwd_sz] = { 1, 1, 1, 1, 1, 1, };
        unsigned no1[pwd_sz] = { 2, 2, 3, 4, 5, 0, };
        unsigned no2[pwd_sz] = { 1, 2, 3, 7, 8, 9, };
        TEST_ASSERT_EQUAL_UINT(1, is_compliant(yes1));
        TEST_ASSERT_EQUAL_UINT(1, is_compliant(yes2));
        TEST_ASSERT_EQUAL_UINT(0, is_compliant(no1));
        TEST_ASSERT_EQUAL_UINT(0, is_compliant(no2));
}

void test_is_compliant2(void)
{
        unsigned yes1[pwd_sz] = { 1, 1, 2, 2, 3, 3, };
        unsigned yes2[pwd_sz] = { 1, 1, 1, 1, 2, 2, };
        unsigned no1[pwd_sz] = { 1, 2, 3, 4, 4, 4, };
        unsigned no2[pwd_sz] = { 1, 2, 3, 7, 8, 9, };
        TEST_ASSERT_EQUAL_UINT(1, is_compliant2(yes1));
        TEST_ASSERT_EQUAL_UINT(1, is_compliant2(yes2));
        TEST_ASSERT_EQUAL_UINT(0, is_compliant2(no1));
        TEST_ASSERT_EQUAL_UINT(0, is_compliant2(no2));
}


int main(void)
{
        UNITY_BEGIN();
        RUN_TEST(test_upow);
        RUN_TEST(test_uint2arr);
        RUN_TEST(test_is_compliant);
        RUN_TEST(test_is_compliant2);
        return UNITY_END();
}

