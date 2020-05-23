#include "day5.h"

int main(void)
{
        int ret = 1;
        char* const path = "2019/data/input-day5.txt";
        char const sep = ',';
        FILE* instream = fopen(path, "r");
        if (instream) {
                unsigned oplen = opcode_cnt(instream, sep);
                int intcodes[oplen];
                int intcodes2[oplen]; /* copy the original for part2 */
                rewind(instream);
                _Bool rxok = opcode_rx(instream, oplen, intcodes, sep);
                if (rxok) {
                        /* copy the original for part2 */
                        memcpy(intcodes2, intcodes, oplen*sizeof(int));
                        fputs("Part 1:\n", stdout);
                        int cnt = opcode_exec(oplen, intcodes);
                        printf("%d operations executed.\n", cnt);
                        fputs("\nPart 2:\n", stdout);
                        cnt = opcode_exec2(oplen, intcodes2);
                        printf("%d operations executed.\n", cnt);
                        ret = 0;
                } else {
                        fputs("Error while reading the opcodes.\n", stderr);
                }
                fclose(instream);
        } else {
                fprintf(stderr, "Error: could not open %s.\n", path);
        }
        return ret;
}

