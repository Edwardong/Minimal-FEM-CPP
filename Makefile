gxx = g++

DEPENDENCIES =  object.o formula.o main.o

all: main

main: ${DEPENDENCIES}
	${gxx} $^ -o $@
	./main
	# rm *.o main 

clean:
	rm *.o main