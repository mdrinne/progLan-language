/*
 * Matthew Rinne
 * lexical-analyzer.c
 * cs403 Programming Languages
 */


#include "lexer.h"
#include "types.h"
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>


FILE *fp;
char pushbackChar;
int charHasBeenPushed = 0;
int line = 1;
int success;


void
skipComment()
{
    char ch = readChar(fp);
    while (ch != '?')
    {
        ch = readChar(fp);
        if (ch == 10)
        {
            line++;
        }
    }
}


int
isWhiteSpace(char ch)
{
    if (ch == 10)
    {
        line++;
        return 1;
    }
    else if (ch >= 64) return 0;
    else if (ch == 0) return 0;
    else if (ch >= 33 && ch <= 62) return 0;
    else if (ch == '?')
    {
        skipComment();
        return 1;
    }
    else 
    {
        return 1;
    }
}


void
skipWhiteSpace()
{
    char ch = readChar(fp);
    while(isWhiteSpace(ch))
    {
            ch = readChar(fp);
    }
    pushback(ch);
    return;
}


void
pushback(char ch)
{
    if (charHasBeenPushed)
    {
        printf("FATAL ERROR: too many pushbacks\n");
        exit(1);
    }
    charHasBeenPushed = 1;
    pushbackChar = ch;
}


char
readChar()
{
    if (charHasBeenPushed)
    {
        charHasBeenPushed = 0;
        return pushbackChar;
    }
    else if (feof(fp)) return 0;
    else
    {
        char ch = fgetc(fp);
        return ch;
    }
}


double
toReal(char *buf)
{
    return strtod(buf, NULL);
}


int
toInt(char *buf)
{
    return atoi(buf);
}


Lexeme *
lexNumber()
{
    int len = 16;
    int index = 0;
    int real = 0;
    char *buffer = malloc(sizeof(char) * len+1);
    char ch;
    ch = readChar();
    while (ch != EOF && (isdigit(ch) || ch == '.'))
    {
        if (index == len)
        {
            len *= 2;
            buffer = realloc(buffer,sizeof(char) * len+1);
        }
        buffer[index] = ch;
        index++;
        buffer[index] = '\0';
        if (ch == '.' && real)
        {
            printf("FATAL ERROR: BAD_NUMBER, too many \'.\' - LINE %d\n",line);
            exit(1);
        }
        if (ch == '.') real = 1;
        ch = readChar();
    }
    pushback(ch);
    if (real) return newLexemeReal(toReal(buffer),getLine());
    else return newLexemeInt(toInt(buffer),getLine());
}


Lexeme *
lexVarOrKeyword()
{
    int len = 16;
    int index = 0;
    char *buffer = malloc(sizeof(char) * len+1);
    char ch;
    ch = readChar();
    while (ch != EOF && (isalpha(ch) || isdigit(ch) || ch == '_'))
    {
        if (index == len)
        {
            len *= 2;
            buffer = realloc(buffer,sizeof(char) * len+1);
        }
        buffer[index] = ch;
        index++;
        buffer[index] = '\0';
        ch = readChar();
    }
    pushback(ch);
    if (strcmp(buffer, "if") == 0) return newLexeme(IF,line);
    else if (strcmp(buffer, "else") == 0) return newLexeme(ELSE,line);
    else if (strcmp(buffer, "while") == 0) return newLexeme(WHILE,line);
    else if (strcmp(buffer, "main") == 0) return newLexeme(MAIN,line);
    else if (strcmp(buffer, "include") == 0) return newLexeme(INCLUDE,line);
    else if (strcmp(buffer, "func") == 0) return newLexeme(FUNCTION,line);
    else if (strcmp(buffer, "return") == 0) return newLexeme(RETURN,line);
    else if (strcmp(buffer, "var") == 0) return newLexeme(VAR,line);
    else if (strcmp(buffer, "for") == 0) return newLexeme(FOR,line);
    else if (strcmp(buffer, "call") == 0) return newLexeme(CALL,line);
    else return newLexemeID(buffer,getLine());
}


Lexeme *
lexString()
{
    int len = 16;
    int index = 0;
    char *buffer = malloc(sizeof(char) * len+1);
    char ch;
    ch = readChar();
    char prev;
    while (ch != EOF && (ch != 39 || (ch == 39 && prev == 92)))
    {
        if (index == len)
        {
            len *= 2;
            buffer = realloc(buffer, sizeof(char) * len+1);
        }
        buffer[index] = ch;
        index++;
        buffer[index] = '\0';
        prev = ch;
        ch = readChar();
    }
    return newLexemeString(buffer,line);
}


Lexeme *
lex()
{
    char ch;
    skipWhiteSpace();
    ch = readChar();
    
    switch(ch)
    {
        case '{': return newLexeme(OBRACE,line);
        case '}': return newLexeme(CBRACE,line);
        case '(': return newLexeme(OPAREN,line);
        case ')': return newLexeme(CPAREN,line);
        case '[': return newLexeme(OBRACKET,line);
        case ']': return newLexeme(CBRACKET,line);
        case ',': return newLexeme(COMMA,line);
        case ';': return newLexeme(SEMI,line);
        case '$': return newLexeme(MONEY,line);
        case '%': return newLexeme(MOD,line);
        case '@': return newLexeme(AT,line);
        case '.': return newLexeme(DOT,line);
        case '#': return newLexeme(HASH,line);
        case '?': return newLexeme(QUESTION,line);
        case ':': return newLexeme(COLON,line);
        case '"': return newLexeme(QUOTE,line);

        default:
            if (ch == '&')
            {
                ch = readChar();
                if (ch == '&') return newLexeme(ANDAND,line);
                else
                {
                    pushback(ch);
                    return newLexeme(AMPERSAND,line);
                }
            }
            else if (ch == '|')
            {
                ch = readChar();
                if (ch == '|') return newLexeme(OROR,line);
                else
                {
                    pushback(ch);
                    return newLexeme(BAR,line);
                }
            }
            else if (ch == '<')
            {
                ch = readChar();
                if (ch == '=') return newLexeme(LESSTHANOREQUAL,line);
                else
                {
                    pushback(ch);
                    return newLexeme(LESSTHAN,line);
                }
            }
            else if (ch == '>')
            {
                ch = readChar();
                if (ch == '=') return newLexeme(GREATERTHANOREQUAL,line);
                else
                {
                    pushback(ch);
                    return newLexeme(GREATERTHAN,line);
                }
            }
            else if (ch == '+')
            {
                ch = readChar();
                if (ch == '+') return newLexeme(PLUSPLUS,line);
                else if (ch == '=') return newLexeme(PLUSEQUALS,line);
                else
                {
                    pushback(ch);
                    return newLexeme(PLUS,line);
                }
            }
            else if (ch == '-')
            {
                ch = readChar();
                if (ch == '-') return newLexeme(MINUSMINUS,line);
                else if (ch == '=') return newLexeme(MINUSEQUALS,line);
                else
                {
                    pushback(ch);
                    return newLexeme(MINUS,line);
                }
            }
            else if (ch == '*')
            {
                ch = readChar();
                if (ch == '=') return newLexeme(TIMESEQUALS,line);
                else
                {
                    pushback(ch);
                    return newLexeme(TIMES,line);
                }
            }
            else if (ch == '/')
            {
                ch = readChar();
                if (ch == '=') return newLexeme(DIVIDES,line);
                else
                {
                    pushback(ch);
                    return newLexeme(DIVIDES,line);
                }
            }
            else if (ch == '=')
            {
                ch = readChar();
                if (ch == '=') return newLexeme(EQUALEQUAL,line);
                else
                {
                    pushback(ch);
                    return newLexeme(ASSIGN,line);
                }
            }
            else if (ch == '!')
            {
                ch = readChar();
                if (ch == '=') return newLexeme(NOTEQUALS,line);
                else
                {
                    pushback(ch);
                    return newLexeme(NOT,line);
                }
            }
            else if (isdigit(ch))
            {
                pushback(ch);
                return lexNumber();
            }
            else if (isalpha(ch))
            {
                pushback(ch);
                return lexVarOrKeyword();
            }
            else if (ch == '\'') return lexString();
            else if (ch == 0) return newLexeme(END_OF_FILE,line);
            else
            {
                printf("FATAL ERROR: UNKOWN CHARACTER - LINE %d\n",line);
                exit(1);
            }
    }
}


void
getFile(FILE *f)
{
    fp = f;
}


int
getLine()
{
    return line;
}