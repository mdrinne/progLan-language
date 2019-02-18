LOPTS = -Wall -Wextra -g -std=c99
OBJS = *.o *.pp.*

make : pp

run : test1

pp :
	gcc $(LOPTS) -c pp.c lexeme.c lexer.c types.c parser.c pretty-printing.c
	gcc $(LOPTS) pp.c lexeme.o lexer.o types.o parser.o pretty-printing.o -o pp

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
	rm -f $(OBJS) pp