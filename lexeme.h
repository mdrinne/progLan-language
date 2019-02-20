/*
 * Matthew Rinne
 * lexeme.h
 * cs403 Programming Languages
 */


#ifndef __LEXEME_INCLUDED__
#define __LEXEME_INCLUDED__


#include <stdio.h>


typedef struct lexeme Lexeme;


Lexeme *newLexeme(char *x, int line);
Lexeme *newLexemeInt(int x, int line);
Lexeme *newLexemeReal(double x, int line);
Lexeme *newLexemeString(char *x, int line);
Lexeme *newLexemeID(char *x, int line);
char   *getLexemeType(Lexeme *n);
int     getLexemeIval(Lexeme *n);
Lexeme *minusLexemeIval(Lexeme *n, int x);
double  getLexemeRval(Lexeme *n);
Lexeme *minusLexemeRval(Lexeme *n, double x);
char   *getLexemeSval(Lexeme *n);
char   *getLexemeID(Lexeme *n);
int     getLineNum(Lexeme *n);
Lexeme *car(Lexeme *l); /* gets lchild */
Lexeme *cdr(Lexeme *l); /* gets rchild */
void    set_car(Lexeme *parent, Lexeme *child);
void    set_cdr(Lexeme *parent, Lexeme *child);

#endif