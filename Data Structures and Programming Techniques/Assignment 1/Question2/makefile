objects = ProgrammImplementation.o  main.o


MainProgramm: $(objects)
	gcc $(objects) -o MainProgramm

main.o: ProgrammInterface.h

ProgrammImplementation.o : ProgrammInterface.h

.PHONY : clean
clean:
	rm MainProgramm  $(objects)

