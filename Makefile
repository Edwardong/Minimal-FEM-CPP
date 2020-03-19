gxx = g++
VERSION_FLAG = -std=c++11
objects =  object.o formula.o

all: FEM

# main: ${DEPENDENCIES}
# 	${gxx} $^ -o $@

FEM:main.cpp $(objects)
	${gxx} ${VERSION_FLAG} -Wall main.cpp $(objects) -o FEM

object.o: object.cpp
	${gxx} -c ${VERSION_FLAG} -Wall object.cpp -o object.o

formula.o: formula.cpp
	${gxx} -c ${VERSION_FLAG} -Wall formula.cpp -o formula.o

clean:
	rm -rf FEM $(objects) out