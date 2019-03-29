#include <stdio.h>
#include <stdlib.h>

#include "filedeptree.h"


int main(void)
{

    fdt *dt;
    int r;
    char *s = "filedeptree.h";
    char *f[2];

    f[0] = s;
    f[1] = NULL;

    dt = fdt_create(10);
    if (dt == NULL) {
        printf("Create returned NULL\n");
        exit(1);
    }

    fdt_print(dt);

    r = fdt_addfile(dt, s, NULL);
    if (r != 0) {
        printf("%d: did not add file successfully\n", r);
    }

    fdt_print(dt);

    r = fdt_addfile(dt, "main.c", f);
    if (r != 0) {
        printf("%d: did not add file successfully\n", r);
    }

    fdt_print(dt);

    r = fdt_delete(dt);

    return 0;

}