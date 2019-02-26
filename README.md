RIN programming language
Matt Rinne
cs403 Programming Languagues

============================
ALL PSEUDO CODE WAS PROVIDED BY DR. LUSTH

============================
Very similar to c, with slight differences

Main function at bottom of program

variable definition: type is not specified, use var
  var x = 123
  var str = 'string'

Function definition: "func FFFF" 
  where FFFF is replaced with the function name

Call functions with "call FFFF()"
  where FFF is replaced with the function name
  
Statements end with a '$'

Available operators :
    +
    -
    *
    /
    =
    >
    >=
    <
    <=
    ==
    !=
    %
    ||
    &&

Var types supported :
    INTEGER
    REAL
    STRING
    BOOL
    FP (file pointer)

============================
BUILTIN FUNCTIONS

newArray(INTEGER) : takes in an integer and creates an array of that size

setArray(ARRAY,INTEGER,VALUE) : takes in an array, integer and value, the integer  being the index of the array to set

getArray(ARRAY,INTEGER) : takes in an array and integer, returning the value at the array at the given index

getArgCount() : returns the number of command line arguments

getArg(INTEGER) : takes in an integer and returns the argument at the given index

openFileForReading(STRING) : takes in a string (filename) and opens that file for reading, returning a file pointer

readInt(FP) : takes in a filepointer and returns the next integer in the file

eof(FP) : takes in a filepointer and returns true if at the end of the file, else returns false

closeFile(FP) : takes in a filepointer and closes the given file

print(UNARY) : takes in any unary and prints it to stdout

printNewLine() : prints a new line character to stdout

============================
To run evaluator using makefile commands:

    make : compiles rin evaluator

    run : runs evaluator on given file name

    error1 : prints error1.rin testfile

    error1x :  compiles evaluator and executes error1.rin testfile

    error2 : prints error2.rin testfile

    error2x :  compiles evaluator and executes error2.rin testfile

    error3 : prints error3.rin testfile

    error3x :  compiles evaluator and executes error3.rin testfile

    error4 : prints error4.rin testfile

    error4x :  compiles evaluator and executes error4.rin testfile

    error5 : prints error5.rin testfile

    error5x :  compiles evaluator and executes error5.rin testfile

    arrays : prints arrays.rin testfile

    arraysx : compiles evaluator and executes arrays.rin testfile

    conditionals : prints conditionals.rin testfilie

    conditionalsx : compiles evaluator and executes conditionals.rin testfile

    recursion : prints recursion.rin testfile

    recursionx : compiles evaluator and executes recursion.rin testfile

    iteration : prints iterations.rin testfile

    iterationx : compiles evaluator and executes iteration.rin testfile

    functions : prints functions.rin testfile

    functionsx : compiles evaluator and executes functions.rin testfile

    lambda : prints lambda.rin testfile

    lambdax : compiles evaluator and executes lambda.rin testfile

    objects : prints objects.rin testfile

    objectsx : compiles evaluator and executes objects.rin testfile

    problem : prints problem.rin testfile

    problemx : compiles evaluator and executes problem.rin testfile

    clean : cleans directory of all .o files and rin executable
