/*
 * Matthew Rinne
 * evaluator.c
 * cs403 Programming Languages
 */


#include "evaluator.h"
#include "environment.h"
#include "lexeme.h"
#include "types.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


char *
concatStrings(Lexeme *n, Lexeme *m)
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


char *
multString(Lexeme *n, int x)
{
    int len = 16;
    int index = 0;
    char *buffer = malloc(sizeof(char) * len+1);
    char *nStr = getLexemeSval(n);
    int nLen = strlen(nStr);
    int times = 0;
    while (times < x)
    {
        while ((index - (nLen*times)) < nLen)
        {
            if (index == len)
            {
                len *= 2;
                buffer = realloc(buffer,sizeof(char) * len+1);
            }
            buffer[index] = nStr[index-(nLen*times)];
            index++;
            buffer[index] = '\0';
        }
        times++;
    }
    return buffer;
}


Lexeme *
eval(Lexeme *tree, Lexeme *env)
{
    if (getLexemeType(tree) == INTEGER) return tree;
    else if (getLexemeType(tree) == REAL) return tree;
    else if (getLexemeType(tree) == STRING) return tree;
    else if (getLexemeType(tree) == ID) return lookup(tree,env);
    else if (getLexemeType(tree) == PROGRAM) return evalProgram(tree,env);
    else if (getLexemeType(tree) == OPTDEFS) return evalOptDefs(tree,env);
    else if (getLexemeType(tree) == DEFS) return evalDefs(tree,env);
    else if (getLexemeType(tree) == DEF) return evalDef(tree,env);
    else if (getLexemeType(tree) == FUNCDEF) return evalFuncDef(tree,env);
    else if (getLexemeType(tree) == VARDEF) return evalVarDef(tree,env);
    else if (getLexemeType(tree) == OPTINIT) return evalOptInit(tree,env);
    else if (getLexemeType(tree) == UNARY) return evalUnary(tree,env);
    // else if (getLexemeType(tree) == UMINUS) return evalUminus(tree,env);
    else if (getLexemeType(tree) == OPTEXPR) return evalOptExpr(tree,env);
    else if (getLexemeType(tree) == EXPR) return evalExpr(tree,env);
    else if (getLexemeType(tree) == OPER) return evalOper(car(tree),env);
    else if (getLexemeType(tree) == DOUBLESELFOP) return evalDoubleSelfOp(car(tree),env);
    else if (getLexemeType(tree) == MAINFUNC) return evalMainFunc(tree,env);
    else if (getLexemeType(tree) == BLOCK) return evalBlock(tree,env);
    else if (getLexemeType(tree) == STATEMENTS) return evalStatements(tree,env);
    else if (getLexemeType(tree) == STATEMENT) return evalStatement(tree,env);
    else if (getLexemeType(tree) == RETURNSTATEMENT) return evalReturnStatement(tree,env);
    else if (getLexemeType(tree) == FUNCCALL) return evalFuncCall(tree,env);
    else return NULL;
}


Lexeme *
evalProgram(Lexeme *tree, Lexeme *env)
{
    eval(car(tree),env);  // OPTDEFS
    eval(cdr(tree),env);  // MAINFUNC
    return NULL;
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
    return NULL;
}


Lexeme *
evalDef(Lexeme *tree, Lexeme *env)
{
    return eval(car(tree),env);  // VARDEF or FUNCDEF
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
    if (car(tree) != NULL)
    {
        // Lexeme *result = eval(car(tree),env);  // EXPR
        // printf("result TYPE: %s\n",getLexemeType(result));
        // return result;
        return eval(car(tree),env);
    }
    else return NULL;
}


Lexeme *
evalUnary(Lexeme *tree, Lexeme *env)
{
    return eval(car(tree),env);  // INTEGER or REAL or ...
}


// Lexeme *
// evalUminus(Lexeme *tree, Lexeme *env)
// {
//     if (getLexemeType(car(tree)) == INTEGER) return minusLexemeIval(car(tree),0 - getLexemeIval(car(tree)));
//     else if (getLexemeType(car(tree)) == REAL) return minusLexemeRval(car(tree),0 - getLexemeRval(car(tree)));
//     else if (getLexemeType(car(tree)) == ID) return evalUminus(lookup(car(tree),env),env);
//     else
//     {
//         printf("CAN ONLY TAKE THE NEGATIVE OF AN INTEGER OR REAL - LINE %d\n",getLineNum(tree));
//         exit(1);
//     }
// }


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
        // else return evalFuncCall(car(tree),env);
    }
    else if (getLexemeType(car(cdr(tree))) == OPER)
    {
        set_car(car(car(cdr(tree))),car(tree));
        set_cdr(car(car(cdr(tree))),cdr(cdr(tree)));
        // Lexeme *evalExpr = car(cdr(tree));
        // printf("evalExpr TYPE: %s\n",getLexemeType(evalExpr));
        // Lexeme *exprResult = eval(car(cdr(tree)),env);
        // printf("exprResult TYPE: %s\n",getLexemeType(exprResult));
        // return exprResult;
        return eval(car(cdr(tree)),env);
    }
    else
    {
        set_car(car(car(cdr(tree))),car(tree));
        return evalDoubleSelfOp(car(cdr(tree)),env);
    }
    return NULL;
}


Lexeme *
evalOper(Lexeme *tree, Lexeme *env)
{
    if (getLexemeType(tree) == PLUS)
    {
        // Lexeme *plusResult = evalPlus(tree,env);
        // printf("operResult TYPE: %s\n",getLexemeType(plusResult));
        // return plusResult;
        return evalPlus(tree,env);
    }
    else if (getLexemeType(tree) == MINUS)
    {
        // Lexeme *operResult = evalMinus(tree,env);
        // printf("operResult TYPE: %s\n",getLexemeType(operResult));
        // return operResult;
        return evalMinus(tree,env);
    }
    else if (getLexemeType(tree) == TIMES)
    {
        // Lexeme *operResult = evalTimes(tree,env);
        // printf("operResult TYPE: %s\n",getLexemeType(operResult));
        // return operResult;
        return evalTimes(tree,env);
    }
    else if (getLexemeType(tree) == DIVIDES)
    {
        // Lexeme *operResult = evalDivides(tree,env);
        // printf("operResult TYPE: %s\n",getLexemeType(operResult));
        // return operResult;
        return evalDivides(tree,env);
    }
    else if (getLexemeType(tree) == ASSIGN)
    {
        // Lexeme *operResult = evalAssign(tree,env);
        // printf("operResult TYPE: %s\n",getLexemeType(operResult));
        // return operResult;
        return evalAssign(tree,env);
    }
    else if (getLexemeType(tree) == GREATERTHAN)
    {
        // Lexeme *operResults = evalGreaterThan(tree,env);
        // printf("operResult TYPE: %s\n", getLexemeType(operResults));
        // return operResults;
        return evalGreaterThan(tree,env);
    }
    else if (getLexemeType(tree) == GREATERTHANOREQUAL)
    {
        // Lexeme *operResults = evalGreaterThanOrEqual(tree,env);
        // printf("operResult TYPE: %s\n", getLexemeType(operResults));
        // return operResults;
        return evalGreaterThanOrEqual(tree,env);
    }
    else if (getLexemeType(tree) == LESSTHAN)
    {
        // Lexeme *operResults = evalLessThan(tree,env);
        // printf("operResult TYPE: %s\n", getLexemeType(operResults));
        // return operResults;
        return evalLessThan(tree,env);
    }
    else if (getLexemeType(tree) == LESSTHANOREQUAL)
    {
        // Lexeme *operResults = evalLessThanOrEqual(tree,env);
        // printf("operResult TYPE: %s\n", getLexemeType(operResults));
        // return operResults;
        return evalLessThanOrEqual(tree,env);
    }
    else if (getLexemeType(tree) == EQUALEQUAL)
    {
        // Lexeme *operResults = evalEqualEqual(tree,env);
        // printf("operResult TYPE: %s\n", getLexemeType(operResults));
        // return operResults;
        return evalEqualEqual(tree,env);
    }
    else if (getLexemeType(tree) == NOTEQUALS)
    {
        // Lexeme *operResults = evalNotEqual(tree,env);
        // printf("operResult TYPE: %s\n", getLexemeType(operResults));
        // return operResults;
        return evalNotEqual(tree,env);
    }
    else if (getLexemeType(tree) == MOD) 
    {
        // Lexeme *operResults = evalMod(tree,env);
        // printf("operResult TYPE: %s\n", getLexemeType(operResults));
        // return operResults;
        return evalMod(tree,env);
    }
    else if (getLexemeType(tree) == OROR)
    {
        // Lexeme *operResults = evalOrOr(tree,env);
        // printf("operResult TYPE: %s\n", getLexemeType(operResults));
        // return operResults;
        return evalOrOr(tree,env);
    }
    else if (getLexemeType(tree) == ANDAND)
    {
        // Lexeme *operResults = evalAndAnd(tree,env);
        // printf("operResult TYPE: %s\n", getLexemeType(operResults));
        // return operResults;
        return evalAndAnd(tree,env);
    }
    return NULL;
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


Lexeme *
evalTimes(Lexeme *tree, Lexeme *env)
{
    Lexeme *left = eval(car(tree),env);
    Lexeme *right = eval(cdr(tree),env);
    if (getLexemeType(left) == INTEGER && getLexemeType(right) == INTEGER) return newLexemeInt(getLexemeIval(left) * getLexemeIval(right),0);
    else if (getLexemeType(left) == INTEGER && getLexemeType(right) == REAL) return newLexemeReal(getLexemeIval(left) * getLexemeRval(right),0);
    else if (getLexemeType(left) == REAL && getLexemeType(right) == INTEGER) return newLexemeReal(getLexemeRval(left) * getLexemeIval(right),0);
    else if (getLexemeType(left) == REAL && getLexemeType(right) == REAL) return newLexemeReal(getLexemeRval(left) * getLexemeRval(right),0);
    else if (getLexemeType(left) == STRING && getLexemeType(right) == INTEGER) return newLexemeString(multString(left,getLexemeIval(right)),0);
    else if (getLexemeType(left) == INTEGER && getLexemeType(right) == STRING) return newLexemeString(multString(right,getLexemeIval(left)),0);
    else
    {
        printf("CANNOT MULTIPLY A STRING AND A STRING/REAL -- LINE %d\n",getLineNum(left));
        exit(1);
    }
}


Lexeme *
evalDivides(Lexeme *tree, Lexeme *env)
{
    Lexeme *left = eval(car(tree),env);
    Lexeme *right = eval(cdr(tree),env);
    if (getLexemeType(left) == INTEGER && getLexemeType(right) == INTEGER) return newLexemeInt(getLexemeIval(left) / getLexemeIval(right),0);
    else if (getLexemeType(left) == INTEGER && getLexemeType(right) == REAL) return newLexemeReal(getLexemeIval(left) / getLexemeRval(right),0);
    else if (getLexemeType(left) == REAL && getLexemeType(right) == INTEGER) return newLexemeReal(getLexemeRval(left) / getLexemeIval(right),0);
    else if (getLexemeType(left) == REAL && getLexemeType(right) == REAL) return newLexemeReal(getLexemeRval(left) / getLexemeRval(right),0);
    else
    {
        printf("CAN ONLY DIVIDE NUMBERS -- LINE %d\n",getLineNum(left));
        exit(1);
    }
}


Lexeme *
evalAssign(Lexeme *tree, Lexeme *env)
{
    // printf("evalAssign car(car(tree)) TYPE: %s\n", getLexemeType(car(car(tree))));
    // printf("evalAssign car(car(tree)) ID: %s\n", getLexemeID(car(car(tree))));
    // Lexeme *left = eval(car(car(tree)),env);
    // printf("left TYPE:%s\n",getLexemeType(left));
    Lexeme *right = eval(cdr(tree),env);
    return update(env,car(car(tree)),right);
}


Lexeme *
evalGreaterThan(Lexeme *tree, Lexeme *env)
{
    Lexeme *left = eval(car(tree),env);
    Lexeme *right = eval(cdr(tree),env);
    if (getLexemeType(left) == INTEGER && getLexemeType(right) == INTEGER)
    {
        if (getLexemeIval(left) > getLexemeIval(right)) return newLexemeTf(true);
        else return newLexemeTf(false);
    }
    else if (getLexemeType(left) == INTEGER && getLexemeType(right) == REAL)
    {
        if (getLexemeIval(left) > getLexemeRval(right)) return newLexemeTf(true);
        else return newLexemeTf(false);
    }
    else if (getLexemeType(left) == REAL && getLexemeType(right) == INTEGER)
    {
        if (getLexemeRval(left) > getLexemeIval(right)) return newLexemeTf(true);
        else return newLexemeTf(false);
    }
    else if (getLexemeType(left) == REAL && getLexemeType(right) == REAL)
    {
        if (getLexemeRval(left) > getLexemeRval(right)) return newLexemeTf(true);
        else return newLexemeTf(false);
    }
    else if (getLexemeType(left) == STRING && getLexemeType(right) == STRING)
    {
        if (strcmp(getLexemeSval(left),getLexemeSval(right)) > 0) return newLexemeTf(true);
        else return newLexemeTf(false);
    }
    else
    {
        printf("CAN ONLY COMPARE STRINGS WITH STRINGS OR NUMBERS WITH NUMBERS -- LINE %d\n", getLineNum(car(tree)));
        exit(1);
    }
}


Lexeme *
evalGreaterThanOrEqual(Lexeme *tree, Lexeme *env)
{
    Lexeme *left = eval(car(tree),env);
    Lexeme *right = eval(cdr(tree),env);
    if (getLexemeType(left) == INTEGER && getLexemeType(right) == INTEGER)
    {
        if (getLexemeIval(left) >= getLexemeIval(right)) return newLexemeTf(true);
        else return newLexemeTf(false);
    }
    else if (getLexemeType(left) == INTEGER && getLexemeType(right) == REAL)
    {
        if (getLexemeIval(left) >= getLexemeRval(right)) return newLexemeTf(true);
        else return newLexemeTf(false);
    }
    else if (getLexemeType(left) == REAL && getLexemeType(right) == INTEGER)
    {
        if (getLexemeRval(left) >= getLexemeIval(right)) return newLexemeTf(true);
        else return newLexemeTf(false);
    }
    else if (getLexemeType(left) == REAL && getLexemeType(right) == REAL)
    {
        if (getLexemeRval(left) >= getLexemeRval(right)) return newLexemeTf(true);
        else return newLexemeTf(false);
    }
    else if (getLexemeType(left) == STRING && getLexemeType(right) == STRING)
    {
        if (strcmp(getLexemeSval(left),getLexemeSval(right)) >= 0) return newLexemeTf(true);
        else return newLexemeTf(false);
    }
    else
    {
        printf("CAN ONLY COMPARE STRINGS WITH STRINGS OR NUMBERS WITH NUMBERS -- LINE %d\n", getLineNum(car(tree)));
        exit(1);
    }
}


Lexeme *
evalLessThan(Lexeme *tree, Lexeme *env)
{
    Lexeme *left = eval(car(tree),env);
    Lexeme *right = eval(cdr(tree),env);
    if (getLexemeType(left) == INTEGER && getLexemeType(right) == INTEGER)
    {
        if (getLexemeIval(left) < getLexemeIval(right)) return newLexemeTf(true);
        else return newLexemeTf(false);
    }
    else if (getLexemeType(left) == INTEGER && getLexemeType(right) == REAL)
    {
        if (getLexemeIval(left) < getLexemeRval(right)) return newLexemeTf(true);
        else return newLexemeTf(false);
    }
    else if (getLexemeType(left) == REAL && getLexemeType(right) == INTEGER)
    {
        if (getLexemeRval(left) < getLexemeIval(right)) return newLexemeTf(true);
        else return newLexemeTf(false);
    }
    else if (getLexemeType(left) == REAL && getLexemeType(right) == REAL)
    {
        if (getLexemeRval(left) < getLexemeRval(right)) return newLexemeTf(true);
        else return newLexemeTf(false);
    }
    else if (getLexemeType(left) == STRING && getLexemeType(right) == STRING)
    {
        if (strcmp(getLexemeSval(left),getLexemeSval(right)) < 0) return newLexemeTf(true);
        else return newLexemeTf(false);
    }
    else
    {
        printf("CAN ONLY COMPARE STRINGS WITH STRINGS OR NUMBERS WITH NUMBERS -- LINE %d\n", getLineNum(car(tree)));
        exit(1);
    }
}


Lexeme *
evalLessThanOrEqual(Lexeme *tree, Lexeme *env)
{
    Lexeme *left = eval(car(tree),env);
    Lexeme *right = eval(cdr(tree),env);
    if (getLexemeType(left) == INTEGER && getLexemeType(right) == INTEGER)
    {
        if (getLexemeIval(left) <= getLexemeIval(right)) return newLexemeTf(true);
        else return newLexemeTf(false);
    }
    else if (getLexemeType(left) == INTEGER && getLexemeType(right) == REAL)
    {
        if (getLexemeIval(left) <= getLexemeRval(right)) return newLexemeTf(true);
        else return newLexemeTf(false);
    }
    else if (getLexemeType(left) == REAL && getLexemeType(right) == INTEGER)
    {
        if (getLexemeRval(left) <= getLexemeIval(right)) return newLexemeTf(true);
        else return newLexemeTf(false);
    }
    else if (getLexemeType(left) == REAL && getLexemeType(right) == REAL)
    {
        if (getLexemeRval(left) <= getLexemeRval(right)) return newLexemeTf(true);
        else return newLexemeTf(false);
    }
    else if (getLexemeType(left) == STRING && getLexemeType(right) == STRING)
    {
        if (strcmp(getLexemeSval(left),getLexemeSval(right)) <= 0) return newLexemeTf(true);
        else return newLexemeTf(false);
    }
    else
    {
        printf("CAN ONLY COMPARE STRINGS WITH STRINGS OR NUMBERS WITH NUMBERS -- LINE %d\n", getLineNum(car(tree)));
        exit(1);
    }
}


Lexeme *
evalEqualEqual(Lexeme *tree, Lexeme *env)
{
    Lexeme *left = eval(car(tree),env);
    Lexeme *right = eval(cdr(tree),env);
    if (getLexemeType(left) == INTEGER && getLexemeType(right) == INTEGER)
    {
        if (getLexemeIval(left) == getLexemeIval(right)) return newLexemeTf(true);
        else return newLexemeTf(false);
    }
    else if (getLexemeType(left) == INTEGER && getLexemeType(right) == REAL)
    {
        if (getLexemeIval(left) == getLexemeRval(right)) return newLexemeTf(true);
        else return newLexemeTf(false);
    }
    else if (getLexemeType(left) == REAL && getLexemeType(right) == INTEGER)
    {
        if (getLexemeRval(left) == getLexemeIval(right)) return newLexemeTf(true);
        else return newLexemeTf(false);
    }
    else if (getLexemeType(left) == REAL && getLexemeType(right) == REAL)
    {
        if (getLexemeRval(left) == getLexemeRval(right)) return newLexemeTf(true);
        else return newLexemeTf(false);
    }
    else if (getLexemeType(left) == STRING && getLexemeType(right) == STRING)
    {
        if (strcmp(getLexemeSval(left),getLexemeSval(right)) == 0) return newLexemeTf(true);
        else return newLexemeTf(false);
    }
    else
    {
        printf("CAN ONLY COMPARE STRINGS WITH STRINGS OR NUMBERS WITH NUMBERS -- LINE %d\n", getLineNum(car(tree)));
        exit(1);
    }
}


Lexeme *
evalNotEqual(Lexeme *tree, Lexeme *env)
{
    Lexeme *left = eval(car(tree),env);
    Lexeme *right = eval(cdr(tree),env);
    if (getLexemeType(left) == INTEGER && getLexemeType(right) == INTEGER)
    {
        if (getLexemeIval(left) != getLexemeIval(right)) return newLexemeTf(true);
        else return newLexemeTf(false);
    }
    else if (getLexemeType(left) == INTEGER && getLexemeType(right) == REAL)
    {
        if (getLexemeIval(left) != getLexemeRval(right)) return newLexemeTf(true);
        else return newLexemeTf(false);
    }
    else if (getLexemeType(left) == REAL && getLexemeType(right) == INTEGER)
    {
        if (getLexemeRval(left) != getLexemeIval(right)) return newLexemeTf(true);
        else return newLexemeTf(false);
    }
    else if (getLexemeType(left) == REAL && getLexemeType(right) == REAL)
    {
        if (getLexemeRval(left) != getLexemeRval(right)) return newLexemeTf(true);
        else return newLexemeTf(false);
    }
    else if (getLexemeType(left) == STRING && getLexemeType(right) == STRING)
    {
        if (strcmp(getLexemeSval(left),getLexemeSval(right)) != 0) return newLexemeTf(true);
        else return newLexemeTf(false);
    }
    else
    {
        printf("CAN ONLY COMPARE STRINGS WITH STRINGS OR NUMBERS WITH NUMBERS -- LINE %d\n", getLineNum(car(tree)));
        exit(1);
    }
}


Lexeme *
evalMod(Lexeme *tree, Lexeme *env)
{
    Lexeme *left = eval(car(tree),env);
    Lexeme *right = eval(cdr(tree),env);
    if (getLexemeType(left) == INTEGER && getLexemeType(right) == INTEGER) return newLexemeInt(getLexemeIval(left) % getLexemeIval(right),0);
    else
    {
        printf("CAN ONLY MOD INTEGERS -- LINE %d\n", getLineNum(car(tree)));
        exit(1);
    }
}


Lexeme *
evalOrOr(Lexeme *tree, Lexeme *env)
{
    Lexeme *left = eval(car(tree),env);
    Lexeme *right = eval(cdr(tree),env);
    if (getLexemeType(left) == INTEGER && getLexemeType(right) == INTEGER)
    {
        if (getLexemeIval(left) == 1 || getLexemeIval(right) == 1) return newLexemeTf(true);
        else return newLexemeTf(false);
    }
    else if (getLexemeType(left) == INTEGER && getLexemeType(right) == BOOL)
    {
        if (getLexemeIval(left) == 1 || getLexemeTf(right) == true) return newLexemeTf(true);
        else return newLexemeTf(false);
    }
    else if (getLexemeType(left) == BOOL && getLexemeType(right) == INTEGER)
    {
        if (getLexemeTf(left) == true || getLexemeIval(right) == 1) return newLexemeTf(true);
        else return newLexemeTf(false);
    }
    else if (getLexemeType(left) == BOOL && getLexemeType(right) == BOOL)
    {
        if (getLexemeTf(left) == true || getLexemeTf(right) == true) return newLexemeTf(true);
        else return newLexemeTf(false);
    }
    else
    {
        printf("CAN ONLY 'OR' INTEGERS AND BOOLS -- LINE %d\n",getLineNum(left));
        exit(1);
    }
}


Lexeme *
evalAndAnd(Lexeme *tree, Lexeme *env)
{
    Lexeme *left = eval(car(tree),env);
    Lexeme *right = eval(cdr(tree),env);
    if (getLexemeType(left) == INTEGER && getLexemeType(right) == INTEGER)
    {
        if (getLexemeIval(left) == 1 && getLexemeIval(right) == 1) return newLexemeTf(true);
        else return newLexemeTf(false);
    }
    else if (getLexemeType(left) == INTEGER && getLexemeType(right) == BOOL)
    {
        if (getLexemeIval(left) == 1 && getLexemeTf(right) == true) return newLexemeTf(true);
        else return newLexemeTf(false);
    }
    else if (getLexemeType(left) == BOOL && getLexemeType(right) == INTEGER)
    {
        if (getLexemeTf(left) == true && getLexemeIval(right) == 1) return newLexemeTf(true);
        else return newLexemeTf(false);
    }
    else if (getLexemeType(left) == BOOL && getLexemeType(right) == BOOL)
    {
        if (getLexemeTf(left) == true && getLexemeTf(right) == true) return newLexemeTf(true);
        else return newLexemeTf(false);
    }
    else
    {
        printf("CAN ONLY 'AND' INTEGERS AND BOOLS -- LINE %d\n",getLineNum(left));
        exit(1);
    }
}


Lexeme *
evalDoubleSelfOp(Lexeme *tree, Lexeme *env)
{
    // printf("in doubleselfop\n");
    // printf("tree TYPE: %s\n",getLexemeType(car(tree)));
    if (getLexemeType(car(tree)) == PLUSPLUS)
    {
        // Lexeme *operResults = evalPlusPlus(car(tree),env);
        // printf("operResult TYPE: %s\n", getLexemeType(operResults));
        // return operResults;
        return evalPlusPlus(car(tree),env);
    }
    else if (getLexemeType(car(tree)) == MINUSMINUS) return evalMinusMinus(car(tree),env);
    else return NULL;
}


Lexeme *
evalPlusPlus(Lexeme *tree, Lexeme *env)
{
    Lexeme *num = eval(car(tree),env);
    if (getLexemeType(num) == INTEGER) return newLexemeInt(getLexemeIval(num) + 1,0);
    else if (getLexemeType(num) == REAL) return newLexemeReal(getLexemeRval(num) + 1,0);
    else
    {
        printf("CAN ONLY INCREMENT NUMBERS -- LINE %d\n", getLineNum(num));
        exit(1);
    }
}


Lexeme *
evalMinusMinus(Lexeme *tree, Lexeme *env)
{
    Lexeme *num = eval(car(tree),env);
    if (getLexemeType(num) == INTEGER) return newLexemeInt(getLexemeIval(num) - 1,0);
    else if (getLexemeType(num) == REAL) return newLexemeReal(getLexemeRval(num) - 1,0);
    else
    {
        printf("CAN ONLY DECREMENT NUMBERS -- LINE %d\n", getLineNum(num));
        exit(1);
    }
}


Lexeme *
evalMainFunc(Lexeme *tree, Lexeme *env)
{
    Lexeme *mainEnv = extend(env,NULL,NULL);
    return eval(car(tree),mainEnv);
}


Lexeme *
evalBlock(Lexeme *tree, Lexeme *env)
{
    return eval(car(tree),env);
}


Lexeme *
evalStatements(Lexeme *tree, Lexeme *env)
{
    while (tree != NULL)
    {
        Lexeme *result = eval(car(tree),env);
        if (getLexemeType(result) == RETURNED) break;
        tree = cdr(tree);
    }
    return eval(car(tree),env);
}


Lexeme *
evalStatement(Lexeme *tree, Lexeme *env)
{
    // printf("evaluating statement: %s\n", getLexemeType(car(tree)));
    // printf("defining ID: %s\n",getLexemeID(car(car(tree))));
    // Lexeme *result = eval(car(tree),env);
    // displayEnv(env);
    // return result;
    return eval(car(tree),env);
}


Lexeme *
evalReturnStatement(Lexeme *tree, Lexeme *env)
{
    // printf("in evalReturnStatement\n");
    Lexeme *returned = cons(RETURNED,eval(car(tree),env),NULL);
    return returned;
}