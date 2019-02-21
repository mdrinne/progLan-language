LOPTS = -Wall -Wextra -g -std=c99
OBJS = *.o *.pp.*

make : eval
	./eval test2.rin

run : test1

scanner :
	gcc $(LOPTS) -c scanner.c lexer.c lexeme.c types.c
	gcc $(LOPTS) scanner.c lexer.o lexeme.o types.o -o scanner

eval :
	gcc $(LOPTS) -c eval.c lexeme.c lexer.c types.c parser.c evaluator.c environment.c pretty-printing.c
	gcc $(LOPTS) eval.c lexeme.o lexer.o types.o parser.o evaluator.o environment.o pretty-printing.o -o eval

pp :
	gcc $(LOPTS) -c pp.c lexeme.c lexer.c types.c parser.c pretty-printing.c
	gcc $(LOPTS) pp.c lexeme.o lexer.o types.o parser.o pretty-printing.o -o pp

recognizer:
	gcc $(LOPTS) -c recognizer.c lexer.c lexeme.c types.c parser.c
	gcc $(LOPTS) recognizer.c lexer.o lexeme.o types.o parser.o -o recognizer

test1 : pp
	@echo Original file:
	@cat test1.rin
	@echo Pretty Printed version of the original:
	@./pp test1.rin > test1.pp.1
	@cat test1.pp.1
	@echo Pretty Printed version of the pretty printed version:
	@./pp test1.pp.1 > test1.pp.2
	@cat test1.pp.2
	diff -s -q test1.pp.1 test1.pp.2

test2 : pp
	@echo Original file:
	@cat test2.rin
	@echo Pretty Printed version of the original:
	@./pp test2.rin > test2.pp.1
	@cat test2.pp.1
	@echo Pretty Printed version of the pretty printed version:
	@./pp test2.pp.1 > test2.pp.2
	@cat test2.pp.2
	diff -s -q test2.pp.1 test2.pp.2

test3 : pp
	@echo Original file:
	@cat test3.rin
	@echo Pretty Printed version of the original:
	@./pp test3.rin > test3.pp.1
	@cat test3.pp.1
	@echo Pretty Printed version of the pretty printed version:
	@./pp test3.pp.1 > test3.pp.2
	@cat test3.pp.2
	diff -s -q test3.pp.1 test3.pp.2

clean :
	rm -f $(OBJS) pp eval scanner