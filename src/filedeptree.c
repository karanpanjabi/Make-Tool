#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "filedeptree.h"


/*
struct filedependencytree {
    int maxfiles;
    int nfiles;
    char **files;
    int **depgraph;
};
typedef struct filedependencytree fdt;


fdt *fdt_create(int nfiles);
int fdt_addfile(fdt *deptree, const char *fname, int *deps);
int fdt_delete(fdt *deptree);
void fdt_getdependents(fdt *deptree, const char *fname, char **deps);
void fdt_print(fdt *deptree);
int fdt_getfileidbyname(fdt *deptree, const char *name)
*/


fdt *fdt_create(int nfiles)
{

    fdt *deptree;
    int i;

    if (nfiles <= 0)
        return NULL;

    deptree = (fdt *)malloc(sizeof(fdt));
    if (deptree == NULL) {
        return NULL;
    }

    deptree->maxfiles = nfiles;
    deptree->nfiles = 0;

    deptree->files = (char **)malloc(sizeof(char *) * nfiles);
    if (deptree->files == NULL) {
        free(deptree->files);
        free(deptree);
        return NULL;
    }

    deptree->cmds = (char **)malloc(sizeof(char *) * nfiles);
    if (deptree->files == NULL) {
        return NULL;
    }

    deptree->depgraph = (int **)malloc(sizeof(int *) * nfiles);
    if (deptree->depgraph == NULL) {
        free(deptree->cmds);
        free(deptree->files);
        free(deptree);
        return NULL;
    }

    for (i = 0; i < nfiles; i++) {
        deptree->depgraph[i] = (int *)malloc(sizeof(int) * nfiles);
        if (deptree->depgraph[i] == NULL) {
            while (--i > -1) {
                free(deptree->depgraph[i]);
            }
            free(deptree->cmds);
            free(deptree->files);
            free(deptree);
            return NULL;
        }
    }

    return deptree;

}


int fdt_addfile(fdt *deptree, const char *fname, const char *cmd, char **depfiles)
{

    int len, i;
    int *deps;
    char *temp;

    if (fname == NULL)
        return 4;

    if (depfiles == NULL)
        temp = NULL;
    else
        temp = depfiles[0];

    deps = (int *)malloc(sizeof(int) * deptree->maxfiles);
    if (deps == NULL)
        return 1;
    for (i = 0; i < deptree->maxfiles; i++) {
        deps[i] = 0;
    }

    i = 1;
    while (temp != NULL) {
        int x;
        x = fdt_getfileidbyname(deptree, temp);
        if (x == -1) {
            free(deps);
            return 2;
        }
        deps[x] = 1;
        temp = depfiles[i++];
    }


    if (deptree->nfiles == deptree->maxfiles) {
        free(deps);
        return 3;
    }

    len = sizeof(char) * (strlen(fname) + 1);
    deptree->files[deptree->nfiles] = (char *)malloc(len);
    if (deptree->files[deptree->nfiles] == NULL) {
        free(deps);
        return 1;
    }
    strncpy(deptree->files[deptree->nfiles], fname, len);

    if (cmd == NULL) {
        cmd = "\0";
    }
    len = sizeof(char) * (strlen(cmd) + 1);
    deptree->cmds[deptree->nfiles] = (char *)malloc(len);
    if (deptree->files[deptree->nfiles] == NULL) {
        free(deps);
        free(deptree->files[deptree->nfiles]);
        return 1;
    }
    strncpy(deptree->cmds[deptree->nfiles], cmd, len);

    for (i = 0; i < deptree->maxfiles; i++) {
        (deptree->depgraph[deptree->nfiles])[i] = deps[i];
    }

    (deptree->nfiles)++;

    free(deps);

    return 0;

}


int fdt_delete(fdt *deptree)
{

    int i;

    for (i = 0; i < deptree->nfiles; i++) {
        free(deptree->files[i]);
    }

    for (i = 0; i < deptree->maxfiles; i++) {
        free(deptree->depgraph[i]);
    }

    free(deptree->files);
    free(deptree->depgraph);
    free(deptree);

    return 0;

}


void fdt_print(fdt *deptree)
{

    int i, j;

    printf("------\nMax capacity = %d\n", deptree->maxfiles);

    if (deptree->nfiles == 0) {
        printf("Empty dependency tree\n");
        return ;
    }

    printf("Files:\n");
    for (i = 0; i < deptree->nfiles; i++) {
        printf("%d. %s\n", i + 1, deptree->files[i]);
        printf("    %s\n", deptree->cmds[i]);
    }

    printf("Dependencies:\n");
    for (i = 0; i < deptree->nfiles; i++) {
        printf("%s -> ", deptree->files[i]);
        for (j = 0; j < deptree->maxfiles; j++) {
            if ((deptree->depgraph[i])[j] == 1) {
                printf("%s; ", deptree->files[j]);
            }
        }
        printf("\n");
    }
    printf("------\n");
}


void fdt_getimmediatedependants(fdt *deptree, const char *fname, char **deps)
{

    int i;
    int file_id;
    int ct;

    file_id = -1;
    for (i = 0; i < deptree->nfiles; i++) {
        if (strcmp(deptree->files[i], fname) == 0) {
            file_id = i;
            break;
        }
    }

    if (file_id == -1) {
        deps[0] = NULL;
        return ;
    }

    ct = 0;
    for (i = 0; i < deptree->maxfiles; i++) {
        if ((deptree->depgraph[file_id])[i] == 1) {
            deps[ct] = deptree->files[i];
            ct++;
        }
    }

    deps[ct] = NULL;

}


int fdt_getfileidbyname(fdt *deptree, const char *name)
{

    int i;

    for (i = 0; i < deptree->nfiles; i++) {

        if (strcmp(deptree->files[i], name) == 0) {

            return i;

        }

    }

    return -1;

}


char *fdt_getfilecmd(fdt *deptree, const char *fname)
{

    int id;

    id = fdt_getfileidbyname(deptree, fname);

    if (id != -1)
        return deptree->cmds[id];
    else
        return NULL;

}

int getFileMTime(const char *fname)
{
    struct stat finfo;
    int exists = stat(fname, &finfo);

    // printf("%s %d %d\n", fname, finfo.st_size, finfo.st_mtime);

    if(exists == -1)
    {
        //File not found or doesn't exist
        printf("File %s not found\n", fname);
        return -1;
    }

    return finfo.st_mtime;
}


void execTree(fdt *deptree, int i)
{
    //compare with dependencies
    //visit all dependencies 
    char *fname = deptree->files[i];
    int mtime = getFileMTime(fname);
    // if(mtime == -1)
    // {
    //     return; //don't stop if src file isn't present
    // }


    for(int j=0; j < deptree->maxfiles; j++)
    {
        if ((deptree->depgraph[i])[j] == 1)
        {
            execTree(deptree, j);
        }
    }

    //execute cmd for the file, if any of dependencies' timestamp is greater than current node's

    int isGreater = 0;
    for(int j=0; j < deptree->maxfiles; j++)
    {
        if ((deptree->depgraph[i])[j] == 1 && getFileMTime(deptree->files[j]) > mtime)
        {
            isGreater = 1;
            break;
        }
    }

    if(isGreater)
    {
        printf("Executing for %s : %s\n", deptree->files[i], deptree->cmds[i]);
        system(deptree->cmds[i]);
    }
}