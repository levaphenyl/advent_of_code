/** Work hypothesis:
 *  - the horizontal axis (left-right) is denoted \f$x\f$ and is in postion 0;
 *  - the vertical axis (up-down) is denoted \f$y\f$ and is in postion 1;
 *  - the central point is considered to be the origin of the 2D space with coordinates (0, 0).
**/
#include <ctype.h>
#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
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

/** @brief Bidimensional segment
 *  If the segment is defined between point A \f$(x_A, y_A)\f$ and B \f$(x_B, y_B)\f$,
 *  then \p start_pt corresponds to A and \p vect equals \f$(x_B - x_A, y_B - y_A)\f$.
**/
struct segment
{
  float start_pt[2];
  float vect[2];
};
typedef struct segment segment;

/** @brief Compute the cross product of two vectors
 *  This function assumes a 2-D carthesian space.
 *  Formula:
 *    \f$\vect{A}\times \vect{B} = (x_A, y_A)\times (x_B, y_B)\f$
 *    \f$                        = x_A*y_B - y_A*x_B\f$
**/
float crossprod(float vectA[2], float vectB[2])
{
        return vectA[0]*vectB[1] - vectA[1]*vectB[0];
}

/** @brief Compute the Manhattan distance between two points
 * This function assumes a 2D-cartesian reference.
 * @param ptA[2] the (x, y) coordinates of the first point
 * @param ptB[2] the (x, y) coordinates of the second point
 *
 * @return \f$d(A, B) = | x_B - x_A | + | y_B - y_A |\f$
 */
float manhattan_dist(float ptA[2], float ptB[2])
{
        return fabsf(ptB[0] - ptA[0]) + fabsf(ptB[1] - ptA[1]);
}

/** @brief Compute the intersection point between 2 segments if it exists.
 * This function assumes a 2-D cartesian space.
 * The order of segments does not matter.
 * @sa https://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect/565282
 * Intersection equation:
 * @verbatim
 * seg1.start_pt + param1 * seg1.vect = seg2.start_pt + param2 * seg2.vect
 * @endverbatim
 *
 * @param seg1 a segment
 * @param seg2 another segment
 * @param point[2] resulting intersection point, will be modified if the intersection exists.
 * @return 1 if there is an intersection, otherwise 0.
 */
int seg_intersect(segment seg1, segment seg2, float point[2])
{
        int res = 0;
        float err = 1e-6;
        float denom = crossprod(seg2.vect, seg1.vect);
        if (fabsf(denom) > err) {
                float ptdiff[2] = { seg1.start_pt[0] - seg2.start_pt[0],
                                    seg1.start_pt[1] - seg2.start_pt[1], };
                float param1 = crossprod(ptdiff, seg2.vect) / denom;
                float param2 = crossprod(ptdiff, seg1.vect) / denom;
                if (param1 >= 0 && param1 <= 1 && param2 >= 0 && param2 <= 1) {
                    res = 1;
                    point[0] = seg1.start_pt[0] + param1*seg1.vect[0];
                    point[1] = seg1.start_pt[1] + param1*seg1.vect[1];
                } /* else intersection outside the segments */
        } /* else denom == 0 and the segments are parallel */
        return res;
}

/** @brief Count the width of a CSV line from a file descriptor
 *  This function assumes that the file is encoded in ASCII.
 *  It also assumes the absence of trailing separator.
 *  This function intends to scan the file in a first pass to know the exact number of values.
 *  The return values gives the size of the buffer that is needed to parse the line.
 *  @param instream the file descriptor
 *  @param sep the separator between the values
 *  @return the number of separators found + 1
**/
unsigned cnt_csv_width(FILE* instream, char const sep)
{
        unsigned sum = 0;
        char buffer[buf_max] = { 0 };
        if (instream) {
                while (fgets(buffer, buf_max, instream)) {
                        for (size_t i = 0; i < buf_max; ++i) {
                                char curr = buffer[i];
                                if (curr == sep) {
                                        ++sum;
                                } else if ((i < buf_max -1 && curr == '\0')
                                           || curr == '\n') {
                                        goto EXIT;
                                }
                        }
                }
        }
EXIT:
        if (sum > 0) {
                /* If we found at least a separator,
                   then there are at least two values. */
                ++sum;
        }
        return sum;
}

/** @brief Parse a wire path from a file.
 * This function assumes the path is comma separated with the U, D, L, R directional keywords,
 * e.g. U7,R6,D4,L4.
 * This function assumes that the file is encoded in ASCII.
 * This function assumes the maximal move amplitude to be 99999.
 * Parsing stops when encountering a newline character '\n'.
 *
 * @param instream file descriptor
 * @param len length of the path
 * @param segbuff array to store the path as segments
 *
 * @return the number of segments found
 */
size_t wire_rx(FILE* instream, size_t const len, segment segbuff[static len])
{
        size_t ret = 0;
        int max_step = 8; // max step "U99999,\0"
        int rx_len = max_step * len;
        char rxbuff[rx_len];
        memset(rxbuff, 0, rx_len*sizeof(char));
        char sep = ',';
        char curr;
        char dir;
        float norm = 0;
        segment lastseg = { 0 };
        if (instream && fgets(rxbuff, rx_len, instream)) {
                for (size_t i = 0; i < rx_len; ++i) {
                        curr = rxbuff[i];
                        if isalpha(curr) {
                            dir = curr;
                        } else if (curr == sep || curr == '\n' || curr == '\0') {
                                switch (dir) {
                                        case 'U': lastseg.vect[1] = norm;
                                                  break;
                                        case 'D': lastseg.vect[1] = -1*norm;
                                                  break;
                                        case 'R': lastseg.vect[0] = norm;
                                                  break;
                                        case 'L': lastseg.vect[0] = -1*norm;
                                }
                                segbuff[ret] = lastseg;
                                ++ret;
                                norm = 0;
                                lastseg.start_pt[0] = lastseg.start_pt[0] + lastseg.vect[0];
                                lastseg.start_pt[1] = lastseg.start_pt[1] + lastseg.vect[1];
                                lastseg.vect[0] = 0;
                                lastseg.vect[1] = 0;
                                if (curr == '\n' || curr == '\0') {
                                    break;
                                }
                        } else {
                            norm = 10*norm + strtof(&curr, 0);
                        }
                }
        }
        return ret;
}

/* In the particular case of the exercise,
 * the vectors are always parallel to axis.
 * Thus, the norm is the non null vector value.
 * Thus, it is the maximum of the absolute value of each coordinate. */
float norm2d(float x, float y)
{
        return fmaxf(fabsf(x), fabsf(y));
}


int main(void)
{
        int ret = 1;
        //char* csvpath = "2019/test/data/input-day3-example.txt";
        char* csvpath = "2019/data/input-day3.txt";
        FILE* csvfile = fopen(csvpath, "r");
        if (csvfile) {
                unsigned ln1 = cnt_csv_width(csvfile, ',');
                unsigned ln2 = cnt_csv_width(csvfile, ',');
                rewind(csvfile);
                segment wires1[ln1];
                segment wires2[ln2];
                size_t n_wire1 = wire_rx(csvfile, ln1, wires1);
                size_t n_wire2 = wire_rx(csvfile, ln2, wires2);
                fclose(csvfile);
                if (n_wire1 && n_wire2) {
                        fprintf(stdout, "Length of the wires: %zu, %zu\n",
                                n_wire1, n_wire2);
                        float origin[2] = { 0 };
                        float crosspt[2] = { 0 };
                        /* Part 1 */
                        float dist = 0;
                        float min_dist = 1e6;
                        for (size_t i = 0; i < n_wire1; ++i) {
                                for (size_t j = 0; j < n_wire2; ++j) {
                                        if (seg_intersect(wires1[i], wires2[j], crosspt)) {
                                                dist = manhattan_dist(origin, crosspt);
                                                if (dist < min_dist && dist > 0) {
                                                        min_dist = dist;
                                                }
                                        }
                                }
                        }
                        fprintf(stdout, "Answer for part 1 is: %f\n", min_dist);
                        /* Part 2 */
                        float steps1 = 0;
                        float steps2 = 0;
                        float stepsum = 0;
                        float min_steps = 1e6;
                        for (size_t i = 0; i < n_wire1; i++) {
                                steps2 = 0;
                                for (size_t j = 0; j < n_wire2; ++j) {
                                        if (seg_intersect(wires1[i], wires2[j], crosspt)){
                                                stepsum = steps1 + steps2;
                                                stepsum += norm2d(
                                                        crosspt[0] - wires1[i].start_pt[0],
                                                        crosspt[1] - wires1[i].start_pt[1]);
                                                stepsum += norm2d(
                                                        crosspt[0] - wires2[j].start_pt[0],
                                                        crosspt[1] - wires2[j].start_pt[1]);
                                                if (stepsum < min_steps && stepsum > 0) {
                                                        min_steps = stepsum;
                                                }
                                        }
                                        steps2 += norm2d(
                                                wires2[j].vect[0],
                                                wires2[j].vect[1]);
                                }
                                steps1 += norm2d(
                                        wires1[i].vect[0],
                                        wires1[i].vect[1]);
                        }
                        fprintf(stdout, "Answer for part 2 is: %f\n", min_steps);
                        ret = 0;
                } else {
                        fprintf(stderr,
                                "Only one wire found: wire 1 (%zu), wire 2 (%zu)\n",
                                n_wire1, n_wire2);
                        ret = 2;
                }
        } else {
                fopenerr(csvpath);
        }
        return ret;
}

