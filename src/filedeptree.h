#ifndef FILEDEPTREE_H
#define FILEDEPTREE_H 0

struct filedependencytree {
    int maxfiles;
    int nfiles;
    char **files;
    char **cmds;
    int **depgraph;
};
typedef struct filedependencytree fdt;


fdt *fdt_create(int nfiles);
int fdt_addfile(fdt *deptree, const char *fname, const char *cmd,char **depfiles);
int fdt_delete(fdt *deptree);
void fdt_getimmediatedependants(fdt *deptree, const char *fname, char **deps);
void fdt_print(fdt *deptree);
int fdt_getfileidbyname(fdt *deptree, const char *name);
char *fdt_getfilecmd(fdt *deptree, const char *fname);

#endif
