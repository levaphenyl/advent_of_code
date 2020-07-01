#define _GNU_SOURCE     /* Expose declaration of tdestroy() */
#include <search.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

static void *root = NULL;

static void * xmalloc(unsigned n)
{
    void *p;
    p = malloc(n);
    if (p)
        return p;
    fprintf(stderr, "insufficient memory\n");
    exit(EXIT_FAILURE);
}

static int compare(const void *pa, const void *pb)
{
    int ret = 0;
    if (*(int *) pa < *(int *) pb)
        ret = -1;
    if (*(int *) pa > *(int *) pb)
        ret = 1;
    printf("compare (%d, %d) => %d\n", *(int*)pa, *(int*)pb, ret);
    return ret;
}

static void action(const void *nodep, VISIT which, int depth)
{
    int *datap;

    switch (which) {
    case preorder:
        break;
    case postorder:
        datap = *(int **) nodep;
        printf("%*s%6d\n", depth*6, "", *datap);
        break;
    case endorder:
        break;
    case leaf:
        datap = *(int **) nodep;
        printf("%*s%6d.\n", depth*6, "", *datap);
        //printf("%6d\n", *datap);
        break;
    }
}

int main(void)
{
    int i, *ptr;
    void *val;

    srand(time(NULL));
    for (i = 0; i < 12; i++) {
        ptr = xmalloc(sizeof(int));
        *ptr = rand() & 0xff;
        printf("\nStep %d: Insert --> %d\n", i, *ptr);
        val = tsearch((void *) ptr, &root, compare);
        twalk(root, action);
        if (val == NULL)
            exit(EXIT_FAILURE);
        else if ((*(int **) val) != ptr)
            free(ptr);
    }
    puts("\nFinal tree\n");
    twalk(root, action);
    tdestroy(root, free);
    exit(EXIT_SUCCESS);
}
