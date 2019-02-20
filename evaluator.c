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
#include <string.h>


char *concatStrings(Lexeme *n, Lexeme *m)
{
    int len = 16;
    int index = 0;
    char *buffer = malloc(sizeof(char) * len+1);
    char *nStr = getLexemeSval(n);
    char *mStr = getLexemeSval(m);
    int nLen = strlen(nStr);
    int mLen = strlen(mStr);
    while (index < nLen)
    {
        if (index == len)
        {
            len *= 2;
            buffer = realloc(buffer,sizeof(char) * len+1);
        }
        buffer[index] = nStr[index];
        index++;
        buffer[index] = '\0';
    }
    while ((index - nLen) < mLen)
    {
        if (index == len)
        {
            len *= 2;
            buffer = realloc(buffer,sizeof(char) * len+1);
        }
        buffer[index] = mStr[index-nLen];
        index++;
        buffer[index] = '\0';
    }
    return buffer;
}


Lexeme *
eval(Lexeme *tree, Lexeme *env)
{
    if (getLexemeType(tree) == INTEGER) return tree;
    else if (getLexemeType(tree) == REAL) return tree;
    else if (getLexemeType(tree) == STRING) return tree;
    else if (getLexemeType(tree) == ID) return lookup(getLexemeID(tree),env);
    else if (getLexemeType(tree) == PROGRAM) return evalProgram(tree,env);
    else if (getLexemeType(tree) == OPTDEFS) return evalOptDefs(tree,env);
    else if (getLexemeType(tree) == DEFS) return evalDefs(tree,env);
    else if (getLexemeType(tree) == DEF) return evalDef(tree,env);
    else if (getLexemeType(tree) == FUNCDEF) return evalFuncDef(tree,env);
    else if (getLexemeType(tree) == VARDEF) return evalVarDef(tree,env);
    else if (getLexemeType(tree) == OPTINIT) return evalOptInit(tree,env);
    else if (getLexemeType(tree) == UNARY) return evalUnary(tree,env);
    else if (getLexemeType(tree) == UMINUS) return evalUminus(tree,env);
    else if (getLexemeType(tree) == OPTEXPR) return evalOptExpr(tree,env);
    else if (getLexemeType(tree) == EXPR) return evalExpr(tree,env);
    else if (getLexemeType(tree) == OPER) return evalOper(car(tree),env);
    else if (getLexemeType(tree) == DOUBLESELFOP) return evalDoubleSelfOp(tree,env);
}


Lexeme *
evalProgram(Lexeme *tree, Lexeme *env)
{
    Lexeme *o = eval(car(tree),env);  // OPTDEFS
    Lexeme *m = eval(cdr(tree),env);  // MAINFUNC
}


Lexeme *
evalOptDefs(Lexeme *tree, Lexeme *env)
{
    if (car(tree) != NULL) return eval(car(tree),env);  // DEFS
    else return NULL;
}


Lexeme *
evalDefs(Lexeme *tree, Lexeme *env)
{
    while (tree != NULL)
    {
        eval(car(tree),env);  // DEF
        tree = cdr(tree);  // DEFS or NULL
    }
}


Lexeme *
evalDef(Lexeme *tree, Lexeme *env)
{
    eval(car(tree),env);  // VARDEF or FUNCDEF
}


Lexeme *
evalFuncDef(Lexeme *tree, Lexeme *env)
{
    return insertEnv(env,car(tree),cons(CLOSURE,env,tree));
}


Lexeme *
evalVarDef(Lexeme *tree, Lexeme *env)
{
    return insertEnv(env,car(tree),eval(cdr(tree),env));
}


Lexeme *
evalOptInit(Lexeme *tree, Lexeme *env)
{
    if (car(tree) != NULL) return eval(car(tree),env);  // EXPR
    else return NULL;
}


Lexeme *
evalUnary(Lexeme *tree, Lexeme *env)
{
    eval(car(tree),env);  // INTEGER or REAL or ...
}


Lexeme *
evalUminus(Lexeme *tree, Lexeme *env)
{
    if (getLexemeType(car(tree)) == INTEGER) return minusLexemeIval(car(tree),0 - getLexemeIval(car(tree)));
    else if (getLexemeType(car(tree)) == REAL) return minusLexemeRval(car(tree),0 - getLexemeRval(car(tree)));
    else if (getLexemeType(car(tree)) == ID) return evalUminus(lookup(car(tree),env),env);
    else
    {
        printf("CAN ONLY TAKE THE NEGATIVE OF AN INTEGER OR REAL - LINE %d\n",getLineNum(tree));
        exit(1);
    }
}


Lexeme *
evalOptExpr(Lexeme *tree, Lexeme *env)
{
    if (car(tree) != NULL) return eval(car(tree),env);
    else return NULL;
}


Lexeme *
evalExpr(Lexeme *tree, Lexeme *env)
{
    if (car(cdr(tree)) == NULL)
    {
        if (getLexemeType(car(tree)) == UNARY) return evalUnary(car(tree),env);
        else return evalFuncCall(car(tree),env);
    }
    else if (getLexemeType(car(cdr(tree))) == OPER)
    {
        set_car(car(car(cdr(tree))),car(tree));
        set_cdr(car(car(cdr(tree))),cdr(cdr(tree)));
        return evalOper(car(cdr(tree)),env);
    }
    else
    {
        set_car(car(car(cdr(tree))),car(tree));
        return evalDoubleSelfOp(car(cdr(tree)),env);
    }
}


Lexeme *
evalOper(Lexeme *tree, Lexeme *env)
{
    if (getLexemeType(tree) == PLUS) evalPlus(tree,env);
    else if (getLexemeType(tree) == MINUS) evalMinus(tree,env);
    else if (getLexemeType(tree) == TIMES) evalTimes(tree,env);
    else if (getLexemeType(tree) == DIVIDES) evalDivides(tree,env);
    else if (getLexemeType(tree) == ASSIGN) evalAssign(tree,env);
    else if (getLexemeType(tree) == GREATERTHAN) evalGreaterThan(tree,env);
    else if (getLexemeType(tree) == GREATERTHANOREQUAL) evalGreaterThanOrEqual(tree,env);
    else if (getLexemeType(tree) == LESSTHAN) evalLessThan(tree,env);
    else if (getLexemeType(tree) == LESSTHANOREQUAL) evalLessThanOrEqual(tree,env);
    else if (getLexemeType(tree) == MOD) evalMod(tree,env);
    else if (getLexemeType(tree) == EQUALEQUAL) evalEqualEqual(tree,env);
    else if (getLexemeType(tree) == NOTEQUALS) evalNotEqual(tree,env);
}


Lexeme *
evalPlus(Lexeme *tree, Lexeme *env)
{
    Lexeme *left = eval(car(tree),env);
    Lexeme *right = eval(cdr(tree),env);
    if (getLexemeType(left) == INTEGER && getLexemeType(right) == INTEGER) return newLexemeInt(getLexemeIval(left) + getLexemeIval(right),0);
    else if (getLexemeType(left) == INTEGER && getLexemeType(right) == REAL) return newLexemeReal(getLexemeIval(left) + getLexemeRval(right),0);
    else if (getLexemeType(left) == REAL && getLexemeType(right) == INTEGER) return newLexemeReal(getLexemeRval(left) + getLexemeIval(right),0);
    else if (getLexemeType(left) == REAL && getLexemeType(right) == REAL) return newLexemeReal(getLexemeRval(left) + getLexemeRval(right),0);
    else if (getLexemeType(left) == STRING && getLexemeType(right) == STRING) return newLexemeString(concatStrings(left,right),0);
    else
    {
        printf("CANNOT ADD A STRING TO A NUMBER -- LINE %d\n",getLineNum(left));
        exit(1);
    }
}


Lexeme *
evalMinus(Lexeme *tree, Lexeme *env)
{
    Lexeme *left = eval(car(tree),env);
    Lexeme *right = eval(cdr(tree),env);
    if (getLexemeType(left) == INTEGER && getLexemeType(right) == INTEGER) return newLexemeInt(getLexemeIval(left) - getLexemeIval(right),0);
    else if (getLexemeType(left) == INTEGER && getLexemeType(right) == REAL) return newLexemeReal(getLexemeIval(left) - getLexemeRval(right),0);
    else if (getLexemeType(left) == REAL && getLexemeType(right) == INTEGER) return newLexemeReal(getLexemeRval(left) - getLexemeIval(right),0);
    else if (getLexemeType(left) == REAL && getLexemeType(right) == REAL) return newLexemeReal(getLexemeRval(left) - getLexemeRval(right),0);
    else
    {
        printf("CAN ONLY SUBTRACT NUMBERS -- LINE %d\n",getLineNum(left));
        exit(1);
    }
}