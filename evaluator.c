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
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int countCL;
char **argsCL;


void
setCLvars(int count, char **args)
{
    countCL = count;
    argsCL = args;
    return;
}


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


int
isBuiltIn(char *n)
{
    if (strcmp(n,"newArray") == 0) return 1;
    else if (strcmp(n,"setArray") == 0) return 1;
    else if (strcmp(n,"getArray") == 0) return 1;
    else if (strcmp(n,"getArgCount") == 0) return 1;
    else if (strcmp(n,"getArg") == 0) return 1;
    else if (strcmp(n,"openFileForReading") == 0) return 1;
    else if (strcmp(n,"readInt") == 0) return 1;
    else if (strcmp(n,"eof") == 0) return 1;
    else if (strcmp(n,"closeFile") == 0) return 1;
    else if (strcmp(n,"print") == 0) return 1;
    else if (strcmp(n,"printNewLine") == 0) return 1;
    else return 0;
}


Lexeme *
eval(Lexeme *tree, Lexeme *env)
{
    if (getLexemeType(tree) == INTEGER) return tree;
    else if (getLexemeType(tree) == REAL) return tree;
    else if (getLexemeType(tree) == STRING) return tree;
    else if (getLexemeType(tree) == ARRAY) return tree;
    else if (getLexemeType(tree) == ID) return lookup(tree,env);
    else if (getLexemeType(tree) == FP) return tree;
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
    else if (getLexemeType(tree) == IFSTATEMENT) return evalIfStatement(tree,env);
    else if (getLexemeType(tree) == OPTELSE) return evalOptElse(tree,env);
    else if (getLexemeType(tree) == ELSESTATEMENT) return evalElseStatement(tree,env);
    else if (getLexemeType(tree) == WHILELOOP) return evalWhileLoop(tree,env);
    else if (getLexemeType(tree) == FORLOOP) return evalForLoop(tree,env);
    else if (getLexemeType(tree) == LAMBDAFUNC) return evalLambdaFunc(tree,env);
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
    if (car(tree) != NULL) return eval(car(tree),env);
    else return NULL;
}


Lexeme *
evalUnary(Lexeme *tree, Lexeme *env)
{
    return eval(car(tree),env);  // INTEGER or REAL or ...
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
        return eval(car(cdr(tree)),env);
    }
    else
    {
        Lexeme *id = car(car(tree));
        set_car(car(car(cdr(tree))),car(tree));
        Lexeme *result = evalDoubleSelfOp(car(cdr(tree)),env);
        return update(env,id,result);
    }
    return NULL;
}


Lexeme *
evalOper(Lexeme *tree, Lexeme *env)
{
    if (getLexemeType(tree) == PLUS) return evalPlus(tree,env);
    else if (getLexemeType(tree) == MINUS) return evalMinus(tree,env);
    else if (getLexemeType(tree) == TIMES) return evalTimes(tree,env);
    else if (getLexemeType(tree) == DIVIDES) return evalDivides(tree,env);
    else if (getLexemeType(tree) == ASSIGN) return evalAssign(tree,env);
    else if (getLexemeType(tree) == GREATERTHAN) return evalGreaterThan(tree,env);
    else if (getLexemeType(tree) == GREATERTHANOREQUAL) return evalGreaterThanOrEqual(tree,env);
    else if (getLexemeType(tree) == LESSTHAN) return evalLessThan(tree,env);
    else if (getLexemeType(tree) == LESSTHANOREQUAL) return evalLessThanOrEqual(tree,env);
    else if (getLexemeType(tree) == EQUALEQUAL) return evalEqualEqual(tree,env);
    else if (getLexemeType(tree) == NOTEQUALS) return evalNotEqual(tree,env);
    else if (getLexemeType(tree) == MOD) return evalMod(tree,env);
    else if (getLexemeType(tree) == OROR) return evalOrOr(tree,env);
    else if (getLexemeType(tree) == ANDAND) return evalAndAnd(tree,env);
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
    Lexeme *right;
    if (getLexemeType(cdr(tree)) == FUNCCALL) right = evalFuncCall(cdr(tree),env);
    else right = eval(cdr(tree),env);
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
    else if (getLexemeType(left) == BOOL && getLexemeType(right) == BOOL)
    {
        if (getLexemeTf(left) == getLexemeTf(right)) return newLexemeTf(false);
        else return newLexemeTf(true);
    }
    else
    {
        printf("CAN ONLY COMPARE STRINGS WITH STRINGS OR NUMBERS WITH NUMBERS OR BOOL WITH BOOL -- LINE %d\n", getLineNum(car(tree)));
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
        if (getLexemeIval(left) == 0 && getLexemeIval(right) == 0) return newLexemeTf(false);
        else return newLexemeTf(true);
    }
    else if (getLexemeType(left) == INTEGER && getLexemeType(right) == BOOL)
    {
        if (getLexemeIval(left) == 0 && getLexemeTf(right) == false) return newLexemeTf(false);
        else return newLexemeTf(true);
    }
    else if (getLexemeType(left) == BOOL && getLexemeType(right) == INTEGER)
    {
        if (getLexemeTf(left) == false && getLexemeIval(right) == 0) return newLexemeTf(false);
        else return newLexemeTf(true);
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
        if (getLexemeIval(left) == 0 || getLexemeIval(right) == 0) return newLexemeTf(false);
        else return newLexemeTf(true);
    }
    else if (getLexemeType(left) == INTEGER && getLexemeType(right) == BOOL)
    {
        if (getLexemeIval(left) == 0 || getLexemeTf(right) == false) return newLexemeTf(false);
        else return newLexemeTf(true);
    }
    else if (getLexemeType(left) == BOOL && getLexemeType(right) == INTEGER)
    {
        if (getLexemeTf(left) == false || getLexemeIval(right) == 0) return newLexemeTf(false);
        else return newLexemeTf(true);
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
    if (getLexemeType(car(tree)) == PLUSPLUS) return evalPlusPlus(car(tree),env);
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
        printf("CAN ONLY DECREMENT NUMBERS -- LINE %d\n", getLineNum(tree));
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
    Lexeme *result;
    while (tree != NULL)
    {
        result = eval(car(tree),env);
        if (getLexemeType(result) == RETURNED) return eval(car(result),env);
        tree = cdr(tree);
    }
    return result;
}


Lexeme *
evalStatement(Lexeme *tree, Lexeme *env)
{
    return eval(car(tree),env);
}


Lexeme *
evalReturnStatement(Lexeme *tree, Lexeme *env)
{
    return cons(RETURNED,eval(car(tree),env),NULL);
}


Lexeme *
evalFuncCall(Lexeme *tree, Lexeme *env)
{
    if (isBuiltIn(getLexemeID(car(tree))))
    {
        Lexeme *optArgs = evalOptArgList(cdr(tree),env);
        return evalBuiltIn(car(tree),optArgs);
    }
    Lexeme *closure = eval(car(tree),env);
    Lexeme *optArgs = evalOptArgList(cdr(tree),env);
    // else if (getLexemeType(closure) == OCLOSURE) evalConstructor(closure,env);
    Lexeme *staticEnv = car(closure);
    Lexeme *params = car(car(cdr(cdr(closure))));
    Lexeme *localEnv = extend(staticEnv,params,optArgs);
    Lexeme *body = cdr(cdr(cdr(closure)));
    Lexeme *result = eval(body,localEnv);
    if (getLexemeType(result) == RETURNED) return car(result);
    else return result;
}

Lexeme *
evalOptArgList(Lexeme *tree, Lexeme *env)
{
    if (car(tree) == NULL) return NULL;
    else return evalArgList(car(tree),env);
}


Lexeme *
evalArgList(Lexeme *tree, Lexeme *env)
{
    if (cdr(tree) != NULL) return cons(GLUE,eval(car(tree),env),evalArgList(cdr(tree),env));
    else return cons(GLUE,eval(car(tree),env),NULL);
}


Lexeme *
evalIfStatement(Lexeme *tree, Lexeme *env)
{
    Lexeme *ifEnv = extend(env,NULL,NULL);
    Lexeme *tf = eval(car(tree),ifEnv);
    if (getLexemeType(tf) != BOOL)
    {
        printf("CONDITIONAL EXPR in IFSTATEMENT MUST BE ONE THAT RETURNS A TYPE BOOL\n");
        exit(1);
    }
    if (getLexemeTf(tf) == true) return eval(car(cdr(tree)),env);
    else return eval(cdr(cdr(tree)),env);
}


Lexeme *
evalOptElse(Lexeme *tree, Lexeme *env)
{
    if (car(tree) != NULL) return eval(car(tree),env);
    else return newLexemeString(NULL,0);
}


Lexeme *
evalElseStatement(Lexeme *tree, Lexeme *env)
{
    if (getLexemeType(car(tree)) == BLOCK)
    {
        Lexeme *elseEnv = extend(env,NULL,NULL);
        return evalBlock(car(tree),elseEnv);
    }
    else return evalIfStatement(car(tree),env);
}


Lexeme *
evalWhileLoop(Lexeme *tree, Lexeme *env)
{
    Lexeme *whileEnv = extend(env,NULL,NULL);
    Lexeme *cond = eval(car(tree),whileEnv);
    Lexeme *whileDone;
    while (getLexemeTf(cond) == true)
    {
        whileDone = eval(cdr(tree),whileEnv);
        cond = eval(car(tree),whileEnv);
    }
    return cond;
}


Lexeme *
evalForLoop(Lexeme *tree, Lexeme *env)
{
    Lexeme *forEnv = extend(env,NULL,NULL);
    eval(car(tree),forEnv);  //varDef
    Lexeme *cond = eval(car(cdr(tree)),forEnv);
    while (getLexemeTf(cond) == true)
    {
        eval(cdr(cdr(cdr(tree))),forEnv);  // block
        eval(car(cdr(cdr(tree))),forEnv);  // expr
        cond = eval(car(cdr(tree)),forEnv);
    }
    return cond;
}


Lexeme *
evalLambdaFunc(Lexeme *tree, Lexeme *env)
{
    return cons(CLOSURE,env,tree);
}


Lexeme *
evalBuiltIn(Lexeme *id, Lexeme *args)
{
    char *funcName = getLexemeID(id);
    if (strcmp(funcName,"newArray") == 0) return evalNewArray(args);
    else if (strcmp(funcName,"setArray") == 0) return evalSetArray(args);
    else if (strcmp(funcName,"getArray") == 0) return evalGetArray(args);
    else if (strcmp(funcName,"getArgCount") == 0) return evalGetArgCount();
    else if (strcmp(funcName,"getArg") == 0) return evalGetArg(args);
    else if (strcmp(funcName,"openFileForReading") == 0) return evalOpenFileForReading(args);
    else if (strcmp(funcName,"readInt") == 0) return evalReadInteger(args);
    else if (strcmp(funcName,"eof") == 0) return evalEOF(args);
    else if (strcmp(funcName,"closeFile") == 0) return evalCloseFile(args);
    else if (strcmp(funcName,"print") == 0) return evalPrint(args);
    else if (strcmp(funcName,"printNewLine") == 0) return evalPrintNewLine(args);
    else return NULL;
}


Lexeme *
evalNewArray(Lexeme *args)
{
    if (car(args) == NULL)
    {
        printf("MUST HAVE ARGUMENT IN NEW ARRAY CALL - LINE %d\n",getLineNum(car(car(args))));
        exit(1);
    }
    if (getLexemeType(car(args)) != INTEGER)
    {
        printf("ARGUMENT MUST BE INTEGER IN NEW ARRAY CALL\n");
        exit(1);
    }
    int size = getLexemeIval(car(args));
    Lexeme *a = newLexemeArray(size);
    return a;
}


Lexeme *
evalSetArray(Lexeme *args)
{
    Lexeme *argCyc = args;
    int count = 0;
    while (argCyc != NULL)
    {
        count++;
        argCyc = cdr(argCyc);
    }
    if (count != 3)
    {
        printf("SET ARRAY NEEDS 3 ARGUMENTS\n");
        exit(1);
    }
    Lexeme *a = car(args);
    Lexeme *i = car(cdr(args));
    if (getLexemeIval(i) >= getArraySize(a))
    {
        printf("ACCESSING ARRAY INDEX OUT OF BOUNDS\n");
        exit(1);
    }
    Lexeme *val = car(cdr(cdr(args)));
    if (getLexemeType(a) != ARRAY || getLexemeType(i) != INTEGER)
    {
        printf("SET ARRAY ARGUMENT MUST BE (ARRAY,INTEGER,VALUE)\n");
        exit(1);
    }
    setArrayVal(a,i,val);
    return val;
}


Lexeme *
evalGetArray(Lexeme *args)
{
    Lexeme *argCyc = args;
    int count = 0;
    while (argCyc != NULL)
    {
        count++;
        argCyc = cdr(argCyc);
    }
    if (count != 2)
    {
        printf("SET ARRAY NEEDS 2 ARGUMENTS\n");
        exit(1);
    }
    Lexeme *a = car(args);
    Lexeme *i = car(cdr(args));
    if (getLexemeIval(i) >= getArraySize(a))
    {
        printf("ACCESSING ARRAY INDEX OUT OF BOUNDS\n");
        exit(1);
    }
    if (getLexemeType(a) != ARRAY || getLexemeType(i) != INTEGER)
    {
        printf("SET ARRAY ARGUMENT MUST BE (ARRAY,INTEGER)\n");
        exit(1);
    }
    Lexeme *result = getArrayVal(a,i);
    if (result == NULL) return newLexemeString("NULL",0);
    return result;
}


Lexeme *
evalGetArgCount()
{
    return newLexemeInt(countCL,0);
}


Lexeme *
evalGetArg(Lexeme *args)
{
    if (getLexemeType(car(args)) != INTEGER)
    {
        printf("PARAMETERS FOR GETARG MUST BE (INTEGER)\n");
        exit(1);
    }
    Lexeme *ind = car(args);
    return newLexemeString(argsCL[getLexemeIval(ind)],0);
}


Lexeme *
evalOpenFileForReading(Lexeme *args)
{
    Lexeme *fileName = car(args);
    if (getLexemeType(fileName) != STRING)
    {
        printf("OPENFILEFORREADING ARGUMENTS MUST BE (STRING)\n");
        exit(1);
    }
    Lexeme *fp = newLexemeFP(getLexemeSval(fileName));
    return fp;
}


Lexeme *
evalReadInteger(Lexeme *args)
{
    if (getLexemeType(car(args)) != FP)
    {
        printf("READINT ARGUMENTS MUST BE (FP)\n");
        exit(1);
    }
    FILE *fp = getLexemeFP(car(args));
    int x;
    fscanf(fp,"%d",&x);
    return newLexemeInt(x,0);
}


Lexeme *
evalEOF(Lexeme *args)
{
    if (getLexemeType(car(args)) != FP)
    {
        printf("EOF ARGUMENTS MUST BE (FP)\n");
        exit(1);
    }
    FILE *fp = getLexemeFP(car(args));
    if (feof(fp)) return newLexemeTf(true);
    else return newLexemeTf(false);
}


Lexeme *
evalCloseFile(Lexeme *args)
{
    if (getLexemeType(car(args)) != FP)
    {
        printf("EOF ARGUMENTS MUST BE (FP)\n");
        exit(1);
    }
    FILE *fp = getLexemeFP(car(args));
    fclose(fp);
    return newLexemeTf(true);
}


Lexeme *
evalPrint(Lexeme *args)
{
    if (cdr(args) != NULL)
    {
        printf("CAN ONLY PASS ONE ARUMENT TO PRINT\n");
    }
    if (!(getLexemeType(car(args)) == INTEGER || getLexemeType(car(args)) == REAL || getLexemeType(car(args)) == BOOL || getLexemeType(car(args)) == STRING))
    {
        printf("CAN ONLY PRINT A STRING OR NUMBER OR BOOL\n");
        exit(1);
    }
    if (getLexemeType(car(args)) == INTEGER) printf("%d",getLexemeIval(car(args)));
    else if (getLexemeType(car(args)) == REAL) printf("%lf",getLexemeRval(car(args)));
    else if (getLexemeType(car(args)) == BOOL)
    {
        if (getLexemeTf(car(args)) == true) printf("true");
        else printf("false");
    }
    else printf("%s",getLexemeSval(car(args)));
    return newLexemeTf(true);
}


Lexeme *
evalPrintNewLine()
{
    printf("\n");
    return newLexemeTf(true);
}