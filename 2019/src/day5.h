#include <errno.h>
#include <stdio.h>
#include <string.h>

enum { buflen = 16, };


/**
 * @brief Count the number of opcodes in a line of a file.
 * Opcodes are expected to be integers, on the same line of the file.
 * The separator is assumed invariant.
 * This function assumes that the file is encoded in ASCII.
 * It also assumes the absence of trailing separator.
 * This function intends to scan the file in a first pass to know the exact number of opcodes.
 * The return values gives the size of the opcode array needed to parse the file entirely.
 *
 * @param instream the opened file handle
 * @param sep the separator between the opcodes
 *
 * @return the number of separators found + 1
**/
unsigned opcode_cnt(FILE* instream, char const sep)
{
        unsigned sum = 0;
        char buffer[buflen] = { 0 };
        while (fgets(buffer, buflen, instream)) {
                for (size_t i = 0; i < buflen; ++i) {
                        char curr = buffer[i];
                        if (curr == sep) {
                                ++sum;
                        } else if (curr == '\0') {
                                break;
                        } else if (curr == '\n') {
                                goto ENDCNT;
                        }
                }
        }
ENDCNT:
        if (sum > 0) {
                /* If we found at least a separator,
                   there are at least two values. */
                ++sum;
        }
        return sum;
}


/**
 * @brief Parse the opcodes from a file.
 * The opcodes can reference positions in the same opcode array.
 * Thus to later compute the opcode result, we need to know the full opcode array.
 * Consequently, this function assumes that @buffer is large enough to store all the opcodes.
 *
 * @param instream the opened file handle
 * @param len the length of the array which stores the opcodes
 * @param buffer the array to store the opcodes, must be initialized outside the function
 *
 * @return 1 in case of success to read the file, zero otherwise.
**/
int opcode_rx(FILE* instream, size_t const len, int buffer[static len], char const sep)
{
        int ret = 0;
        int scres;
        char* const fmt = "%d";
        char const fmtsep[3] = { fmt[0], fmt[1], sep };
        if (len > 0) {
                for (size_t i = 0; i < len - 1; ++i) {
                        scres = fscanf(instream, fmtsep, &buffer[i]);
                        if (scres < 1) {
                                /* unexpected error */
                                perror(0);
                                errno = 0;
                                return ret;
                        }
                }
                /* no trailing separator expected */
                fscanf(instream, fmt, &buffer[len-1]);
                ret = 1;
        }
        return ret;
}

/**
 * @brief Parse the opcode parameter mode.
 * This function assumes a 5-digit integer following the format:
 *      ABCDE   A: mode of parameter 3
 *              B: mode of parameter 2
 *              C: mode of parameter 1
 *              DE: actual operation code.
 *
 * @param op a 5- or 4-digit opcode.
 * @param isval1 mode of the first parameter.
 * @param isval2 mode of the second parameter.
 * @param isval3 mode of the third parameter.
 *
 * @return 1 if the opcode was parsed, 0 otherwise.
 */
int opcode_mode(int* op, _Bool* isval1, _Bool* isval2, _Bool* isval3)
{
        int ret = 0;
        if (*op > 99) {
                *isval3 = *op / 10000;
                *op -= *isval3 * 10000;
                *isval2 = *op / 1000;
                *op -= *isval2 * 1000;
                *isval1 = *op / 100;
                *op -= *isval1 * 100;
                ret = 1;
        }
        return ret;
}

/** @brief Execute the Intcode program.
 *  Starting code meaning:
 *      1 = addition (2 params)
 *      2 = multiplication (2 params)
 *      3 = ask input (1 param)
 *      4 = print output (1 param)
 *      99 = end of programm
 *  This function supports modes (value or position) for parameters.
 *  The last code in the block encodes the postion of the result.
 *  @param len length of the opcode array
 *  @param opcodes intcode program as array of opcodes, this array is modified by the function!
 *  @return the number of operations executed, -1 if an error occured.
**/
int opcode_exec(size_t const len, int opcodes[static len])
{
        int ret = 0;
        size_t i = 0;
        size_t dest_idx;
        int op, arg1, arg2;
        _Bool isval1, isval2, isval3;
        while (i < len) {
                isval1 = 0;
                isval2 = 0;
                isval3 = 0;
                op = opcodes[i];
                opcode_mode(&op, &isval1, &isval2, &isval3);
                switch (op) {
                        /* IF ? THEN : ELSE */
                        case 1: arg1 = isval1 ? opcodes[i+1] : opcodes[opcodes[i+1]];
                                arg2 = isval2 ? opcodes[i+2] : opcodes[opcodes[i+2]];
                                dest_idx = opcodes[i+3];
                                opcodes[dest_idx] = arg1 + arg2;
                                ++ret;
                                i += 4;
                                break;
                        case 2: arg1 = isval1 ? opcodes[i+1] : opcodes[opcodes[i+1]];
                                arg2 = isval2 ? opcodes[i+2] : opcodes[opcodes[i+2]];
                                dest_idx = opcodes[i+3];
                                opcodes[dest_idx] = arg1 * arg2;
                                ++ret;
                                i += 4;
                                break;
                        case 3: dest_idx = opcodes[i+1];
                                printf("Input required at operation %d:\n", ret);
                                if (scanf("%d", &opcodes[dest_idx]) != 1) {
                                        fputs("Invalid input, aborting!\n", stderr);
                                        i = len;
                                }
                                ++ret;
                                i += 2;
                                break;
                        case 4: arg1 = isval1 ? opcodes[i+1] : opcodes[opcodes[i+1]];
                                printf("%d\n", arg1);
                                ++ret;
                                i += 2;
                                break;
                        case 99: i = len;
                                 break;
                        default: fprintf(stderr,
                                         "Unknown opcode %d after %d operations.\n",
                                         op, ret);
                                 ret = -1;
                                 i = len;
                                 break;
                }
        }
        return ret;
}

/** @brief Execute the Intcode program for part 2.
 *  Starting code meaning:
 *      1 = addition (2 params)
 *      2 = multiplication (2 params)
 *      3 = ask input (1 param)
 *      4 = print output (1 param)
 *      5 = jump if true (2 params)
 *      6 = jump if false (2 params)
 *      7 = less than (2 params)
 *      8 = equals (2 params)
 *      99 = end of programm
 *  This function supports modes (value or position) for parameters.
 *  The last code in the block encodes the postion of the result.
 *  @param len length of the opcode array
 *  @param opcodes intcode program as array of opcodes, this array is modified by the function!
 *  @return the number of operations executed, -1 if an error occured.
**/
int opcode_exec2(size_t const len, int opcodes[static len])
{
        int ret = 0;
        size_t i = 0;
        size_t dest_idx;
        int op, arg1, arg2;
        _Bool isval1, isval2, isval3;
        while (i < len) {
                isval1 = 0;
                isval2 = 0;
                isval3 = 0;
                op = opcodes[i];
                /* Assuming operation 99 has no modes */
                if (op == 99) {
                        break;
                }
                opcode_mode(&op, &isval1, &isval2, &isval3);
                /* IF ? THEN : ELSE */
                arg1 = isval1 ? opcodes[i+1] : opcodes[opcodes[i+1]];
                arg2 = isval2 ? opcodes[i+2] : opcodes[opcodes[i+2]];
                dest_idx = opcodes[i+3];
                switch (op) {
                        case 1: opcodes[dest_idx] = arg1 + arg2;
                                i += 4;
                                break;
                        case 2: opcodes[dest_idx] = arg1 * arg2;
                                i += 4;
                                break;
                        case 3: dest_idx = opcodes[i+1];
                                printf("Input required at operation %d:\n", ret);
                                if (scanf("%d", &opcodes[dest_idx]) != 1) {
                                        fputs("Invalid input, aborting!\n", stderr);
                                        i = len;
                                }
                                i += 2;
                                break;
                        case 4: printf("%d\n", arg1);
                                i += 2;
                                break;
                        case 5: if (arg1) {
                                        i = arg2;
                                } else {
                                        i += 3;
                                }
                                break;
                        case 6: if (!arg1) {
                                        i = arg2;
                                } else {
                                        i += 3;
                                }
                                break;
                        case 7: opcodes[dest_idx] = arg1 < arg2;
                                i += 4;
                                break;
                        case 8: opcodes[dest_idx] = arg1 == arg2;
                                i += 4;
                                break;
                        default: fprintf(stderr,
                                         "Unknown opcode %d after %d operations.\n",
                                         op, ret);
                                 ret = -2;
                                 i = len;
                                 break;
                }
                ++ret;
        }
        return ret;
}

