objects = Implementations.o  main.o


Main: $(objects)
	gcc $(objects) -o Main

main.o:QueueInterface.h

Implementations.o : QueueInterface.h

QueueInterface.h: QueueTypes.h

.PHONY : clean
clean:
	rm Main  $(objects)



