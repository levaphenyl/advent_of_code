#include "day7.h"

int main(void)
{
        int ret = EXIT_FAILURE;
        char* const path = "2019/data/input-day7.txt";
        char const sep = ',';
        FILE* instream = fopen(path, "r");
        if (instream) {
                unsigned oplen = opcode_cnt(instream, sep);
                int intcodes[oplen];
                rewind(instream);
                _Bool rxok = opcode_rx(instream, oplen, intcodes, sep);
                fclose(instream);
                if (rxok) {
                        puts("Part 1:");
                        int max_sig = max_amp_signal(oplen, intcodes);
                        printf("highest amplifier signal: %d\n", max_sig);
                } else {
                        fputs("Error while reading the opcodes.\n", stderr);
                }
        } else {
                fprintf(stderr, "Error: could not open %s.\n", path);
        }

        return ret;
}

