/*
 * Matthew Rinne
 * parser.c
 * cs403 Programming Languages
 */

#include "lexer.h"
#include "lexeme.h"
#include "types.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>


Lexeme *current;
Lexeme *prev;
int legal = 1;


Lexeme *
advance()
{
    prev = current;
    current = lex();
    return prev;
}


int
check(char *type)
{
    return getLexemeType(current) == type;
}


Lexeme *
match(char *type)
{
    if (check(type)) return advance();
    legal = 0;
    printf("SYNTAX ERROR: expected %s, got %s - line %d\n",type,getLexemeType(current),getLine());
    printf("illegal\n");
    exit(0);
}


int
getLegal()
{
    return legal;
}


Lexeme *
cons(char *type, Lexeme *left, Lexeme *right)
{
    Lexeme *n = newLexeme(type, getLine());
    set_car(n,left);
    set_cdr(n,right);
    return n;
}


Lexeme *
program()
{
    Lexeme *d, *m;
    // i = includes();
    d = optDefs();
    m = mainFunc();
    return cons(PROGRAM,d,m);
}


Lexeme *
defs()
{
    Lexeme *d, *ds;
    d = def();
    if (defsPending()) ds = optDefs();
    else ds = NULL;
    return cons(DEFS,d,ds);
}


int
defsPending()
{
    return defPending();
}


Lexeme *
def()
{
    Lexeme *d;
    if (varDefPending()) d = varDef();
    else d = funcDef();
    return cons(DEF,d,NULL);
}


int
defPending()
{
    return varDefPending() || funcDefPending();
}


Lexeme *
optDefs()
{
    Lexeme *d;
    if (defsPending()) d = defs();
    else d = NULL;
    return cons(OPTDEFS,d,NULL);
}


int
optDefsPending()
{
    return defsPending();
}


// Lexeme *
// includes()
// {
//     Lexeme *i, *is;
//     i = includeStatement();
//     if (includesPending()) is = includes();
//     else is = NULL;
//     return cons(INCLUDES,i,is);
// }


// int
// includesPending()
// {
//     return check(HASH);
// }


// Lexeme *
// includeStatement()
// {
//     Lexeme *fn, *fex;
//     match(HASH);
//     match(INCLUDE);
//     match(QUOTE);
//     fn = match(ID);
//     match(DOT);
//     fex = match(ID);
//     match(QUOTE);
//     return cons(INCLUDESTATEMENT,fn,fex);
// }


// Lexeme *
// optIncludes()
// {
//     Lexeme *i;
//     if (includesPending()) i = includes();
//     else i = NULL;
//     return cons(OPTINCLUDES,i,NULL);
// }


Lexeme *
funcDef()
{
    Lexeme *fn, *op, *b;
    match(FUNCTION);
    fn = match(ID);
    match(OPAREN);
    op = optParamList();
    match(CPAREN);
    b = block();
    return cons(FUNCDEF,fn,cons(GLUE,op,b));
}


int
funcDefPending()
{
    return check(FUNCTION);
}


Lexeme *
parameterList()
{
    Lexeme *i, *p;
    match(VAR);
    i = match(ID);
    if (check(COMMA))
    {
        match(COMMA);
        p = parameterList();
    }
    else p = NULL;
    return cons(PARAMLIST,i,p);
}


int
parameterListPending()
{
    return check(VAR);
}


Lexeme *
optParamList()
{
    Lexeme *p;
    if (parameterListPending()) p = parameterList();
    else p = NULL;
    return cons(OPTPARAMLIST,p,NULL);
}


Lexeme *
mainFunc()
{
    Lexeme *op, *b;
    match(MAINFUNC);
    match(OPAREN);
    op = optParamList();
    match(CPAREN);
    b = block();
    return cons(MAINFUNC,op,b);
}


Lexeme *
varDef()
{
    Lexeme *vn, *oi;
    match(VAR);
    vn = match(ID);
    oi = optInit();
    match(MONEY);
    return cons(VARDEF,vn,oi);
}


int
varDefPending()
{
    return check(VAR);
}


Lexeme *
optInit()
{
    Lexeme *e;
    if (check(ASSIGN))
    {
        match(ASSIGN);
        e = expr();
    }
    else e = NULL;
    return cons(OPTINIT,e,NULL);
}


Lexeme *
unary()
{
    Lexeme *u;
    // if (check(MINUS))
    // {
    //     u = uminus();
    // }
    if (check(INTEGER)) u = match(INTEGER);
    else if (check(REAL)) u = match(REAL);
    else if (check(STRING)) u = match(STRING);
    else u = match(ID);
    return cons(UNARY,u,NULL);
}


int
unaryPending()
{
    return check(INTEGER) || check(REAL) || check(STRING) || check(ID);
}


// Lexeme *
// uminus()
// {
//     Lexeme *u;
//     match(MINUS);
//     u = unary();
//     return cons(UMINUS,u,NULL);
// }


Lexeme *
oper()
{
    Lexeme *o;
    if (check(PLUS)) o = match(PLUS);
    else if (check(TIMES)) o = match(TIMES);
    else if (check(DIVIDES)) o = match(DIVIDES);
    else if (check(MINUS)) o = match(MINUS);
    else if (check(ASSIGN)) o = match(ASSIGN);
    else if (check(GREATERTHAN)) o = match(GREATERTHAN);
    else if (check(GREATERTHANOREQUAL)) o = match(GREATERTHANOREQUAL);
    else if (check(LESSTHAN)) o = match(LESSTHAN);
    else if (check(LESSTHANOREQUAL)) o = match(LESSTHANOREQUAL);
    else if (check(MOD)) o = match(MOD);
    else if (check(ANDAND)) o = match(ANDAND);
    else if (check(OROR)) o = match(OROR);
    else if (check(EQUALEQUAL)) o = match(EQUALEQUAL);
    else o = match(NOTEQUALS);
    return cons(OPER,o,NULL);
}


int
operPending()
{
    return check(PLUS) || check(TIMES) || check(DIVIDES) || check(MINUS)
        || check(ASSIGN) || check(GREATERTHAN) || check(GREATERTHANOREQUAL)
        || check(LESSTHAN) || check(LESSTHANOREQUAL) || check(NOTEQUALS)
        || check(MOD) || check(ANDAND) || check(OROR) || check(EQUALEQUAL);
}


Lexeme *
expr()
{
    Lexeme *uf, *op, *ex;
    if (unaryPending())
    {
        uf = unary();
        if (doubleSelfOpPending())
        {
            op = doubleSelfOp();
            ex = NULL;
        }
        else if (operPending())
        {
            op = oper();
            ex = expr();
        }
        else
        {
            op = NULL;
            ex = NULL;
        }
    }
    else
    {
        uf = funcCall();
        if (operPending())
        {
            op = oper();
            ex = expr();
        }
        else
        {
            op = NULL;
            ex = NULL;
        }
    }
    return cons(EXPR,uf,cons(GLUE,op,ex));
}


int
exprPending()
{
    return unaryPending() || funcCallPending();
}


Lexeme *
optExpr()
{
    Lexeme *ex;
    if (exprPending()) ex = expr();
    else ex = NULL;
    return cons(OPTEXPR,ex,NULL);
}


Lexeme * 
argList()
{
    Lexeme *ex, *arg;
    ex = expr();
    if (check(COMMA))
    {
        match(COMMA);
        arg = argList();
    }
    else arg = NULL;
    return cons(ARGLIST,ex,arg);
}


int
argListPending()
{
    return exprPending();
}


Lexeme *
optArgList()
{
    Lexeme *arg;
    if (argListPending()) arg = argList();
    else arg = NULL;
    return cons(OPTARGLIST,arg,NULL);
}


Lexeme *
funcCall()
{
    Lexeme *fn, *oarg;
    match(CALL);
    fn = match(ID);
    match(OPAREN);
    oarg = optArgList();
    match(CPAREN);
    return cons(FUNCCALL,fn,oarg);
}


int
funcCallPending()
{
    return check(CALL);
}


Lexeme *
ifStatement()
{
    Lexeme *ex, *b, *oe;
    match(IF);
    match(OPAREN);
    ex = expr();
    match(CPAREN);
    b = block();
    oe = optElse();
    return cons(IFSTATEMENT,ex,cons(GLUE,b,oe));
}


int
ifStatementPending()
{
    return check(IF);
}


Lexeme *
elseStatement()
{
    Lexeme *bif;
    match(ELSE);
    if (ifStatementPending()) bif = ifStatement();
    else bif = block();
    return cons(ELSESTATEMENT,bif,NULL);
}


int
elseStatementPending()
{
    return check(ELSE);
}


Lexeme *
optElse()
{
    Lexeme *e;
    if (elseStatementPending()) e = elseStatement();
    else e = NULL;
    return cons(OPTELSE,e,NULL);
}


Lexeme *
whileLoop()
{
    Lexeme *ex, *b;
    match(WHILE);
    match(OPAREN);
    ex = expr();
    match(CPAREN);
    b = block();
    return cons(WHILELOOP,ex,b);
}


int
whileLoopPending()
{
    return check(WHILE);
}


Lexeme *
returnStatement()
{
    Lexeme *oe;
    match(RETURN);
    oe = optExpr();
    match(MONEY);
    return cons(RETURNSTATEMENT,oe,NULL);
}


int
returnStatementPending()
{
    return check(RETURN);
}


Lexeme *
forLoop()
{
    Lexeme *i1, *int1, *i2, *co, *int2, *i3, *so, *b;
    match(FOR);
    match(OPAREN);
    match(VAR);
    i1 = match(ID);
    match(ASSIGN);
    int1 = match(INTEGER);
    match(SEMI);
    i2 = match(ID);
    co = checkOper();
    int2 = match(INTEGER);
    match(SEMI);
    i3 = match(ID);
    so = selfOp();
    match(CPAREN);
    b = block();
    return cons(FORLOOP,i1,cons(GLUE,int1,cons(GLUE,i2,cons(GLUE,co,cons(GLUE,int2,cons(GLUE,i3,cons(GLUE,so,b)))))));
}


int
forLoopPending()
{
    return check(FOR);
}


Lexeme *
checkOper()
{
    Lexeme *co;
    if (check(GREATERTHAN)) co = match(GREATERTHAN);
    else if (check(GREATERTHAN)) co = match(GREATERTHANOREQUAL);
    else if (check(LESSTHAN)) co = match(LESSTHAN);
    else co = match(LESSTHANOREQUAL);
    return cons(CHECKOPER,co,NULL);
}


Lexeme *
selfOp()
{
    Lexeme *op, *i;
    if (check(PLUSEQUALS))
    {
        op = match(PLUSEQUALS);
        i = match(INTEGER);
    }
    else if (check(MINUSEQUALS))
    {
        op = match(MINUSEQUALS);
        i = match(INTEGER);
    }
    else if (check(TIMESEQUALS))
    {
        op = match(TIMESEQUALS);
        i = match(INTEGER);
    }
    else if (check(DIVIDESEQUALS))
    {
        op = match(DIVIDESEQUALS);
        i = match(INTEGER);
    }
    else
    {
        op = doubleSelfOp();
        i = NULL;
    }
    return cons(SELFOP,op,i);
}


Lexeme *
doubleSelfOp()
{
    Lexeme *dso;
    if (check(PLUSPLUS)) dso = match(PLUSPLUS);
    else dso = match(MINUSMINUS);
    return cons(DOUBLESELFOP,dso,NULL);
}


int
doubleSelfOpPending()
{
    return check(PLUSPLUS) || check(MINUSMINUS);
}


Lexeme *
block()
{
    Lexeme *s;
    match(OBRACE);
    s = statements();
    match(CBRACE);
    return cons(BLOCK,s,NULL);
}


Lexeme *
statements()
{
    Lexeme *st, *sts;
    st = statement();
    if (statementsPending()) sts = statements();
    else sts = NULL;
    return cons(STATEMENTS,st,sts);
}


int
statementsPending()
{
    return statementPending();
}


Lexeme *
statement()
{
    Lexeme *s;
    if (exprPending())
    {
       s = expr();
        match(MONEY);
    }
    else if (varDefPending()) s = varDef();
    else if (funcDefPending()) s = funcDef();
    else if (ifStatementPending()) s = ifStatement();
    else if (whileLoopPending()) s = whileLoop();
    else if (forLoopPending()) s = forLoop();
    else if (returnStatementPending()) s = returnStatement();
    else s = funcCall();
    return cons(STATEMENT,s,NULL);
}


int
statementPending()
{
    return exprPending() ||  varDefPending() || funcDefPending() || ifStatementPending()
        || whileLoopPending() || forLoopPending() || returnStatementPending()
        || funcCallPending();
}