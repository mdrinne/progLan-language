/*
 * Matthew Rinne
 * pretyy-printing.c
 * cs403 Programming Languages
 */


#include "pretty-printing.h"
#include "lexeme.h"
#include "types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


int ppline = 1;
int tabs = 0;


void
fixLine(Lexeme *cur)
{
    while (ppline < getLineNum(cur))
    {
        printf("\n");
        ppline++;
    }
}


void
fixLineElse(Lexeme *cur)
{
    while (ppline < (getLineNum(cur) - 3))
    {
        printf("\n");
        ppline++;
    }
}


void
pp(Lexeme *tree)
{
    if (getLexemeType(tree) == INTEGER)
    {
        fixLine(tree);
        printf("%d", getLexemeIval(tree));
    }
    else if (getLexemeType(tree) == REAL)
    {
        fixLine(tree);
        printf("%lf", getLexemeRval(tree));
    }
    else if (getLexemeType(tree) == STRING) 
    {
        fixLine(tree);
        printf("'");
        printf("%s", getLexemeSval(tree));
        printf("'");
    }
    else if (getLexemeType(tree) == ID)
    {
        fixLine(tree);
        printf("%s", getLexemeID(tree));
    }
    else if (getLexemeType(tree) == PLUS) printf("+");
    else if (getLexemeType(tree) == TIMES) printf("*");
    else if (getLexemeType(tree) == DIVIDES) printf("/");
    else if (getLexemeType(tree) == MINUS) printf("-");
    else if (getLexemeType(tree) == ASSIGN) printf("=");
    else if (getLexemeType(tree) == GREATERTHAN) printf(">");
    else if (getLexemeType(tree) == GREATERTHANOREQUAL) printf(">=");
    else if (getLexemeType(tree) == LESSTHAN) printf("<");
    else if (getLexemeType(tree) == LESSTHANOREQUAL) printf("<=");
    else if (getLexemeType(tree) == NOTEQUALS) printf("!=");
    else if (getLexemeType(tree) == PLUSEQUALS) printf("+=");
    else if (getLexemeType(tree) == MINUSEQUALS) printf("-=");
    else if (getLexemeType(tree) == TIMESEQUALS) printf("*=");
    else if (getLexemeType(tree) == DIVIDESEQUALS) printf("/=");
    else if (getLexemeType(tree) == PLUSPLUS) printf("++");
    else if (getLexemeType(tree) == MINUSMINUS) printf("--");
    else if (getLexemeType(tree) == NOT) printf("!");
    else if (getLexemeType(tree) == MOD) printf("%%");
    else if (getLexemeType(tree) == EQUALEQUAL) printf("==");
    else if (getLexemeType(tree) == ANDAND) printf("&&");
    else if (getLexemeType(tree) == OROR) printf("||");
    else if (getLexemeType(tree) == PLUSPLUS) printf("++");
    else if (getLexemeType(tree) == MINUSMINUS) printf("--");
            else if (getLexemeType(tree) == PROGRAM) printProgram(tree);
            else if (getLexemeType(tree) == DEFS) printDefs(tree);
            else if (getLexemeType(tree) == DEF) printDef(tree);
            else if (getLexemeType(tree) == OPTDEFS) printOptDefs(tree);
    // else if (getLexemeType(tree) == INCLUDES) printIncludes(tree);
    // else if (getLexemeType(tree) == INCLUDESTATEMENT) printInclude(tree);
    // else if (getLexemeType(tree) == OPTINCLUDES) printOptIncludes(tree);
            else if (getLexemeType(tree) == FUNCDEF) printFuncDef(tree);
    else if (getLexemeType(tree) == PARAMLIST) printParamList(tree);
    else if (getLexemeType(tree) == OPTPARAMLIST) printOptParamList(tree);
    else if (getLexemeType(tree) == MAINFUNC) printMainFunc(tree);
            else if (getLexemeType(tree) == VARDEF) printVarDef(tree);
            else if (getLexemeType(tree) == OPTINIT) printOptInit(tree);
            else if (getLexemeType(tree) == UNARY) printUnary(tree);
            else if (getLexemeType(tree) == UMINUS) printUminus(tree);
    else if (getLexemeType(tree) == OPER) printOper(tree);
            else if (getLexemeType(tree) == EXPR) printExpr(tree);
            else if (getLexemeType(tree) == OPTEXPR) printOptExpr(tree);
    else if (getLexemeType(tree) == ARGLIST) printArgList(tree);
    else if (getLexemeType(tree) == OPTARGLIST) printOptArgList(tree);
    else if (getLexemeType(tree) == FUNCCALL) printFuncCall(tree);
    else if (getLexemeType(tree) == IFSTATEMENT) printIfStatement(tree);
    else if (getLexemeType(tree) == ELSESTATEMENT) printElseStatement(tree);
    else if (getLexemeType(tree) == OPTELSE) printOptElse(tree);
    else if (getLexemeType(tree) == WHILELOOP) printWhileLoop(tree);
    else if (getLexemeType(tree) == RETURNSTATEMENT) printReturnStatement(tree);
    else if (getLexemeType(tree) == FORLOOP) printForLoop(tree);
    else if (getLexemeType(tree) == CHECKOPER) printCheckOper(tree);
    else if (getLexemeType(tree) == SELFOP) printSelfOp(tree);
    else if (getLexemeType(tree) == DOUBLESELFOP) printDoubleSelfOp(tree);
    else if (getLexemeType(tree) == BLOCK) printBlock(tree);
    else if (getLexemeType(tree) == STATEMENTS) printStatements(tree);
    else if (getLexemeType(tree) == STATEMENT) printStatement(tree);
    else if (tree != NULL) printf("not listed TYPE\n");
}


void
printProgram(Lexeme *tree)
{
    // pp(car(tree));  // INCLUDES
    // printf("optdefs\n");
    pp(car(tree));  // OPTDEFS
    // printf("main\n");
    pp(cdr(tree));  // MAINFUNC
}


void
printDefs(Lexeme *tree)
{
    while (tree != NULL)
    {
        pp(car(tree));  // DEF
        tree = cdr(tree);  // DEFS or NULL
    }
}


void
printDef(Lexeme *tree)
{
    pp(car(tree));  // VARDEF or FUNCDEF
}


void
printOptDefs(Lexeme *tree)
{
    if (car(tree) != NULL)  pp(car(tree));
}


// void
// printIncludes(Lexeme *tree)
// {
//     while (tree != NULL)
//     {
//         pp(car(tree));  // INCLUDE
//         tree = cdr(tree);  // INCLUDES or NULL
//     }
// }


// void
// printInclude(Lexeme *tree)
// {
//     fixLine(car(tree));
//     printf("#include \"");
//     pp(car(tree));  // ID (filename)
//     printf(".");
//     pp(cdr(tree));  // ID (file extension)
//     printf("\"");
// }


// void
// printOptIncludes(Lexeme *tree)
// {
//     if (car(tree) != NULL) pp(car(tree));
// }


void
printFuncDef(Lexeme *tree)
{
    fixLine(car(tree));
    printf("func ");
    pp(car(tree));  // ID (funcName)
    printf("(");
    pp(car(cdr(tree)));  // OPTPARAMLIST
    printf(")");
    pp(cdr(cdr(tree)));  // BLOCK
}


void
printParamList(Lexeme *tree)
{
    while (tree != NULL)
    {
        fixLine(car(tree));
        printf("var ");
        pp(car(tree));  // ID
        if (cdr(tree) != NULL) printf(", ");
        tree = cdr(tree);  // PARAMLIST
    }
}


void
printOptParamList(Lexeme *tree)
{
    if (car(tree) != NULL) pp(car(tree));  // PARAMLIST
}


void
printMainFunc(Lexeme *tree)
{
    fixLine(car(tree));
    printf("main (");
    pp(car(tree));  // OPTPARAMLIST
    printf(")");
    pp(cdr(tree));  // BLOCK
}


void
printVarDef(Lexeme *tree)
{
    fixLine(car(tree));
    printf("var ");
    pp(car(tree));  // ID (varName)
    pp(cdr(tree));  // OPTINIT
    printf("$");
}


void
printOptInit(Lexeme *tree)
{
    if (car(tree) != NULL)
    {
        printf(" = ");
        pp(car(tree));  // EXPR
    }
}


void
printUnary(Lexeme *tree)
{
    pp(car(tree));  // ID or INTEGER or REAL or STRING or UMINUS
}


void
printUminus(Lexeme *tree)
{
    fixLine(car(tree));
    printf("-");
    pp(car(tree));  // UNARY
}


void
printOper(Lexeme *tree)
{
    pp(car(tree));  // PLUS or MINUS or TIMES or ...
}


void
printExpr(Lexeme *tree)
{
    pp(car(tree));  // EXPR or FUNCCALL
    if (car(cdr(tree)) != NULL)
    {
        if (getLexemeType(car(cdr(tree))) == DOUBLESELFOP) pp(car(cdr(tree)));  // DOUBLESELFOP
        else if (getLexemeType(car(cdr(tree))) == OPER)
        {
            printf(" ");
            pp(car(cdr(tree)));  // OPER
            printf(" ");
            pp(cdr(cdr(tree)));  // EXPR
        }
    }
}


void
printOptExpr(Lexeme *tree)
{
    if (car(tree) != NULL) pp(car(tree));  // EXPR
}


void
printArgList(Lexeme *tree)
{
    while (tree != NULL)
    {
        pp(car(tree));  // EXPR
        tree = cdr(tree);  // ARGLIST
    }
}
void
printOptArgList(Lexeme *tree)
{
    if (car(tree) != NULL) pp(car(tree));
}


void
printFuncCall(Lexeme *tree)
{
    fixLine(car(tree));
    printf("call ");
    pp(car(tree));  // ID (funcName)
    printf("(");
    pp(cdr(tree));  // OPTPARANLIST
    printf(")");
}


void
printIfStatement(Lexeme *tree)
{
    fixLine(car(tree));
    printf("if (");
    pp(car(tree));  // EXPR
    printf(")");
    pp(car(cdr(tree)));  // BLOCK
    pp(cdr(cdr(tree)));  // OPTELSE
}


void
printElseStatement(Lexeme *tree)
{
    if (getLexemeType(car(tree)) == IFSTATEMENT)
    {
        fixLine(car(car(tree)));
    }
    else
    {
        fixLineElse(car(car(tree)));
    }
    printf("else ");
    pp(car(tree));  // BLOCK or IFSTATEMENT
}


void
printOptElse(Lexeme *tree)
{
    if (car(tree) != NULL) pp(car(tree));  // ELSESTATEMENT
}


void
printWhileLoop(Lexeme *tree)
{
    fixLine(car(tree));
    printf("while (");
    pp(car(tree));  // EXPR
    printf(")");
    pp(cdr(tree));  // BLOCK
}


void
printReturnStatement(Lexeme *tree)
{
    fixLine(car(tree));
    printf("return");
    if (car(car(tree)) != NULL)
    {
        printf(" ");
        pp(car(tree));  // OPTEXPR
    }
    printf("$");
}


void
printForLoop(Lexeme *tree)
{
    fixLine(car(tree));
    printf("for (var ");
    pp(car(tree));  // ID
    printf("=");
    pp(car(cdr(tree)));  // INTEGER
    printf("; ");
    pp(car(cdr(cdr(tree))));  // ID
    pp(car(cdr(cdr(cdr(tree)))));  // CHECKOPER
    pp(car(cdr(cdr(cdr(cdr(tree))))));  // INTEGER
    printf("; ");
    pp(car(cdr(cdr(cdr(cdr(cdr(tree)))))));  // ID
    pp(car(cdr(cdr(cdr(cdr(cdr(cdr(tree))))))));  // SELFOP
    printf(")");
    pp(cdr(cdr(cdr(cdr(cdr(cdr(cdr(tree))))))));  // BLOCK
}


void
printCheckOper(Lexeme *tree)
{
    pp(car(tree));  // GREATETHAN or GREATERTHANOREQUAL or LESSTHAN or LESSTHANOREQUAL
}


void
printSelfOp(Lexeme *tree)
{
    if (getLexemeType(car(tree)) == DOUBLESELFOP) pp(car(tree));  // DOUBLESELFOP
    else
    {
        pp(car(tree));  // PLUSEQUALS or MINUSEQUALS ...
        pp(cdr(tree));  // INTEGER
    }
}


void
printDoubleSelfOp(Lexeme *tree)
{
    pp(car(tree));  // PLUSPLUS or MINUSMINUS
}


void
printBlock(Lexeme *tree)
{
    printf("\n");
    printf("{\n");
    ppline += 2;
    pp(car(tree));  // STATEMENTS
    printf("\n");
    printf("}\n");
    ppline += 2;
}


void
printStatements(Lexeme *tree)
{
    while (tree != NULL)
    {
        pp(car(tree));  // STATEMENT
        tree = cdr(tree);  // STATEMENTS or NULL
    }
}


void
printStatement(Lexeme *tree)
{
    if (getLexemeType(car(tree)) == EXPR || getLexemeType(car(tree)) == FUNCCALL)
    {
        pp(car(tree));  // EXPR or FUNCCALL
        printf("$");
    }
    else pp(car(tree));  // VARDEF or FUNCDEF or ...
}