/*
 * Matthew Rinne
 * environment.h
 * cs403 Programming Languages
 */


#ifndef __ENVIRONMENT_INCLUDED__
#define __ENVIRONMENT_INCLUDED__


#include <stdio.h>
#include "lexeme.h"


Lexeme *insertEnv(Lexeme *env, Lexeme *id, Lexeme *val);
Lexeme *lookup(Lexeme *id, Lexeme *env);
Lexeme *update(Lexeme *env, Lexeme *id, Lexeme *val);
Lexeme *newEnv();
Lexeme *extend(Lexeme *env, Lexeme *vars, Lexeme *vals);
void    displayEnv(Lexeme *env);

#endif