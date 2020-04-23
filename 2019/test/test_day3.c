#include "unity.h"
#include "../src/day3.c"


/* Constructor/destructor for tests.
   Leave empty if not explicitly needed. */
void setUp(void) {}
void tearDown(void) {}

void test_crossprod(void)
{
        float zero[2] = {0.0, 0.0};
        float hori[2] = {1.0, 0.0};
        float vert[2] = {0.0, 1.0};
        float arbi[2] = {5.0, 7.0};
        TEST_ASSERT_EQUAL_FLOAT(0.0, crossprod(zero, zero));
        TEST_ASSERT_EQUAL_FLOAT(0.0, crossprod(arbi, arbi));
        TEST_ASSERT_EQUAL_FLOAT(1.0, crossprod(hori, vert));
        TEST_ASSERT_EQUAL_FLOAT(-5.0, crossprod(vert, arbi));
}

void test_manhattan_dist(void)
{
        float ptO[2] = { 0 };
        float ptA[2] = { 3, 3 };
        float ptB[2] = { 6, 5 };
        TEST_ASSERT_EQUAL_FLOAT(0, manhattan_dist(ptO, ptO));
        TEST_ASSERT_EQUAL_FLOAT(6, manhattan_dist(ptO, ptA));
        TEST_ASSERT_EQUAL_FLOAT(11, manhattan_dist(ptO, ptB));
        TEST_ASSERT_EQUAL_FLOAT(5, manhattan_dist(ptA, ptB));
        TEST_ASSERT_EQUAL_FLOAT(5, manhattan_dist(ptB, ptA));
        TEST_ASSERT_EQUAL_FLOAT(0, manhattan_dist(ptA, ptA));
}

void test_seg_intersect(void)
{
        float inter_pt[2];
        segment inter_seg1 = { { 6, 7 }, { 0, -4} };
        segment inter_seg2 = { { 8, 5 }, { -5, 0 } };
        float expected1[2] = { 6, 5 };
        TEST_ASSERT_EQUAL_UINT(1, seg_intersect(inter_seg1, inter_seg2, inter_pt));
        TEST_ASSERT_EQUAL_FLOAT_ARRAY(expected1, inter_pt, 2);
        segment inter_seg3 = { { 6, 3 }, { -4, 0 } };
        segment inter_seg4 = { { 3, 5 }, { 0, -3 } };
        float expected2[2] = { 3, 3 };
        TEST_ASSERT_EQUAL_UINT(1, seg_intersect(inter_seg3, inter_seg4, inter_pt));
        TEST_ASSERT_EQUAL_FLOAT_ARRAY(expected2, inter_pt, 2);
        segment parallel_seg5 = { { 0, 1 }, { 0, 1 } };
        segment parallel_seg6 = { { 3, 5 }, { 0, 7 } };
        TEST_ASSERT_EQUAL_UINT(0,
                seg_intersect(parallel_seg5, parallel_seg6, inter_pt));
}

void test_cnt_csv_width(void)
{
        FILE* stream = fopen("2019/test/data/input-day3-cnt.csv", "r");
        if (stream) {
                TEST_ASSERT_EQUAL_UINT(3, cnt_csv_width(stream, ','));
                TEST_ASSERT_EQUAL_UINT(13, cnt_csv_width(stream, ','));
                TEST_ASSERT_EQUAL_UINT(0, cnt_csv_width(stream, ','));
                fclose(stream);
        } else {
                TEST_FAIL_MESSAGE("Cannot open test data.");
        }
}

void test_wire_rx(void)
{
        FILE* stream = fopen("2019/test/data/input-day3.csv", "r");
        segment res1[4] = { 0 };
        segment res2[4] = { 0 };
        if (stream) {
                segment expected1[4] = {
                        { { 0, 0 }, { 8, 0 } },
                        { { 8, 0 }, { 0, 5 } },
                        { { 8, 5 }, { -5, 0 } },
                        { { 3, 5 }, { 0, -3 } },
                };
                segment expected2[4] = {
                        { { 0, 0 }, { 0, 7 } },
                        { { 0, 7 }, { 6, 0 } },
                        { { 6, 7 }, { 0, -4 } },
                        { { 6, 3 }, { -4, 0 } },
                };
                TEST_ASSERT_EQUAL_UINT(4, wire_rx(stream, 4, res1));
                TEST_ASSERT_EQUAL_UINT(4, wire_rx(stream, 4, res2));
                for (int i = 0; i < 4; ++i ) {
                        TEST_ASSERT_EQUAL_FLOAT_ARRAY(
                                expected1[i].start_pt, res1[i].start_pt, 2);
                        TEST_ASSERT_EQUAL_FLOAT_ARRAY(
                                expected1[i].vect, res1[i].vect, 2);
                        TEST_ASSERT_EQUAL_FLOAT_ARRAY(
                                expected2[i].start_pt, res2[i].start_pt, 2);
                        TEST_ASSERT_EQUAL_FLOAT_ARRAY(
                                expected2[i].vect, res2[i].vect, 2);
                }
                fclose(stream);
        } else {
                TEST_FAIL_MESSAGE("Cannot open test data.");
        }
}


int main(void)
{
        UNITY_BEGIN();
        RUN_TEST(test_crossprod);
        RUN_TEST(test_manhattan_dist);
        RUN_TEST(test_seg_intersect);
        RUN_TEST(test_cnt_csv_width);
        RUN_TEST(test_wire_rx);
        return UNITY_END();
}
