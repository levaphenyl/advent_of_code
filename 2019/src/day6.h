#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/* idlen = 4 because 3 characters + \0 */
enum { max_sats = 4, idlen = 4, };


/* tsearch from the GNU libc builds balanced binary trees.
 * In this exercise, we don't want the COM, i.e. the root of the tree, to change.
 * Hence a new tree structure called `planet`. */
typedef struct planet planet;
struct planet {
        char id[idlen];
        planet* sats[max_sats];
        unsigned sat_idx;
};

/**
 * @brief Append a planet to the satellite list of another.
 * The new satellite object is malloc'ed.
 * It is the responsibility of the calling function to free the satellites.
 *
 * @param p the planet at the center or the orbit.
 * @param new_sat the new satellite planet.
 *
 * @return 1 if the satellite was added to the list, 0 if the list or the memory is full.
 */
static _Bool append_sat(planet* p, planet* new_sat)
{
        _Bool ret = 0;
        /* p->field is a shortcut for *(p).field */
        if (p->sat_idx < max_sats - 1) {
                p->sats[p->sat_idx] = malloc(sizeof(planet));
                if (p->sats[p->sat_idx]) {
                        memcpy(p->sats[p->sat_idx], new_sat, sizeof(planet));
                        ++p->sat_idx;
                        ret = 1;
                }
        }
        return ret;
}

/**
 * @brief Check if the planet ID exists among the existing satellites.
 *
 * @param p an existing planet object, which satellites are to check.
 * @param id the ID to look for.
 *
 * @return 1 if the ID was found among the satellites, otherwise 0.
 */
_Bool isinsats(planet* p, char const id[idlen])
{
        _Bool ret = 0;
        for (size_t i = 0; i < p->sat_idx; ++i) {
                if (!strcmp(p->sats[i]->id, id)) {
                        ret = 1;
                        break;
                }
        }
        return ret;
}

/**
 * @brief Find the center of the orbit for the given satellite name.
 *
 * @param p the root of the planet system.
 * @param id the name of the satellite, this string will be changed to the name
 * of the center if found.
 * @param depth recursive accumulator, should be 0 when calling the function.
 *
 * @return the depth of the center in the planet system.
 */
unsigned find_center(planet* p, char id[idlen], unsigned depth)
{
        unsigned ret = 0;
        if (isinsats(p, id)) {
                strcpy(id, p->id);
                ret = depth;
        } else {
                for (size_t i = 0; i < p->sat_idx; ++i) {
                        ret = find_center(p->sats[i], id, depth + 1);
                        if (ret) {
                                break;
                        }
                }
        }
        return ret;
}

/**
 * @brief Add a planet to a system if the center of its orbit is known.
 * The planet object will be created and malloc'ed by this function.
 * It is the responsibility of the calling function to free the satellites.
 * This function proceeds recursively.
 *
 * @param sys_root pointer to the planet system.
 * @param orbit_center name of the orbit center of the new planet.
 * @param new_id name of the new planet.
 *
 * @return 1 if the @a orbit_center was found in @a sys_root and the new * satellite was added, otherwise 0.
 */
_Bool add_planet(planet* sys_root, char const orbit_center[idlen], char const new_id[idlen])
{
        _Bool ret = 0;
        if (!strcmp(sys_root->id, orbit_center)) {
                if (!isinsats(sys_root, new_id)) {
                        planet new_sat = {
                                .id = { 0 },
                                .sats = { 0 },
                                .sat_idx = 0,
                        };
                        strcpy(new_sat.id, new_id);
                        ret = append_sat(sys_root, &new_sat);
                }
        } else {
                for (size_t i = 0; i < sys_root->sat_idx; ++i) {
                        ret = add_planet(sys_root->sats[i], orbit_center, new_id);
                        if (ret) {
                                break;
                        }
                }
        }
        return ret;
}

/**
 * @brief Recursively print the planet system.
 *
 * @param sys_root pointer to the planet system.
 * @param depth for formating only, should be always 0 when calling the function.
 */
void print_planets(planet* sys_root, int depth)
{
        printf("%s", sys_root->id);
        if (sys_root->sat_idx) {
                printf("\t");
                for (size_t i = 0; i < sys_root->sat_idx; ++i) {
                        printf("%.*s", depth, "\t");
                        print_planets(sys_root->sats[i], depth + 1);
                }
        } else {
                printf("\n");
        }
}

/**
 * @brief Recursively free malloc'ed satellites.
 *
 * @param sys_root pointer to the planet system.
 *
 * @return number of freed satellites.
 */
unsigned destroy_planets(planet* sys_root)
{
        unsigned ret = 0;
        if (sys_root->sat_idx) {
                for (size_t i = 0; i < sys_root->sat_idx; ++i) {
                        ret += destroy_planets(sys_root->sats[i]);
                }
        } else {
                free(sys_root);
                ++ret;
        }
        return ret;
}


/**
 * @brief Parse orbit file and add planets to the system root.
 * Planets are added to the system in the order of the file.
 * If the orbit center does not exist yet for a given pair, the satellite won't be added.
 * Planet objects will be created and malloc'ed by this function.
 * It is the responsibility of the calling function to free the satellites.
 *
 * @param instream open file descriptor.
 * @param sys_root pointer to the planet system.
 *
 * @return the number of added planets.
 */
static unsigned parse_orbits(FILE* instream, planet* sys_root)
{
        unsigned ret = 0;
        int scanerr = 2;
        /* Make the parsing dependent on the idlen identifier */
        char* const metafmt = "%%%ds)%%%ds";
        char fmt[8] = { 0 };
        sprintf(fmt, metafmt, idlen-1, idlen-1);
        char center[idlen] = { 0 };
        char satellite[idlen] = { 0 };
        while (scanerr == 2) {
                scanerr = fscanf(instream, fmt, center, satellite);
                if (scanerr == EOF) {
                        break;
                } else {
                        ret += add_planet(sys_root, center, satellite);
                }
        }
        if (scanerr != EOF) {
                /* unexpected error */
                fprintf(stderr, "parse_orbits: error code %d\n", scanerr);
                perror(0);
                errno = 0;
        }
        return ret;
}

/**
 * @brief Read and build a planet system from an orbit file.
 * This function deals with unsorted orbit files by doing several passes over the file.
 * Planet objects will be created and malloc'ed by this function.
 * It is the responsibility of the calling function to free the satellites.
 *
 * @param path path to the orbit file.
 * @param sys_root planet object containing the center of mass.
 *
 * @return the number of planets added.
 */
unsigned read_planet_system(char* path, planet* sys_root)
{
        unsigned ret = 0;
        unsigned dret = 1;
        FILE* orbitfile = fopen(path, "r");
        if (orbitfile) {
                while (dret) {
                        dret = parse_orbits(orbitfile, sys_root);
                        ret += dret;
                        rewind(orbitfile);
                }
                fclose(orbitfile);
        } else {
                fprintf(stderr, "read_planet_system: failed to open file %s\n", path);
                perror(0);
                errno = 0;
        }
        return ret;
}


/**
 * @brief Compute the orbit checksum for the given planet system.
 *
 * @param sys_root the planet object at the center of the planet system.
 * @param depth accumulator variable, should be 0 when calling the function.
 *
 * @return the checksum as the sum of direct and indirect orbits.
 */
unsigned orbit_checksum(planet* sys_root, unsigned depth)
{
        unsigned ret = depth;
        if (sys_root->sat_idx) {
                for (size_t i = 0; i < sys_root->sat_idx; ++i) {
                        ret += orbit_checksum(sys_root->sats[i], depth + 1);
                }
        }
        return ret;
}


/**
 * @brief Compute the minimum transfer length from the start satellite to the target satellite.
 *
 * @param sys_root the planet object at the center of the planet system.
 * @param start the satellite name at the beginning of the transfer.
 * @param target the name of the target satellite.
 *
 * @return the number of orbital transfers.
 */
int transfer_length(planet* sys_root, char const start[idlen], char const target[idlen])
{
        int ret = -2;
        char curr_st[idlen];
        strcpy(curr_st, start);
        char curr_tg[idlen];
        strcpy(curr_tg, target);
        unsigned st_depth = 0;
        unsigned tg_depth = 0;
        while (strcmp(curr_st, curr_tg)) {
                if (st_depth > tg_depth) {
                        st_depth = find_center(sys_root, curr_st, 0);
                        ++ret;
                } else if (tg_depth > st_depth) {
                        tg_depth = find_center(sys_root, curr_tg, 0);
                        ++ret;
                } else {
                        st_depth = find_center(sys_root, curr_st, 0);
                        tg_depth = find_center(sys_root, curr_tg, 0);
                        ret += 2;
                }
                if (!st_depth && !tg_depth) {
                        fprintf(stderr, "transfer_length: planets %s and %s were not found.\n",
                                curr_st, curr_tg);
                        break;
                }
        }
        return ret;
}

