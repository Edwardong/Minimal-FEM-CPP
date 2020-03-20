gxx = g++

CXXFLAGS = -g -ggdb -std=c++11

TETGEN_DIR = include/tetgen

tetgen = ${TETGEN_DIR}/libtet.a

DEPENDENCIES = loader.o object.o formula.o main.o ${tetgen} # scene.o


all: main

main: ${DEPENDENCIES}
	${gxx} $^ -o $@
	#./main
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
	rm *.o main ${TETGEN_DIR}/*.a ${TETGEN_DIR}/*.o
