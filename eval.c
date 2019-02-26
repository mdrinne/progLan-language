/*
 * Matthew Rinne
 * eval.c
 * cs403 Programming Languages
 */


#include "lexeme.h"
#include "lexer.h"
#include "types.h"
#include "parser.h"
#include "evaluator.h"
#include "environment.h"
#include "pretty-printing.h"
#include <stdio.h>
#include <stdlib.h>


int 
main(int argc, char *argv[])
{
    FILE *fp = fopen(argv[1], "r");
    getFile(fp);
    advance();
    Lexeme *tree = program();
    match(END_OF_FILE);
    Lexeme *env = newEnv();
    // pp(tree);
    setCLvars(argc,argv);
    eval(tree,env);
    // displayEnv(env);
    fclose(fp);
    return 0;
}
