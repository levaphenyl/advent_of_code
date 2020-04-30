#include <stdlib.h>

enum { pwd_sz = 6, };


unsigned upow(unsigned base, unsigned exp)
{
        unsigned res = 1;
        while(exp) {
                /* Use bitwise AND operator to test if exp is odd */
                if (exp & 1) {
                    res *= base;
                }
                /* right bit shift = divide by 2 */
                exp = exp >> 1;
                base *= base;
        }
        return res;
}

/**
 * @brief Decompose a natural integer in an array of single integers.
 * The array is ordered from the higher power of 10 to the lower.
 *
 * @param num the integer to decompose.
 * @param array the array to fill with the numbers composing @a num in base 10.
 */
void uint2arr(unsigned num, unsigned arr[pwd_sz])
{
        unsigned rest = num;
        unsigned unit;
        for (unsigned i = pwd_sz; i > 0; --i) {
                unit = upow(10, i - 1);
                arr[pwd_sz-i] = rest / unit;
                rest -= arr[pwd_sz-i] * unit;
        }
}

/**
 * @brief Test the compliance of the password for Part 1.
 *
 * @param pwd[pwd_sz] a password as a decomposition of an integer in base 10.
 *
 * @return 1 if the password is compliant, 0 otherwise.
 */
_Bool is_compliant(unsigned pwd[pwd_sz])
{
        _Bool res = 1;
        _Bool pair = 0;
        for (size_t i = 1; i < pwd_sz; ++i) {
                if (pwd[i] < pwd[i-1]) {
                        res = 0;
                        break;
                }
                if (pwd[i] == pwd[i-1]) {
                        pair = 1;
                }
        }
        return res && pair;
}

/**
 * @brief Test the compliance of the password for Part 2.
 *
 * @param pwd[pwd_sz] a password as a decomposition of an integer in base 10.
 *
 * @return 1 if the password is compliant, 0 otherwise.
 */
_Bool is_compliant2(unsigned pwd[pwd_sz])
{
        _Bool res = 1;
        unsigned min_rep = 1e6;
        unsigned tmp_rep = 0;
        unsigned last;
        for (size_t i = 1; i < pwd_sz; ++i) {
                if (pwd[i] < pwd[i-1]) {
                        res = 0;
                        break;
                }
                if (pwd[i] == pwd[i-1]) {
                        if (tmp_rep) {
                                if (pwd[i] == last) {
                                        ++tmp_rep;
                                } else {
                                    if (tmp_rep < min_rep) {
                                            min_rep = tmp_rep;
                                    }
                                    tmp_rep = 1;
                                    last = pwd[i];
                                }
                        } else {
                                last = pwd[i];
                                ++tmp_rep;
                        }
                }
        }
        if (tmp_rep < min_rep) {
                min_rep = tmp_rep;
        }
        return res && (min_rep == 1);
}

