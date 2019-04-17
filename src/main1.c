#include <stdio.h>
#include <stdlib.h>

#include "make.h"
#include "filedeptree.h"


int main(int argc, char *argv[])
{

    FILE *f;

    if (argc < 2) {
        fprintf(stderr, "Provide make file\n");
        exit(1);
    } if (argc > 2) {
        fprintf(stderr, "Provide only one file\n");
        exit(1);
    }

    f = fopen(argv[1], "r");

    fdt *deptree = parse(f);

    execTree(deptree, deptree->nfiles - 1);

    fclose(f);

    return 0;

}
