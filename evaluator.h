/*
 * Matthew Rinne
 * evaluator.h
 * cs403 Programming Languages
 */


#ifndef __EVALUATOR_INCLUDED__
#define __EVALUATOR_INCLUDED__


#include "lexeme.h"


void    setCLvars(int count, char **args);
char   *concatStrings(Lexeme *n, Lexeme *m);
char   *multString(Lexeme *n, int x);
int     isBuiltIn(char *n);


Lexeme *eval(Lexeme *tree, Lexeme *env);
Lexeme *evalProgram(Lexeme *tree, Lexeme *env);
Lexeme *evalOptDefs(Lexeme *tree, Lexeme *env);
Lexeme *evalDefs(Lexeme *tree, Lexeme *env);
Lexeme *evalDef(Lexeme *tree, Lexeme *env);
Lexeme *evalFuncDef(Lexeme *tree, Lexeme *env);
Lexeme *evalVarDef(Lexeme *tree, Lexeme *env);
Lexeme *evalOptInit(Lexeme *tree, Lexeme *env);
Lexeme *evalUnary(Lexeme *tree, Lexeme *env);
// Lexeme *evalUminus(Lexeme *tree, Lexeme *env);
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
Lexeme *evalEqualEqual(Lexeme *tree, Lexeme *env);
Lexeme *evalNotEqual(Lexeme *tree, Lexeme *env);
Lexeme *evalMod(Lexeme *tree, Lexeme *env);
Lexeme *evalOrOr(Lexeme *tree, Lexeme *env);
Lexeme *evalAndAnd(Lexeme *tree, Lexeme *env);
Lexeme *evalDoubleSelfOp(Lexeme *tree, Lexeme *env);
Lexeme *evalPlusPlus(Lexeme *tree, Lexeme *env);
Lexeme *evalMinusMinus(Lexeme *tree, Lexeme *env);
Lexeme *evalMainFunc(Lexeme *tree, Lexeme *env);
Lexeme *evalBlock(Lexeme *tree, Lexeme *env);
Lexeme *evalStatements(Lexeme *tree, Lexeme *env);
Lexeme *evalStatement(Lexeme *tree, Lexeme *env);
Lexeme *evalReturnStatement(Lexeme *tree, Lexeme *env);
Lexeme *evalFuncCall(Lexeme *tree, Lexeme *env);
Lexeme *evalOptArgList(Lexeme *tree, Lexeme *env);
Lexeme *evalArgList(Lexeme *tree, Lexeme *env);
Lexeme *evalIfStatement(Lexeme *tree, Lexeme *env);
Lexeme *evalOptElse(Lexeme *tree, Lexeme *env);
Lexeme *evalElseStatement(Lexeme *tree, Lexeme *env);
Lexeme *evalWhileLoop(Lexeme *tree, Lexeme *env);
Lexeme *evalForLoop(Lexeme *tree, Lexeme *env);
Lexeme *evalBuiltIn(Lexeme *closure, Lexeme *args);
Lexeme *evalNewArray(Lexeme *args);
Lexeme *evalSetArray(Lexeme *args);
Lexeme *evalGetArray(Lexeme *args);
Lexeme *evalGetArgCount();
Lexeme *evalGetArg(Lexeme *args);
Lexeme *evalOpenFileForReading(Lexeme *args);
Lexeme *evalReadInteger(Lexeme *args);
Lexeme *evalEOF(Lexeme *args);


Lexeme *evalConstructor(Lexeme *closure, Lexeme *env);


#endif