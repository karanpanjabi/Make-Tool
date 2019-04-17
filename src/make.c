#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "filedeptree.h"


struct file_t {
    char *fname;
    char **deps;
    char *cmd;
};
typedef struct file_t file;


fdt * parse(FILE *input)
{

    fdt *deptree;
    file f;
    char c;
    int state;
    char buffer[1024];
    int buf_ind;
    int line_no;
    int dep_no;
    int len;

    deptree = fdt_create(32);

    /*
    state 0: reading filename, read 0 characters
    state 1: reading filename, read at least 1 character
    state 2: reading dependency filename, read 0 characters
    state 3: reading dependency filename, read at least 1 character
    state 4: reading compile command
    */

    buf_ind = 0;
    state = 0;
    line_no = 1;
    dep_no = 0;
    while ((c = fgetc(input)) != EOF) {

        switch (state) {

            case 0:
                if (c != '\n' && c != ' ' && c != '\t') {
                    buffer[buf_ind++] = (char)c;
                    printf("reading new filename\n");
                    state = 1;
                }
                break;
            case 1:
                if (c == ':') {
                    buffer[buf_ind++] = '\0';
                    printf("finished reading %s\n", buffer);
                    state = 2;
                    len = buf_ind;
                    f.fname = (char *)malloc(sizeof(char) * len);
                    f.deps = (char **)malloc(sizeof(char *) * deptree->nfiles);
                    dep_no = 0;
                    strncpy(f.fname, buffer, len);
                    buf_ind = 0;
                } else {
                    if (c == '\n') {
                        fprintf(stderr, "error on line %d: unexpected newline\n", line_no);
                        fdt_delete(deptree);
                    }
                    buffer[buf_ind++] = (char)c;
                }
                break;
            case 2:
                if (c == '\n') {
                    printf("no dependencied, reading command\n");
                    state = 4;
                } else if (c != ' ' && c != '\t') {
                    printf("reading dependency %d\n", dep_no);
                    buffer[buf_ind++] = (char)c;
                    state = 3;
                }
                break;
            case 3:
                if (c == ' ' || c == '\t') {
                    buffer[buf_ind++] = '\0';
                    printf("finished reading dependency %d = %s\n", dep_no, buffer);
                    state = 2;
                    len = buf_ind;
                    f.deps[dep_no] = (char *)malloc(sizeof(char) * len);
                    strncpy(f.deps[dep_no], buffer, len);
                    buf_ind = 0;
                    dep_no++;
                } else if (c == '\n') {
                    buffer[buf_ind++] = '\0';
                    printf("finished reading dependency %d = %s\n", dep_no, buffer);
                    state = 4;
                    len = buf_ind;
                    f.deps[dep_no] = (char *)malloc(sizeof(char) * len);
                    strncpy(f.deps[dep_no], buffer, len);
                    buf_ind = 0;
                    dep_no++;
                } else {
                    buffer[buf_ind++] = (char)c;
                }
                break;
            case 4:
                if (c == '\n') {
                    int r;
                    int i;
                    buffer[buf_ind++] = '\0';
                    state = 0;
                    len = buf_ind;
                    f.cmd = (char *)malloc(sizeof(char) * len);
                    strncpy(f.cmd, buffer, len);
                    buf_ind = 0;
                    f.deps[dep_no] = NULL;
                    r = fdt_addfile(deptree, f.fname, f.cmd, f.deps);
                    if (r == 0) {
                        printf("successfully added:\n%s\ncmd:%s\n", f.fname, f.cmd);
                    }
                    if (r == 1) {
                        fprintf(stderr, "MemoryError\n");
                    } else if (r == 2) {
                        fdt_print(deptree);
                        fprintf(stderr, "error on line %d: unknown filename\n", line_no);
                    } else if (r == 3) {
                        fprintf(stderr, "error on line %d: FDT_OverflowError\n", line_no);
                    } else if (r == 4) {
                        fprintf(stderr, "error on line %d: no filename given\n", line_no);
                    }
                    if (r != 0) {
                        exit(1);
                    }
                    for (i = 0; i < dep_no; i++) {
                        free(f.deps[i]);
                    }
                    free(f.fname);
                    free(f.deps);
                    free(f.cmd);
                } else {
                    buffer[buf_ind++] = (char)c;
                }
                break;
            default:
                fprintf(stderr, "ParserError: case %d not handled\n", state);
                fdt_delete(deptree);
                exit(1);

        }

        if (c == '\n') {
            line_no++;
        }

    }

    if (state != 0) {
        fprintf(stderr, "error on line %d: unexpected EOF\n", line_no);
    }

    fdt_print(deptree);

    return deptree;
}
