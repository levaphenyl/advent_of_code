#include <errno.h>
#include <stdio.h>
#include <stdlib.h>


/* Size of the buffer to parse the input.
   We expect only numbers. */
enum { buf_max = 32, };

/* Piecewise function for part 1 */
unsigned mod_fuel(unsigned mass)
{
        unsigned res = mass/3;
        if (res > 2) {
                return res - 2;
        } else {
                return 0;
        }
}

/* Piecewise recursive function for part 2 */
unsigned mod_fuel_rec(unsigned mass)
{
        if (mass > 2) {
                unsigned fuelstep = mod_fuel(mass);
                /* Not tail-recursive :( */
                return fuelstep + mod_fuel_rec(fuelstep);
        } else {
                return 0;
        }
}

/* Parse unsigned integer values from a file, transform them one by one and computes the sum.
   This function assumes one value per line.
   Input: path to the file and transformation function.
   Output: sum of the transformed values. */
unsigned file_parse_sum(char const* path, unsigned func(unsigned))
{
        unsigned acc = 0;
        unsigned new = 0;
        FILE* instream = fopen(path, "r");
        if (instream) {
                while ( fscanf(instream, "%u\n", &new) > 0 ) {
                        acc += func(new);
                }
                fclose(instream);
        } else {
                fprintf(stderr, "Could not open %s: ", path);
                perror(0);
                errno = 1;
        }
        return acc;
}


int main(void)
{
        unsigned part1 = file_parse_sum("2019/data/input-day1.txt", mod_fuel);
        fprintf(stdout, "Answer for part 1 is: %u\n", part1);
        unsigned part2 = file_parse_sum("2019/data/input-day1.txt", mod_fuel_rec);
        fprintf(stdout, "Answer for part 2 is: %u\n", part2);
}
