/*
 * Matthew Rinne
 * evaluator.h
 * cs403 Programming Languages
 */


#ifndef __EVALUATOR_INCLUDED__
#define __EVALUATOR_INCLUDED__


#include "lexeme.h"


char   *concatStrings(Lexeme *n, Lexeme *m);
char   *multString(Lexeme *n, int x);


Lexeme *eval(Lexeme *tree, Lexeme *env);
Lexeme *evalProgram(Lexeme *tree, Lexeme *env);
Lexeme *evalOptDefs(Lexeme *tree, Lexeme *env);
Lexeme *evalDefs(Lexeme *tree, Lexeme *env);
Lexeme *evalDef(Lexeme *tree, Lexeme *env);
Lexeme *evalFuncDef(Lexeme *tree, Lexeme *env);
Lexeme *evalVarDef(Lexeme *tree, Lexeme *env);
Lexeme *evalOptInit(Lexeme *tree, Lexeme *env);
Lexeme *evalUnary(Lexeme *tree, Lexeme *env);
Lexeme *evalUminus(Lexeme *tree, Lexeme *env);
Lexeme *evalOptExpr(Lexeme *tree, Lexeme *env);
Lexeme *evalExpr(Lexeme *tree, Lexeme *env);
Lexeme *evalOper(Lexeme *tree, Lexeme *env);
Lexeme *evalPlus(Lexeme *tree, Lexeme *env);
Lexeme *evalMinus(Lexeme *tree, Lexeme *env);
Lexeme *evalTimes(Lexeme *tree, Lexeme *env);
Lexeme *evalDivides(Lexeme *tree, Lexeme *env);
Lexeme *evalAssign(Lexeme *tree, Lexeme *env);
Lexeme *evalGreaterThan(Lexeme *tree, Lexeme *env);
Lexeme *evalGreaterThanOrEqual(Lexeme *tree, Lexeme *env);
Lexeme *evalLessThan(Lexeme *tree, Lexeme *env);
Lexeme *evalLessThanOrEqual(Lexeme *tree, Lexeme *env);
Lexeme *evalMod(Lexeme *tree, Lexeme *env);
Lexeme *evalEqualEqual(Lexeme *tree, Lexeme *env);
Lexeme *evalNotEqual(Lexeme *tree, Lexeme *env);
Lexeme *evalDoubleSelfOp(Lexeme *tree, Lexeme *env);

Lexeme *evalFuncCall(Lexeme *tree, Lexeme *env);

#endif