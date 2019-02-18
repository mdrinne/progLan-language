RIN programming language
Matt Rinne
cs403 Programming Languagues

============================
Very similar to c, with slight differences

Includes at the top

Main function at bottom of program

variable definition: type is not specified, use var
  var x = 123
  var str = 'string'
  var temp

Function definition: "func FFFF" 
  where FFFF is replaced with the function name

Call functions with "call FFFF()"
  where FFF is replaced with the function name
  
Statements end with a '$'

Strings can be added together with '+'

Built in stacks, queues, and arrays in the making

Print with 'call print()'

============================
To run Pretty Printer using makefile commands:

  make : compiles pp executable
  
  make run : runs all 3 test cases
  
  make clean : gets rid of all *.o, *.pp.* files and pp executable

  make test1: runs test1, output below

  make test2: runs test2, output below

  make test3: runs test3, output below

=============================
test1 output:

Original file:
? 
  Matthew Rinne
  test1.txt
  cs403 Programming Languages
?

#include "example.h"

func newFunc(var numb, var text)
{
    return numb$
}

func secondFunc()
{
    var xyz = 'hello, world!'$
    return xyz$
}

main () {
    ? variable definitions ?
    var x = 57.3$
    var string = 'hello'$
    var empty$

    ? some expressions ?
    var y = x + 1$
    var z = y * x$
    var temp = string + 'world'$

    ? functioncall (also uses optExprList) ?
    var a = call newFunc(x, string)$
    var b = call secondFunc()$

    ? if block ?
    if (x = 58)
    {
        x = x - 1$
    }
    else if (x = 56)
    {
        x = x + 1$
    }
    else
    {
        x = 57$
    }

    ? while loop ?
    while (x != 59)
    {
        x = x + 1$
    }

    ? for loop ?
    for (var i=0; i<5; i++)
    {
        x * 5$
    }

    return$
}
Pretty Printed version of the original:






#include "example.h"

func newFunc(var numb, var text)
{
return numb$
}

func secondFunc()
{
var xyz = 'hello, world!'$
return xyz$
}

main ()
{
var x = 57.300000$
var string = 'hello'$
var empty$


var y = x + 1$
var z = y * x$
var temp = string + 'world'$


var a = call newFunc(xstring)$
var b = call secondFunc()$


if (x = 58)
{
x = x - 1$
}
else if (x = 56)
{
x = x + 1$
}
else 
{
x = 57$
}


while (x != 59)
{
x = x + 1$
}


for (var i=0; i<5; i++)
{
x * 5$
}

return$
}
Pretty Printed version of the pretty printed version:






#include "example.h"

func newFunc(var numb, var text)
{
return numb$
}

func secondFunc()
{
var xyz = 'hello, world!'$
return xyz$
}

main ()
{
var x = 57.300000$
var string = 'hello'$
var empty$


var y = x + 1$
var z = y * x$
var temp = string + 'world'$


var a = call newFunc(xstring)$
var b = call secondFunc()$


if (x = 58)
{
x = x - 1$
}
else if (x = 56)
{
x = x + 1$
}
else 
{
x = 57$
}


while (x != 59)
{
x = x + 1$
}


for (var i=0; i<5; i++)
{
x * 5$
}

return$
}
diff -s -q test1.pp.1 test1.pp.2
Files test1.pp.1 and test1.pp.2 are identical
  
============================
test2 output:

Original file:
? 
  Matthew Rinne
  test2.txt
  cs403 Programming Languages
?

#include "stdfunc.h"
#include "sbkahsdhjk.c"
#include "skljs.skd"

#include "shjksld.sdfk"


func combineStrings(var str1, var str2)
{
    return str1_hsj + str2$
}


func blah()
{
    return 12$
}


main ()
{
    var x = 1$

    while (x != 20)
    {
        for (var i=0; i<=2; i++)
        {
            call print(x)$
        }
        x++$
    }
    
    var y = x % 6$

    var str1 = 'hello'$
    var str2 = ', world!'$
    
    var str3 = call combineStrings(str1,str2)$

    call print(str3)$
    
    return$
}Pretty Printed version of the original:






#include "stdfunc.h"
#include "sbkahsdhjk.c"
#include "skljs.skd"

#include "shjksld.sdfk"


func combineStrings(var str1, var str2)
{
return str1_hsj + str2$
}


func blah()
{
return 12$
}


main ()
{
var x = 1$

while (x != 20)
{
for (var i=0; i<=2; i++)
{
call print(x)$
}
x++$
}

var y = x % 6$

var str1 = 'hello'$
var str2 = ', world!'$

var str3 = call combineStrings(str1str2)$

call print(str3)$

return$
}
Pretty Printed version of the pretty printed version:






#include "stdfunc.h"
#include "sbkahsdhjk.c"
#include "skljs.skd"

#include "shjksld.sdfk"


func combineStrings(var str1, var str2)
{
return str1_hsj + str2$
}


func blah()
{
return 12$
}


main ()
{
var x = 1$

while (x != 20)
{
for (var i=0; i<=2; i++)
{
call print(x)$
}
x++$
}

var y = x % 6$

var str1 = 'hello'$
var str2 = ', world!'$

var str3 = call combineStrings(str1str2)$

call print(str3)$

return$
}
diff -s -q test2.pp.1 test2.pp.2
Files test2.pp.1 and test2.pp.2 are identical

============================
test3 output:

Original file:
? 
  Matthew Rinne
  test3.txt
  cs403 Programming Languages
?

#include "stdfunc.h"


func square(var num)
{
    return num * num$
}

func doubleString(var str)
{
    return string + string$
}


main ()
{
    var x = 10$
    x = call square(x)$

    var y = 5$
    y = call square(call square(y))$

    var str = 'ha'$
    str = call doubleString(call doubleString(str))$
    call print(str)$

    if (x == 100 && y == 25)
    {
        call print('WOW')$
    }
    else if (x == 100 && y == 125)
    {
        call print('WOOH')$
    }
    else
    {
        call print(str)$
    }

    return$
}Pretty Printed version of the original:






#include "stdfunc.h"


func square(var num)
{
return num * num$
}

func doubleString(var str)
{
return string + string$
}


main ()
{
var x = 10$
x = call square(x)$

var y = 5$
y = call square(call square(y))$

var str = 'ha'$
str = call doubleString(call doubleString(str))$
call print(str)$

if (x == 100 && y == 25)
{
call print('WOW')$
}
else if (x == 100 && y == 125)
{
call print('WOOH')$
}
else 
{
call print(str)$
}

return$
}
Pretty Printed version of the pretty printed version:






#include "stdfunc.h"


func square(var num)
{
return num * num$
}

func doubleString(var str)
{
return string + string$
}


main ()
{
var x = 10$
x = call square(x)$

var y = 5$
y = call square(call square(y))$

var str = 'ha'$
str = call doubleString(call doubleString(str))$
call print(str)$

if (x == 100 && y == 25)
{
call print('WOW')$
}
else if (x == 100 && y == 125)
{
call print('WOOH')$
}
else 
{
call print(str)$
}

return$
}
diff -s -q test3.pp.1 test3.pp.2
Files test3.pp.1 and test3.pp.2 are identical

============================
