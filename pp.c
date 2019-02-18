/*
 * Matthew Rinne
 * pp.c
 * cs403 Programming Languages
 */


#include "lexeme.h"
#include "lexer.h"
#include "types.h"
#include "parser.h"
#include "pretty-printing.h"
#include <stdio.h>
#include <stdlib.h>


int 
main(int argc, char const *argv[])
{
    FILE *fp = fopen(argv[argc-1], "r");
    getFile(fp);
    advance();
    Lexeme *tree = program();
    match(END_OF_FILE);
    pp(tree);
    fclose(fp);
    return 0;
}
