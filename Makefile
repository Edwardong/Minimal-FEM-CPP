gxx = g++

DEPENDENCIES =  object.o formula.o main.o

all: main

main: ${DEPENDENCIES}
	${gxx} $^ -o $@

clean:
	rm *.o main