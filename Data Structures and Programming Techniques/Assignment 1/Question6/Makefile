objects = Implementations.o  main.o


Main: $(objects)
	gcc $(objects) -o Main

main.o: Interface.h

Implementations.o : Interface.h

.PHONY : clean
clean:
	rm Main  $(objects)


