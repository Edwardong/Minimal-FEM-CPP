gxx = g++

CXXFLAGS = -g -ggdb

DEPENDENCIES = loader.o object.o formula.o main.o # scene.o

TETGEN_DIR = include/tetgen

tetgen = ${TETGEN_DIR}/libtet.a

all: main

main: ${DEPENDENCIES}
	${gxx} $^ -o $@
	./main
	# rm *.o main 

# %.o: %.cpp %.hpp
# 	gcc -o $@ $<

${tetgen}:
	cd ${TETGEN_DIR} && make tetlib 

test_loader: test_loader.o loader.o ${tetgen}
	${gxx} ${CXXFLAGS} $^ -o $@.out
	./$@.out

test_object: test_object.o object.o loader.o ${tetgen}
	${gxx} ${CXXFLAGS} $^ -o $@.out
	./$@.out

# test_scene: test_scene.o scene.o object.o
# 	${gxx} ${CXXFLAGS} $^ -o $@.out

clean:
	rm *.o *.out main ${TETGEN_DIR}/*.a ${TETGEN_DIR}/*.o
