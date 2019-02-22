/*
 * Matthew Rinne
 * lexeme.c
 * cs403 Programming Languages
 */


#include "lexeme.h"
#include "types.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>


struct lexeme
{
    char   *type;
    int     ival;
    double  rval;
    char   *sval;
    char   *ID;
    bool    tf;
    int     lineNum;
    Lexeme *left;
    Lexeme *right;
} ;


Lexeme *
newLexeme(char *x, int line)
{
    Lexeme *new = malloc(sizeof(Lexeme));
    assert(new != 0);
    new->type = x;
    new->lineNum = line;
    return new;
}


Lexeme *
newLexemeInt(int x, int line)
{
    Lexeme *new = malloc(sizeof(Lexeme));
    assert(new != 0);
    new->type = INTEGER;
    new->ival = x;
    new->lineNum = line;
    return new;
}


Lexeme *
newLexemeReal(double x, int line)
{
    Lexeme *new = malloc(sizeof(Lexeme));
    assert(new != 0);
    new->type = REAL;
    new->rval = x;
    new->lineNum = line;
    return new;
}


Lexeme *
newLexemeString(char *x, int line)
{
    Lexeme *new = malloc(sizeof(Lexeme));
    assert(new != 0);
    new->type = STRING;
    new->sval = x;
    new->lineNum = line;
    return new;
}


Lexeme *
newLexemeID(char *x, int line)
{
    Lexeme *new = malloc(sizeof(Lexeme));
    assert(new != 0);
    new->type = ID;
    new->ID   = x;
    new->lineNum = line;
    return new;
}


Lexeme *
newLexemeTf(bool x)
{
    Lexeme *new = malloc(sizeof(Lexeme));
    assert(new != 0);
    new->type = BOOL;
    new->tf   = x;
    return new;
}


char *
getLexemeType(Lexeme *n)
{
    return n->type;
}


int
getLexemeIval(Lexeme *n)
{
    return n->ival;
}


Lexeme *
minusLexemeIval(Lexeme *n, int x)
{
    Lexeme *new = newLexemeInt(x,getLineNum(n));
    return new;
}


double
getLexemeRval(Lexeme *n)
{
    return n->rval;
}


Lexeme *
minusLexemeRval(Lexeme *n, double x)
{
    Lexeme *new = newLexemeReal(x,getLineNum(n));
    return new;
}


char *
getLexemeSval(Lexeme *n)
{
    return n->sval;
}


char *
getLexemeID(Lexeme *n)
{
    return n->ID;
}


int
getLineNum(Lexeme *n)
{
    return n->lineNum;
}


bool
getLexemeTf(Lexeme *n)
{
    return n->tf;
}


Lexeme *
car(Lexeme *l)
{
    return l->left;
}


Lexeme *
cdr(Lexeme *l)
{
    return l->right;
}


void
set_car(Lexeme *parent, Lexeme *child)
{
    parent->left = child;
    return;
}


void
set_cdr(Lexeme *parent, Lexeme *child)
{
    parent->right = child;
    return;
}