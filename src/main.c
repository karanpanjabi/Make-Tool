#include <stdio.h>
#include <stdlib.h>

#include "filedeptree.h"

#define MAXFILES 3


int main(void)
{

    fdt *dt;
    int r;
    int i;
    char *s = "filedeptree.h";
    char *f[4];

    f[0] = s;
    f[1] = NULL;

    // test 1
    dt = fdt_create(MAXFILES);
    if (dt == NULL) {
        printf("Create returned NULL\n");
        exit(1);
    }

    fdt_print(dt);

    // test 2
    r = fdt_addfile(dt, s, NULL);
    if (r != 0) {
        printf("%d: did not add file successfully\n", r);
    }

    fdt_print(dt);

    // test 3
    r = fdt_addfile(dt, "main.c", f);
    if (r != 0) {
        printf("%d: did not add file successfully\n", r);
    }

    fdt_print(dt);

    // test 4
    f[1] = "main.c";
    f[2] = NULL;

    r = fdt_addfile(dt, "filedeptree.c", f);
    if (r != 0) {
        printf("%d: did not add file successfully\n", r);
    }

    fdt_print(dt);

    // test 5: check for deptree full
    f[2] = "filedeptree.c";
    f[3] = NULL;

    r = fdt_addfile(dt, "someotherefile.c", f);
    if (r != 0) {
        printf("%d: did not add file successfully\n", r);
    }

    fdt_print(dt);

    //test 6: test fdt_getimmediatedependants
    fdt_getimmediatedependants(dt, "filedeptree.c", f);
    printf("dependants of %s are:\n", "filedeptree.c");
    i = 0;
    while (f[i] != NULL) {
        printf("%s\n", f[i]);
        i++;
    }

    r = fdt_delete(dt);

    return 0;

}
