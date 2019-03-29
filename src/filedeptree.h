#ifndef FILEDEPTREE_H
#define FILEDEPTREE_H 0

struct filedependencytree {
    int maxfiles;
    int nfiles;
    char **files;
    int **depgraph;
};
typedef struct filedependencytree fdt;


fdt *fdt_create(int nfiles);
int fdt_addfile(fdt *deptree, const char *fname, char **depfiles);
int fdt_delete(fdt *deptree);
void fdt_getdependents(fdt *deptree, const char *fname, char **deps);
void fdt_print(fdt *deptree);
int fdt_getfileidbyname(fdt *deptree, const char *name);

#endif
