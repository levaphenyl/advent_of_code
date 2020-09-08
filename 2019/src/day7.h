#include <stdlib.h>
#include "day5.h"

enum { n_lvl = 5, };


/** @brief Execute the Intcode program, expecting only one input and one output.
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
 *  @param inlen length of the inputs vector.
 *  @param invals values to give as input.
 *  @param len length of the opcode array.
 *  @param opcodes intcode program as array of opcodes, this array is modified by the function!
 *  @return the program output, or 0 if the program succeeded without output, or -1 if an error occured.
**/
int opcode_exec_in_out(size_t const inlen, int const invals[static inlen],
                       size_t const len, int opcodes[static len])
{
        int ret = 0;
        size_t i = 0;
        size_t dest_idx;
        size_t inidx = 0;
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
                                if (inidx < inlen) {
                                        opcodes[dest_idx] = invals[inidx];
                                        i += 2;
                                } else {
                                        fprintf(stderr,
                                                "opcode_exec: input index out of range.\n");
                                        ret = -1;
                                        i = len;
                                }
                                ++inidx;
                                break;
                        case 4: ret = arg1;
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
                                 ret = -1;
                                 i = len;
                                 break;
                }
        }
        return ret;
}


/**
 * @brief Execute the Intcode of an thruster amplifier.
 * The Intcode is copied for the execution, thus the original is not modified.
 *
 * @param inlen length of the input vector.
 * @param amp_ins input vector for the Intcode.
 * @param len Intcode length.
 * @param opcodes thruster Intcode.
 *
 * @return the amplifier output as the Intcode print value.
 */
int amp_exec(size_t const inlen, int const amp_ins[static inlen],
             size_t const len, int const opcodes[static len])
{
        int amp_out = 0;
        int* opcpy = malloc(len * sizeof(int));
        if (opcpy) {
                memcpy(opcpy, opcodes, len * sizeof(int));
                amp_out = opcode_exec_in_out(inlen, amp_ins, len, opcpy);
                free(opcpy);
        }
        return amp_out;
}


/**
 * @brief Swap to values in an array.
 * Useful to compute permutations of a set of integers.
 *
 * @param len   length of the array.
 * @param arr   original array. This variable will be modified!
 * @param i     index of the first value to be swapped.
 * @param j     index of the second value to be swapped.
 *
 * @return  True if the two values were swapped, false if one of the indices is
 * out of bound.
 */
_Bool perm_swap(size_t const len, int arr[static len], int i, int j)
{
        _Bool ret = 0;
        if (i <= len && j <= len) {
                int tmp = arr[i];
                arr[i] = arr[j];
                arr[j] = tmp;
                ret = 1;
        }
        return ret;
}


/**
 * @brief Compute the power of an integer.
 * From: https://stackoverflow.com/a/29787467/8289769
 *
 * @param base  number to be powered.
 * @param exp   exponent.
 *
 * @return  base^(exp).
 */
int powi(int base, int exp)
{
        int res = 1;
        while (exp) {
                if (exp % 2) {
                    res *= base;
                }
                exp /= 2;
                base *= base;
        }
        return res;
}

/**
 * @brief Search for the maximal amplifier signal for thrusters.
 *
 * @param len           length of the Intcode.
 * @param opcodes[len]  Intcode. The Intcode is preserved by this function
 * through copy mechanism.
 *
 * @return  the maximal Intcode output for all permutations of settings.
 */
int max_amp_signal(size_t const len, int const opcodes[len])
{
        int ret = 0;
        int argmax = 0;
        int tmp_out = 0;
        int tmp_in[2] = { 0 };
        int settings[n_lvl];
        for (size_t i = 0; i < n_lvl; ++i) settings[i] = i;
        /* Generate permutations */
        for (size_t l = n_lvl-1; l < n_lvl; --l) {
                for (size_t i = 0; i < n_lvl; ++i) {
                        perm_swap(n_lvl, settings, l, i);
                        for (size_t j = i; j < n_lvl; ++j) {
                                perm_swap(n_lvl, settings, i, j);
                                /* Use permutation here */
                                tmp_out = 0;
                                for (size_t k = 0; k < n_lvl; ++k) {
                                        tmp_in[0] = settings[k];
                                        tmp_in[1] = tmp_out;
                                        tmp_out = amp_exec(2, tmp_in, len, opcodes);
                                }
                                if (tmp_out > ret) {
                                        ret = tmp_out;
                                        argmax = 0;
                                        for (size_t k = 0; k < n_lvl; ++k) {
                                                int p = powi(10, n_lvl - k - 1);
                                                argmax += settings[k] * p;
                                        }
                                }
                        }
                        perm_swap(n_lvl, settings, l, i);
                }
        }
        printf("max_amp_signal: argmax = %05d\n", argmax);
        return ret;
}

