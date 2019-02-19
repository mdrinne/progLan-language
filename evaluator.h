/*
 * Matthew Rinne
 * evaluator.h
 * cs403 Programming Languages
 */


#ifndef __EVALUATOR_INCLUDED__
#define __EVALUATOR_INCLUDED__


#include "lexeme.h"


Lexeme *eval(Lexeme *tree, Lexeme *env);


#endif