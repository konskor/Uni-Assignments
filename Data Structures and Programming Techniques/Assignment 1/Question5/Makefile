objects = STACKImplementations.o  postfix-to-infix.o


Main: $(objects)
	gcc $(objects) -o Main

postfix-to-infix.o: STACK.h

STACKImplementations.o : STACK.h

STACK.h: Item.h

.PHONY : clean
clean:
	rm Main  $(objects)



