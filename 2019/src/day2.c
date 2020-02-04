#include <errno.h>
#include <stdio.h>
#include <string.h>


enum { buf_max = 32, };

/** @brief Print an error when a file cannot be opened.
 *  @param path the path to the file.
**/
void fopenerr(char const* path)
{
        fprintf(stderr, "Could not open %s: ", path);
        perror(0);
        errno = 0;
}

/** @brief Open an input file and count the number of opcodes.
 *  Opcodes are expected to be unsigned integer, on the first line of the file with always the same separator.
 *  This function assumes that the file is encoded in ASCII.
 *  It also assumes the absence of trailing separator.
 *  This function intends to scan the file in a first pass to know the exact number of opcodes.
 *  The return values gives the size of the buffer that is needed to parse the file entirely.
 *  @param path the path to the file to open
 *  @param sep the separator between the opcodes
 *  @return the number of separators found + 1
**/
unsigned opcode_cnt(char const* path, char const sep)
{
        unsigned sum = 0;
        char buffer[buf_max] = { 0 };
        FILE* instream = fopen(path, "r");
        if (instream) {
                while (fgets(buffer, buf_max, instream)) {
                        for (size_t i = 0; i < buf_max; ++i) {
                                char curr = buffer[i];
                                if (curr == sep) {
                                        ++sum;
                                } else if (curr == '\0' || curr == '\n') {
                                        break;
                                }
                        }
                }
                fclose(instream);
                if (sum > 0) {
                        /* If we found at least a separator,
                           then there are at least two values. */
                        ++sum;
                }
        } else {
                fopenerr(path);
        }
        return sum;
}

/** Parse the opcodes from a file.
 *  The opcodes can reference positions in the same opcode array.
 *  Thus to later compute the opcode result, we need to know the full opcode array.
 *  Consequently, this function assumes that @buffer is large enough to store all the opcodes.
 *  @param path the path to the file
 *  @param len the length of the array which stores the opcodes
 *  @param buffer the array to store the opcodes, must be initialized outside the function
 *  @return 0 in case of success to read the file, non-zero otherwise.
**/
int opcode_rx(char const* path, size_t const len,
              unsigned buffer[static len], char const sep)
{
        int ret = -1;
        int scres;
        char const fmt[3] = { '%', 'u', sep };
        if (len > 0) {
                FILE* instream = fopen(path, "r");
                if (instream) {
                        for (size_t i = 0; i < len - 1; ++i) {
                                scres = fscanf(instream, fmt, &buffer[i]);
                                if (scres < 1) {
                                        /* unexpected error */
                                        perror(0);
                                        errno = 0;
                                        return ret;
                                }
                        }
                        /* no trailing separator expected */
                        fscanf(instream, "%u", &buffer[len-1]);
                        ret = 0;
                        fclose(instream);
                } else {
                        fopenerr(path);
                }
        }
        return ret;
}

/** @brief Execute the Intcode program.
 *  Blocks of 4 opcodes are expected.
 *  Starting code meaning:
 *      1 = addition
 *      2 = multiplication
 *      99 = end of programm
 *  The 2 following codes are the positions of the arguments.
 *  The last code in the block encodes the postion of the result.
 *  @param len length of the opcode array
 *  @param opcodes Incode program as array of opcodes, this array is modified by the function!
 *  @return the number of operations executed, -1 if an error occured.
**/
int opcode_exec(size_t const len, unsigned opcodes[static len])
{
        int ret = -1;
        size_t i = 0;
        unsigned op, arg1, arg2;
        if (len) {
                ret = 0;
                while (i < len) {
                        op = opcodes[i];
                        switch (op) {
                                case 1: arg1 = opcodes[opcodes[i+1]];
                                        arg2 = opcodes[opcodes[i+2]];
                                        opcodes[opcodes[i+3]] = arg1 + arg2;
                                        ++ret;
                                        break;
                                case 2: arg1 = opcodes[opcodes[i+1]];
                                        arg2 = opcodes[opcodes[i+2]];
                                        opcodes[opcodes[i+3]] = arg1 * arg2;
                                        ++ret;
                                        break;
                                case 99: i = len;
                                         break;
                                default: fprintf(stderr,
                                                 "Unknown opcode %u after %u operations.\n",
                                                 op, ret);
                                         ret = -1;
                                         break;
                        }
                        i += 4;
                }
        }
        return ret;
}

/** @brief Finds the optimal noun and verbs of the intcode that returns @target.
 *  This is basically an optimization problem where we try to minimize the distance to @target.
 *  In other words, we try to minimize: \f$ target - opcodes[0] \f$
 *  where: \f$ opcodes[0] = f(opcodes[1], opcodes[2]) \f$
 *  Since the noun and verb, i.e. @opcodes[1] and @opcodes[2], range from 0 to 99,
 *  we chose an extensive approach where all combinations are tested.
 *  Worst case represents 10 000 iterations.
 *  @param len length of the opcode array
 *  @param opcodes Incode program as array of opcodes, will be copied
 *  @return 100 * noun + verb, 0 in case of failure
**/
unsigned opcode_opt(unsigned const target,
                    size_t const len, unsigned const opcodes[static len])
{
        if (len > 0) {
                unsigned tmp[len];
                int cnt = 0;
                for (size_t noun = 0; noun < 100; ++noun) {
                        for (size_t verb = 0; verb < 100; ++verb) {
                                memcpy(tmp, opcodes, sizeof(unsigned) * len);
                                tmp[1] = noun;
                                tmp[2] = verb;
                                cnt = opcode_exec(len, tmp);
                                if (cnt > 0 && tmp[0] == target) {
                                        return 100 * noun + verb;
                                }
                        }
                }
        }
        return 0;
}


int main(void)
{
        char const* progpath = "2019/data/input-day2.txt";
        unsigned proglen = opcode_cnt(progpath, ',');
        unsigned intcode[proglen];
        int res = opcode_rx(progpath, proglen, intcode, ',');
        if (!res) {
                // Replacements from the instructions (part 1)
                intcode[1] = 12;
                intcode[2] = 2;
                int cnt = opcode_exec(proglen, intcode);
                if (cnt > 0) {
                        fprintf(stdout, "%d operations executed.\n", cnt);
                        fprintf(stdout, "Anwser to part 1: %u\n", intcode[0]);
                } else {
                        fprintf(stderr, "Execution error (%d).\n", cnt);
                        res = cnt;
                }
        }
        // Reset for part 2
        res = opcode_rx(progpath, proglen, intcode, ',');
        if (!res) {
                unsigned ans = opcode_opt(19690720, proglen, intcode);
                if (ans) {
                        fprintf(stdout, "Anwser to part 2: %u\n", ans);
                } else {
                        fputs("No answer found", stderr);
                }
        }
        return res;
}
