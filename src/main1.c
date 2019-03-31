#include <stdio.h>
#include <stdlib.h>

#include "make.h"


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

    parse(f);

    fclose(f);

    return 0;

}
