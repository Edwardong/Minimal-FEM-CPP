
#include "loader.hpp"
// #include "include/OBJ_Loader/OBJ_Loader.h"
#include <iostream>

const char* FILENAME = "models/redundant_unit_cube.obj";


int main(){

    auto obj = readTetgenFile("tmp_redundant_unit_cube");
    for(auto i : obj.nodes) {
        std::cout<<i<<std::endl<<std::endl;
    }
    return 0;

    // Test loadObjFile()
    std::vector<double> vertices;
    std::vector<std::vector<int>> polygons;
    readObjFile(FILENAME, vertices, polygons);

    std::cout<<vertices.size()<<std::endl;
    for(auto v : vertices)
        std::cout<<v<<' ';
    std::cout<<std::endl;

    std::cout<<polygons.size()<<std::endl;
    for(auto polygon : polygons) {
        for(auto p: polygon) {
            std::cout<<p<<' ';
        }
        std::cout<<std::endl;
    }

    // Test tetgen()
    tetgen("redundant_unit_cube", vertices, polygons);
    


    return 0;
}
