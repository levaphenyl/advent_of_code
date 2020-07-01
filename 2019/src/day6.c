#include "day6.h"


int main(void)
{
        int ret = EXIT_SUCCESS;
        planet sys_root = {
                .id = "COM",
                .sats = { 0 },
                .sat_idx = 0,
        };
        char* const datapath = "2019/data/input-day6.txt";
        unsigned n_planets = read_planet_system(datapath, &sys_root);
        if (n_planets) {
                unsigned cksum = orbit_checksum(&sys_root, 0);
                printf("Part 1: %u\n", cksum);
                int ntrans = transfer_length(&sys_root, "YOU", "SAN");
                printf("Part 2: %d\n", ntrans);
                destroy_planets(&sys_root);
        } else {
                ret = EXIT_FAILURE;
        }
        return ret;
}
