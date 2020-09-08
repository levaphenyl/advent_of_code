#include "unity.h"
#include "../src/day7.h"


/* Constructor/destructor for tests.
   Leave empty if not explicitly needed. */
void setUp(void) {}
void tearDown(void) {}

void test_opcode_exec_in_out(void)
{
        int res;
        int in1[12] = { 1, 9, 10, 3, 2, 3, 11, 0, 99, 30, 40, 50, };
        int out1[12] = { 3500, 9, 10, 70, 2, 3, 11, 0, 99, 30, 40, 50, };
        int* in_empty = { 0 };
        res = opcode_exec_in_out(0, in_empty, 12, in1);
        TEST_ASSERT_EQUAL_INT(0, res);
        TEST_ASSERT_EQUAL_INT_ARRAY(out1, in1, 12);
        int in2[5] = { 3, 0, 4, 0, 99, };
        int out2[5] = { 57, 0, 4, 0, 99, };
        int inputs[1] = { 57, };
        res = opcode_exec_in_out(1, inputs, 5, in2);
        TEST_ASSERT_EQUAL_INT(57, res);
        TEST_ASSERT_EQUAL_INT_ARRAY(out2, in2, 5);
        int in3[5] = { 1002, 4, 3, 4, 33, };
        int out3[5] = { 1002, 4, 3, 4, 99, };
        res = opcode_exec_in_out(0, in_empty, 5, in3);
        TEST_ASSERT_EQUAL_INT(0, res);
        TEST_ASSERT_EQUAL_INT_ARRAY(out3, in3, 5);
        int in4[5] = { 1101, 100, -1, 4, 0, };
        int out4[5] = { 1101, 100, -1, 4, 99, };
        res = opcode_exec_in_out(0, in_empty, 5, in4);
        TEST_ASSERT_EQUAL_INT(0, res);
        TEST_ASSERT_EQUAL_INT_ARRAY(out4, in4, 5);
        int in5[9] = { 5, 8, 7, 1102, 3, 3, 0, 99, 1, };
        res = opcode_exec_in_out(0, in_empty, 9, in5);
        TEST_ASSERT_EQUAL_INT(0, res);
        TEST_ASSERT_EQUAL_INT_ARRAY(in5, in5, 9);
        int in6[9] = { 6, 8, 7, 2, 1, 2, 0, 99, 1, };
        int out6[9] = { 56, 8, 7, 2, 1, 2, 0, 99, 1, };
        res = opcode_exec_in_out(0, in_empty, 9, in6);
        TEST_ASSERT_EQUAL_INT(0, res);
        TEST_ASSERT_EQUAL_INT_ARRAY(out6, in6, 9);
        int in7[5] = { 1107, 1, 8, 3, 99, };
        int out7[5] = { 1107, 1, 8, 1, 99, };
        res = opcode_exec_in_out(0, in_empty, 5, in7);
        TEST_ASSERT_EQUAL_INT(0, res);
        TEST_ASSERT_EQUAL_INT_ARRAY(out7, in7, 5);
        int in8[7] = { 8, 5, 6, 5, 99, 8, 8, };
        int out8[7] = { 8, 5, 6, 5, 99, 1, 8, };
        res = opcode_exec_in_out(0, in_empty, 7, in8);
        TEST_ASSERT_EQUAL_INT(0, res);
        TEST_ASSERT_EQUAL_INT_ARRAY(out8, in8, 7);
}

void test_amp_exec(void)
{
        int opcodes[17] = { 3, 15, 3, 16, 1002, 16, 10, 16, 1, 16, 15, 15, 4,
                            15, 99, 0, 0 };
        int input1[2] = { 0, 0, };
        TEST_ASSERT_EQUAL_INT(0, amp_exec(2, input1, 17, opcodes));
        int input2[2] = { 4, 0, };
        TEST_ASSERT_EQUAL_INT(4, amp_exec(2, input2, 17, opcodes));
        int input3[2] = { 7, 5, };
        TEST_ASSERT_EQUAL_INT(57, amp_exec(2, input3, 17, opcodes));
}

void test_powi(void)
{
        TEST_ASSERT_EQUAL_INT(1000, powi(10, 3));
        TEST_ASSERT_EQUAL_INT(1, powi(57, 0));
        TEST_ASSERT_EQUAL_INT(16, powi(2, 4));
}

void test_max_amp_signal(void)
{
        int opcodes1[17] = { 3, 15, 3, 16, 1002, 16, 10, 16, 1, 16, 15, 15, 4,
                            15, 99, 0, 0 };
        TEST_ASSERT_EQUAL_INT(43210, max_amp_signal(17, opcodes1));
        int opcodes2[25] = { 3, 23, 3, 24, 1002, 24, 10, 24, 1002, 23, -1, 23,
                             101, 5, 23, 23, 1, 24, 23, 23, 4, 23, 99, 0, 0 };
        TEST_ASSERT_EQUAL_INT(54321, max_amp_signal(25, opcodes2));
}


int main(void)
{
        UNITY_BEGIN();
        RUN_TEST(test_opcode_exec_in_out);
        RUN_TEST(test_amp_exec);
        RUN_TEST(test_powi);
        RUN_TEST(test_max_amp_signal);
        return UNITY_END();
}

