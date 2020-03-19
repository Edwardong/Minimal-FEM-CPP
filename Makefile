gxx = g++

CXXFLAGS = -g -ggdb

DEPENDENCIES =  object.o formula.o main.o

TETGEN_DIR = include/tetgen

all: main

main: ${DEPENDENCIES}
	${gxx} $^ -o $@
	./main
	# rm *.o main 

# %.o: %.cpp %.hpp
# 	gcc -o $@ $<

tetgen: 
	cd ${TETGEN_DIR} && make tetlib 

test_loader: test_loader.o loader.o tetgen
	${gxx} ${CXXFLAGS} test_loader.o loader.o ${TETGEN_DIR}/libtet.a -o $@.out
	./$@.out

test_object: test_object.o object.o
	${gxx} ${CXXFLAGS} $^ -o $@.out
	./$@.out

clean:
	rm -rf out *.o main 
	# rm *.o *.out main include/tetgen/*.a include/tetgen/*.o