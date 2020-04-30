#include <stdio.h>
#include "day4.h"


int main(void)
{
        fputs("Day 4\n", stdout);
        fputs("-----\n", stdout);
        unsigned n_pwd = 0;
        unsigned n_pwd2 = 0;
        unsigned tmp[pwd_sz];
        for (unsigned c = 278384; c <= 824795; ++c) {
                uint2arr(c, tmp);
                if (is_compliant(tmp)) {
                        ++n_pwd;
                }
                if (is_compliant2(tmp)) {
                        ++n_pwd2;
                }
        }
        fprintf(stdout, "Part 1: %u passwords meet the criteria.\n", n_pwd);
        fprintf(stdout, "Part 2: %u passwords meet the criteria.\n", n_pwd2);
}

