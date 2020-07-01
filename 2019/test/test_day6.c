#include "unity.h"
#include "../src/day6.h"

#define SYS_ROOT { .id = "COM", .sats = { 0 }, .sat_idx = 0, }


/* Constructor and destructor for tests.
 * Leave empty if not explicitely needed. */
void setUp(void) {}
void tearDown(void) {}

void test_append_sat(void)
{
        planet sys_root = SYS_ROOT;
        planet new_sat = {
                .id = "AAA",
                .sats = { 0 },
                .sat_idx = 0,
        };
        TEST_ASSERT_EQUAL_UINT(1, append_sat(&sys_root, &new_sat));
        TEST_ASSERT_EQUAL_STRING("AAA", sys_root.sats[0]->id);
        free(sys_root.sats[0]);
}

void test_isinsats(void)
{
        planet sys_root = SYS_ROOT;
        planet new_sat = {
                .id = "AAA",
                .sats = { 0 },
                .sat_idx = 0,
        };
        append_sat(&sys_root, &new_sat);
        TEST_ASSERT_EQUAL_UINT(1, isinsats(&sys_root, "AAA"));
        TEST_ASSERT_EQUAL_UINT(0, isinsats(&sys_root, "BBB"));
}

void test_add_planet(void)
{
        planet sys_root = SYS_ROOT;
        TEST_ASSERT_EQUAL_UINT(1, add_planet(&sys_root, "COM", "AAA"));
        TEST_ASSERT_EQUAL_STRING("AAA", sys_root.sats[0]->id);
        TEST_ASSERT_EQUAL_UINT(0, add_planet(&sys_root, "BBB", "CCC"));
        free(sys_root.sats[0]);
}

void test_destroy_planets(void)
{
        planet sys_root = SYS_ROOT;
        add_planet(&sys_root, "COM", "AAA");
        add_planet(&sys_root, "COM", "BBB");
        add_planet(&sys_root, "BBB", "CCC");
        TEST_ASSERT_EQUAL_UINT(2, destroy_planets(&sys_root));
}

void test_find_center(void)
{
        planet sys_root = SYS_ROOT;
        add_planet(&sys_root, "COM", "AAA");
        add_planet(&sys_root, "COM", "BBB");
        add_planet(&sys_root, "BBB", "CCC");
        char sat1[idlen] = "AAA";
        TEST_ASSERT_EQUAL_UINT(0, find_center(&sys_root, sat1, 0));
        TEST_ASSERT_EQUAL_CHAR_ARRAY("COM", sat1, idlen);
        char sat2[idlen] = "CCC";
        TEST_ASSERT_EQUAL_UINT(1, find_center(&sys_root, sat2, 0));
        TEST_ASSERT_EQUAL_CHAR_ARRAY("BBB", sat2, idlen);
}

void test_parse_orbits(void)
{
        char* path = "2019/test/data/input-day6.txt";
        FILE* teststream = fopen(path, "r");
        if (teststream) {
                planet sys_root = SYS_ROOT;
                TEST_ASSERT_EQUAL_UINT(3, parse_orbits(teststream, &sys_root));
                fclose(teststream);
        } else {
                TEST_FAIL_MESSAGE("Cannot open test data.");
        }
}

void test_read_planet_system(void)
{
        char* path = "2019/test/data/input-day6.txt";
        planet sys_root = SYS_ROOT;
        TEST_ASSERT_EQUAL_UINT(12, read_planet_system(path, &sys_root));
        print_planets(&sys_root, 0);
}

void test_orbit_checksum(void)
{
        char* path = "2019/test/data/input-day6.txt";
        planet sys_root = SYS_ROOT;
        read_planet_system(path, &sys_root);
        TEST_ASSERT_EQUAL_UINT(44, orbit_checksum(&sys_root, 0));
}

void test_transfer_length(void)
{
        char* path = "2019/test/data/input-day6-pt2.txt";
        planet sys_root = SYS_ROOT;
        read_planet_system(path, &sys_root);
        TEST_ASSERT_EQUAL_UINT(4, transfer_length(&sys_root, "YOU", "SAN"));
}


int main(void)
{
        UNITY_BEGIN();
        RUN_TEST(test_append_sat);
        RUN_TEST(test_isinsats);
        RUN_TEST(test_add_planet);
        RUN_TEST(test_destroy_planets);
        RUN_TEST(test_find_center);
        RUN_TEST(test_parse_orbits);
        RUN_TEST(test_read_planet_system);
        RUN_TEST(test_orbit_checksum);
        RUN_TEST(test_transfer_length);
        return UNITY_END();
}

