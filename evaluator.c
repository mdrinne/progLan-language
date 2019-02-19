/*
 * Matthew Rinne
 * evaluator.c
 * cs403 Programming Languages
 */


#include "evaluator.h"
#include "environment.h"
#include "lexeme.h"
#include "types.h"
#include <stdio.h>
#include <stdlib.h>


Lexeme *
eval(Lexeme *tree, Lexeme *env)
{
    if (getLexemeType(tree) == INTEGER) return tree;
    else if (getLexemeType(tree) == REAL) return tree;
    else if (getLexemeType(tree) == STRING) return tree;
    else if (getLexemeType(tree) == ID) return lookup(getLexemeID(tree),env);
    // else if (getLexemeType(tree) ==)
}