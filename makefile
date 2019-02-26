LOPTS = -Wall -Wextra -g -std=c99
OBJS = *.o *.pp.*

make : eval
	./eval test2.rin

run : error1 error1x error2 error2x error3 error3x error4 error4x error5 error5x arrays arraysx conditionals conditionalsx recursion recursionx iteration iterationx functions functionsx lambda lambdax objects objectsx problem problemx

rin :
	gcc $(LOPTS) -c eval.c lexeme.c lexer.c types.c parser.c evaluator.c environment.c pretty-printing.c
	gcc $(LOPTS) eval.c lexeme.o lexer.o types.o parser.o evaluator.o environment.o pretty-printing.o -o rin

error1 :
	@echo error1.rin contents:
	@cat error1.rin

error1x : rin
	-./rin error1.rin
	@echo 

error2 :
	@echo error2.rin contents:
	@cat error2.rin

error2x : rin
	-./rin error2.rin
	@echo

error3 :
	@echo error3.rin contents:
	@cat error3.rin

error3x : rin
	-./rin error3.rin
	@echo

error4 :
	@echo error4.rin contents:
	@cat error4.rin

error4x : rin
	-./rin error4.rin
	@echo

error5 :
	@echo error5.rin contents:
	@cat error5.rin

error5x : rin
	-./rin error5.rin
	@echo

arrays :
	@echo arrays.rin content:
	@cat arrays.rin

arraysx : rin
	./rin arrays.rin
	@echo

conditionals :
	@echo conditionals.rin content:
	@cat conditionals.rin

conditionalsx : rin
	./rin conditionals.rin
	@echo

recursion :
	@echo recursion.rin content:
	@cat recursion.rin

recursionx : rin
	./rin recursion.rin
	@echo

iteration :
	@echo iteration.rin contents:
	@cat iteration.rin

iterationx : rin
	./rin iteration.rin
	@echo

functions :
	@echo functions.rin content:
	@cat functions.rin

functionsx : rin
	./rin functions.rin
	@echo

lambda :
	@echo lambda.rin contents:
	@cat lambda.rin

lambdax : rin
	./rin lambda.rin
	@echo

objects :
	@echo COULD NOT GET OBJECTS TO WORK
	@echo

objectsx :
	@echo COULD NOT GET OBJECTS TO WORK
	@echo

problem :
	@echo problem.rin contents:
	@cat problem.rin

problemx : rin
	./rin problem.rin ints.txt

clean :
	rm -f $(OBJS) rin