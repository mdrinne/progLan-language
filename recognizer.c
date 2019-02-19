/*
 * Matthew Rinne
 * recognize.c
 * cs403 Programming Languages
 */


#include "lexer.h"
#include "types.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>


int
main(int argc, char const *argv[])
{
    FILE *fp = fopen(argv[argc-1], "r");
    getFile(fp);
    advance();
    program();
    match(END_OF_FILE);
    printf("legal\n");
    fclose(fp);
    return 0;
}
