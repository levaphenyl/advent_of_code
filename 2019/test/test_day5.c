#include "unity.h"
#include "../src/day5.h"


/* Constructor/destructor for tests.
   Leave empty if not explicitly needed. */
void setUp(void) {}
void tearDown(void) {}

void test_opcode_cnt(void)
{
        char* filename = "2019/test/data/input-day5.txt";
        char sep = ',';
        FILE* teststream = fopen(filename, "r");
        if (teststream) {
                TEST_ASSERT_EQUAL_UINT(12, opcode_cnt(teststream, sep));
                TEST_ASSERT_EQUAL_UINT(0, opcode_cnt(teststream, sep));
                TEST_ASSERT_EQUAL_UINT(5, opcode_cnt(teststream, sep));
                TEST_ASSERT_EQUAL_UINT(5, opcode_cnt(teststream, sep));
                fclose(teststream);
        } else {
                TEST_FAIL_MESSAGE("Cannot open test data.");
        }
}

void test_opcode_rx(void)
{
        char* filename = "2019/test/data/input-day5.txt";
        char const sep = ',';
        FILE* teststream = fopen(filename, "r");
        if (teststream) {
                int expected1[12] = { 1, 9, 10, 3, 2, 3, 11, 0, 99, 30, 40, 50, };
                int buff1[12] = { 0 };
                opcode_rx(teststream, 12, buff1, sep);
                TEST_ASSERT_EQUAL_INT_ARRAY(expected1, buff1, 12);
                TEST_ASSERT_EQUAL_UINT(0, opcode_rx(teststream, 0, buff1, sep));
                int expected2[5] = { 3, 0, 4, 0, 99, };
                int buff2[5] = { 0 };
                opcode_rx(teststream, 5, buff2, sep);
                TEST_ASSERT_EQUAL_INT_ARRAY(expected2, buff2, 5);
                int expected3[5] = { 1101, 100, -1, 4, 0, };
                int buff3[5] = { 0 };
                opcode_rx(teststream, 5, buff3, sep);
                TEST_ASSERT_EQUAL_INT_ARRAY(expected3, buff3, 5);
                fclose(teststream);
        } else {
                TEST_FAIL_MESSAGE("Cannot open test data.");
        }
}

void test_opcode_mode(void)
{
        int ret;
        int op = 1002;
        _Bool isval1 = 0;
        _Bool isval2 = 0;
        _Bool isval3 = 0;
        ret = opcode_mode(&op, &isval1, &isval2, &isval3);
        TEST_ASSERT_EQUAL_INT(1, ret);
        TEST_ASSERT_EQUAL_INT(2, op);
        TEST_ASSERT_EQUAL_UINT(0, isval1);
        TEST_ASSERT_EQUAL_UINT(1, isval2);
        TEST_ASSERT_EQUAL_UINT(0, isval3);
        op = 99;
        isval1 = 0;
        isval2 = 0;
        isval3 = 0;
        ret = opcode_mode(&op, &isval1, &isval2, &isval3);
        TEST_ASSERT_EQUAL_INT(0, ret);
        TEST_ASSERT_EQUAL_INT(99, op);
        TEST_ASSERT_EQUAL_UINT(0, isval1);
        TEST_ASSERT_EQUAL_UINT(0, isval2);
        TEST_ASSERT_EQUAL_UINT(0, isval3);
        op = 10101;
        isval1 = 0;
        isval2 = 0;
        isval3 = 0;
        ret = opcode_mode(&op, &isval1, &isval2, &isval3);
        TEST_ASSERT_EQUAL_INT(1, ret);
        TEST_ASSERT_EQUAL_INT(1, op);
        TEST_ASSERT_EQUAL_UINT(1, isval1);
        TEST_ASSERT_EQUAL_UINT(0, isval2);
        TEST_ASSERT_EQUAL_UINT(1, isval3);
}

void test_opcode_exec(void)
{
        int res;
        int in1[12] = { 1, 9, 10, 3, 2, 3, 11, 0, 99, 30, 40, 50, };
        int out1[12] = { 3500, 9, 10, 70, 2, 3, 11, 0, 99, 30, 40, 50, };
        res = opcode_exec(12, in1);
        TEST_ASSERT_EQUAL_INT(2, res);
        TEST_ASSERT_EQUAL_INT_ARRAY(out1, in1, 12);
        puts("Enter 12 below.");
        int in2[5] = { 3, 0, 4, 0, 99, };
        int out2[5] = { 12, 0, 4, 0, 99, };
        res = opcode_exec(5, in2);
        TEST_ASSERT_EQUAL_INT(2, res);
        TEST_ASSERT_EQUAL_INT_ARRAY(out2, in2, 5);
        int in3[5] = { 1002, 4, 3, 4, 33, };
        int out3[5] = { 1002, 4, 3, 4, 99, };
        res = opcode_exec(5, in3);
        TEST_ASSERT_EQUAL_INT(1, res);
        TEST_ASSERT_EQUAL_INT_ARRAY(out3, in3, 5);
        int in4[5] = { 1101, 100, -1, 4, 0, };
        int out4[5] = { 1101, 100, -1, 4, 99, };
        res = opcode_exec(5, in4);
        TEST_ASSERT_EQUAL_INT(1, res);
        TEST_ASSERT_EQUAL_INT_ARRAY(out4, in4, 5);
}

void test_opcode_exec2(void)
{
        int res;
        int in1[12] = { 1, 9, 10, 3, 2, 3, 11, 0, 99, 30, 40, 50, };
        int out1[12] = { 3500, 9, 10, 70, 2, 3, 11, 0, 99, 30, 40, 50, };
        res = opcode_exec2(12, in1);
        TEST_ASSERT_EQUAL_INT(2, res);
        TEST_ASSERT_EQUAL_INT_ARRAY(out1, in1, 12);
        puts("Enter 57 below.");
        int in2[5] = { 3, 0, 4, 0, 99, };
        int out2[5] = { 57, 0, 4, 0, 99, };
        res = opcode_exec2(5, in2);
        TEST_ASSERT_EQUAL_INT(2, res);
        TEST_ASSERT_EQUAL_INT_ARRAY(out2, in2, 5);
        int in3[5] = { 1002, 4, 3, 4, 33, };
        int out3[5] = { 1002, 4, 3, 4, 99, };
        res = opcode_exec2(5, in3);
        TEST_ASSERT_EQUAL_INT(1, res);
        TEST_ASSERT_EQUAL_INT_ARRAY(out3, in3, 5);
        int in4[5] = { 1101, 100, -1, 4, 0, };
        int out4[5] = { 1101, 100, -1, 4, 99, };
        res = opcode_exec2(5, in4);
        TEST_ASSERT_EQUAL_INT(1, res);
        TEST_ASSERT_EQUAL_INT_ARRAY(out4, in4, 5);
        int in5[9] = { 5, 8, 7, 1102, 3, 3, 0, 99, 1, };
        res = opcode_exec2(9, in5);
        TEST_ASSERT_EQUAL_INT(1, res);
        TEST_ASSERT_EQUAL_INT_ARRAY(in5, in5, 9);
        int in6[9] = { 6, 8, 7, 2, 1, 2, 0, 99, 1, };
        int out6[9] = { 56, 8, 7, 2, 1, 2, 0, 99, 1, };
        res = opcode_exec2(9, in6);
        TEST_ASSERT_EQUAL_INT(2, res);
        TEST_ASSERT_EQUAL_INT_ARRAY(out6, in6, 9);
        int in7[5] = { 1107, 1, 8, 3, 99, };
        int out7[5] = { 1107, 1, 8, 1, 99, };
        res = opcode_exec2(5, in7);
        TEST_ASSERT_EQUAL_INT(1, res);
        TEST_ASSERT_EQUAL_INT_ARRAY(out7, in7, 5);
        int in8[7] = { 8, 5, 6, 5, 99, 8, 8, };
        int out8[7] = { 8, 5, 6, 5, 99, 1, 8, };
        res = opcode_exec2(7, in8);
        TEST_ASSERT_EQUAL_INT(1, res);
        TEST_ASSERT_EQUAL_INT_ARRAY(out8, in8, 7);
}


int main(void)
{
        UNITY_BEGIN();
        RUN_TEST(test_opcode_cnt);
        RUN_TEST(test_opcode_rx);
        RUN_TEST(test_opcode_mode);
        RUN_TEST(test_opcode_exec);
        RUN_TEST(test_opcode_exec2);
        return UNITY_END();
}

