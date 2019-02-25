/*
 * Matthew Rinne
 * parser.h
 * cs403 Programming Languages
 */


#ifndef __PARSER_INCLUDED__
#define __PARSER_INCLUDED__


#include "lexeme.h"


Lexeme *advance();
int     check(char *type);
Lexeme *match(char *type);
int     getLegal();
Lexeme *cons(char *type, Lexeme *left, Lexeme *right);
Lexeme *program();
Lexeme *defs();
int     defsPending();
Lexeme *def();
int     defPending();
Lexeme *optDefs();
int     optDefsPending();
// Lexeme *includes();
// int     includesPending();
// Lexeme *includeStatement();
// Lexeme *optIncludes();
Lexeme *funcDef();
int     funcDefPending();
Lexeme *parameterList();
int     parameterListPending();
Lexeme *optParamList();
Lexeme *mainFunc();
Lexeme *varDef();
int     varDefPending();
Lexeme *optInit();
Lexeme *unary();
int     unaryPending();
Lexeme *uminus();
Lexeme *oper();
int     operPending();
Lexeme *expr();
int     exprPending();
Lexeme *optExpr();
Lexeme *argList();
int     argListPending();
Lexeme *optArgList();
Lexeme *funcCall();
int     funcCallPending();
Lexeme *ifStatement();
int     ifStatementPending();
Lexeme *elseStatement();
int     elseStatementPending();
Lexeme *optElse();
Lexeme *whileLoop();
int     whileLoopPending();
Lexeme *returnStatement();
int     returnStatementPending();
Lexeme *forLoop();
int     forLoopPending();
Lexeme *checkOper();
Lexeme *selfOp();
Lexeme *doubleSelfOp();
int     doubleSelfOpPending();
Lexeme *block();
Lexeme *statements();
int     statementsPending();
Lexeme *statement();
int     statementPending();
Lexeme *lambda();
// int     lambdaPending();


#endif