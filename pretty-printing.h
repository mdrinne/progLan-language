/*
 * Matthew Rinne
 * pretty-printing.h
 * cs403 Programming Languages
 */


#ifndef __PRETTYPRINTING_INCLUDED__
#define __PRETTYPRINTING_INCLUDED__


#include "lexeme.h"

void fixLine(Lexeme *cur);
void fixLineElse(Lexeme *cur);
void pp(Lexeme *tree);
void printProgram(Lexeme *tree);
void printDefs(Lexeme *tree);
void printDef(Lexeme *tree);
void printOptDefs(Lexeme *tree);
void printFuncDef(Lexeme *tree);
void printParamList(Lexeme *tree);
void printOptParamList(Lexeme *tree);
void printMainFunc(Lexeme *tree);
void printVarDef(Lexeme *tree);
void printOptInit(Lexeme *tree);
void printUnary(Lexeme *tree);
void printUminus(Lexeme *tree);
void printOper(Lexeme *tree);
void printExpr(Lexeme *tree);
void printOptExpr(Lexeme *tree);
void printArgList(Lexeme *tree);
void printOptArgList(Lexeme *tree);
void printFuncCall(Lexeme *tree);
void printIfStatement(Lexeme *tree);
void printElseStatement(Lexeme *tree);
void printOptElse(Lexeme *tree);
void printWhileLoop(Lexeme *tree);
void printReturnStatement(Lexeme *tree);
void printForLoop(Lexeme *tree);
void printDoubleSelfOp(Lexeme *tree);
void printBlock(Lexeme *tree);
void printStatements(Lexeme *tree);
void printStatement(Lexeme *tree);
void printLambdaFunc(Lexeme *tree);
void printLambdaBlock(Lexeme *tree);


#endif