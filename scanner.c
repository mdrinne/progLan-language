/*
 * Matthew Rinne
 * scanner.c
 * cs403 Programming Languages
 */


#include "lexer.h"
#include "types.h"
#include <stdio.h>
#include <stdlib.h>



int main(int argc, char const *argv[])
{
    FILE *fp = fopen(argv[argc-1], "r");
    getFile(fp);
    Lexeme *token = lex();
    while (getLexemeType(token) != END_OF_FILE)
    {   
        if (getLexemeType(token) == INTEGER) printf("%s %d\n",getLexemeType(token),getLexemeIval(token));
        else if (getLexemeType(token) == REAL) printf("%s %lf\n",getLexemeType(token),getLexemeRval(token));
        else if (getLexemeType(token) == STRING) printf("%s %s\n",getLexemeType(token),getLexemeSval(token));
        else if (getLexemeType(token) == ID) printf("%s %s\n",getLexemeType(token),getLexemeID(token));
        else printf("%s\n", getLexemeType(token));
        token = lex();
    }
    fclose(fp);
    return 0;
}
