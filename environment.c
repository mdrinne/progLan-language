/*
 * Matthew Rinne
 * environment.c
 * cs403 Programming Languages
 */


#include "lexeme.h"
#include "types.h"
#include "environment.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


Lexeme *
insertEnv(Lexeme *env, Lexeme *id, Lexeme * val)
{
    set_car(car(env),cons(JOIN,id,car(car(env))));
    set_cdr(car(env),cons(JOIN,val,cdr(car(env))));
    return val;
}


Lexeme *
lookup(Lexeme *id, Lexeme *env)
{
    Lexeme *vars;
    Lexeme *vals;
    while (env != NULL)
    {
        vars = car(car(env));
        vals = cdr(car(env));
        while (vars != NULL)
        {
            if (strcmp(getLexemeID(id),getLexemeID(car(vars))) == 0)
            {
                return car(vals);
            }
            vars = cdr(vars);
            vals = cdr(vals);
        }
        env = cdr(env);
    }
    printf("VARIABLE %s UNDEFINED\n",getLexemeID(id));
    exit(1);
}


Lexeme *
update(Lexeme *env, Lexeme *id, Lexeme *val)
{
    Lexeme *vars;
    Lexeme *vals;
    while (env != NULL)
    {
        vars = car(car(env));
        vals = cdr(car(env));
        while (vars != NULL)
        {
            if (strcmp(getLexemeID(id),getLexemeID(car(vars))) == 0)
            {
                set_car(vals, val);
                return val;
            }
            vars = cdr(vars);
            vals = cdr(vals);
        }
        env = cdr(env);
    }
    printf("VAR %s IS UNDEFINED\n",getLexemeID(id));
    return NULL;
}


Lexeme *
newEnv()
{
    return cons(ENV,cons(TABLE,NULL,NULL),NULL);
}


Lexeme *
extend(Lexeme *env, Lexeme *vars, Lexeme *vals)
{
    return cons(ENV,cons(TABLE,vars,vals),env);
}


void
displayEnv(Lexeme *env)
{
    int first = 1;
    Lexeme *vars;
    Lexeme *vals;
    while (env != NULL)
    {
        vars = car(car(env));
        vals = cdr(car(env));
        if (first)
        {
            printf("local\n");
            first = 0;
        }
        else if (cdr(env) == NULL) printf("global\n");
        else printf("non-local\n");
        printf("==============\n");

        while (vars != NULL)
        {
            printf("%s: ", getLexemeID(car(vars)));
            if (getLexemeType(car(vals)) == INTEGER) printf("%d\n",getLexemeIval(car(vals)));
            else if (getLexemeType(car(vals)) == REAL) printf("%lf\n",getLexemeRval(car(vals)));
            else if (getLexemeType(car(vals)) == STRING) printf("%s\n",getLexemeSval(car(vals)));
            else if (getLexemeType(car(vals)) == CLOSURE) printf("function\n");
            else if (getLexemeType(car(vals)) == BOOL && getLexemeTf(car(vals)) == true) printf("true\n");
            else if (getLexemeType(car(vals)) == BOOL && getLexemeTf(car(vals)) == false) printf("false\n");
            else printf("%s\n",getLexemeType(vals));
            vars = cdr(vars);
            vals = cdr(vals);
        }
        printf("\n\n");
        env = cdr(env);
    }
}