#include "unity.h"
#include "../src/day2.c"


/* Constructor/destructor for tests.
   Leave empty if not explicitly needed. */
void setUp(void) {}
void tearDown(void) {}

void test_opcode_cnt(void)
{
        unsigned simple = opcode_cnt(
                "2019/test/data/input-day2.txt",
                ','
        );
        unsigned empty = opcode_cnt(
                "2019/test/data/input-day2-empty.txt",
                ','
        );
        unsigned longer = opcode_cnt(
                "2019/test/data/input-day2-long.txt",
                ','
        );
        TEST_ASSERT_EQUAL_UINT(5, simple);
        TEST_ASSERT_EQUAL_UINT(0, empty);
        TEST_ASSERT_EQUAL_UINT(68, longer);
}

void test_opcode_rx(void)
{
        unsigned simple[5] = { 0 };
        unsigned simple_exp[5] = { 1, 0, 0, 0, 99 };
        opcode_rx("2019/test/data/input-day2.txt", 5, simple, ',');
        unsigned empty[1] = { 0 };
        unsigned empty_exp[1] = { 0 };
        opcode_rx("2019/test/data/input-day2-empty.txt", 1, empty, ',');
        TEST_ASSERT_EQUAL_UINT_ARRAY(simple_exp, simple, 5);
        TEST_ASSERT_EQUAL_UINT_ARRAY(empty_exp, empty, 1);
}

void test_opcode_exec(void)
{
        unsigned empty[0];
        int ret = opcode_exec(0, empty);
        unsigned prog1[5] = { 1, 0, 0, 0, 99 };
        unsigned prog1_res[5] = { 2, 0, 0, 0, 99 };
        opcode_exec(5, prog1);
        unsigned prog2[5] = { 2, 3, 0, 3, 99 };
        unsigned prog2_res[5] = { 2, 3, 0, 6, 99 };
        opcode_exec(5, prog2);
        unsigned prog3[6] = { 2, 4, 4, 5, 99, 0 };
        unsigned prog3_res[6] = { 2, 4, 4, 5, 99, 9801 };
        opcode_exec(6, prog3);
        unsigned prog4[9] = { 1, 1, 1, 4, 99, 5, 6, 0, 99 };
        unsigned prog4_res[9] = { 30, 1, 1, 4, 2, 5, 6, 0, 99 };
        opcode_exec(9, prog4);
        unsigned prog5[12] = { 1, 9, 10, 3, 2, 3, 11, 0, 99, 30, 40, 50 };
        unsigned prog5_res[12] = { 3500, 9, 10, 70, 2, 3, 11, 0, 99, 30, 40, 50 };
        opcode_exec(12, prog5);
        TEST_ASSERT_EQUAL_INT(-1, ret);
        TEST_ASSERT_EQUAL_UINT_ARRAY(prog1_res, prog1, 5);
        TEST_ASSERT_EQUAL_UINT_ARRAY(prog2_res, prog2, 5);
        TEST_ASSERT_EQUAL_UINT_ARRAY(prog3_res, prog3, 6);
        TEST_ASSERT_EQUAL_UINT_ARRAY(prog4_res, prog4, 9);
        TEST_ASSERT_EQUAL_UINT_ARRAY(prog5_res, prog5, 12);
}


int main(void)
{
        UNITY_BEGIN();
        RUN_TEST(test_opcode_cnt);
        RUN_TEST(test_opcode_rx);
        RUN_TEST(test_opcode_exec);
        return UNITY_END();
}
