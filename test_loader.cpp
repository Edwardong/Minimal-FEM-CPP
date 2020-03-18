
#include "loader.hpp"
// #include "include/OBJ_Loader/OBJ_Loader.h"
#include <iostream>

const char* FILENAME = "models/redundant_unit_cube.obj";


int main(){
    // std::vector<Object> X;
    // load(&X, FILENAME);

    // Test loadObjFile()
    std::vector<double> vertices;
    std::vector<std::vector<int>> polygons;
    loadObjFile(FILENAME, vertices, polygons);

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

    tetgen("redundant_unit_cube", vertices, polygons);
    
    // objl::Loader loader;
    // loader.LoadFile(FILENAME);

    // for(auto v: loader.LoadedVertices) {
    //     std::cout << v.Position.X << "   " << v.Position.Y << "   " << v.Position.Z << std::endl;
    // }
    
    // for(auto m: loader.LoadedMeshes) {
    //     std::cout << "mesh" << std::endl;
    //     for(auto vi: m.Indices) {
    //         auto v = loader.LoadedVertices[vi];
    //         std::cout << "   " << v.Position.X << "   " << v.Position.Y << "   " << v.Position.Z << std::endl;
    //     }
    //     for(auto i: m.Indices) {
    //         std::cout << i << std::endl;
    //     }
    // }

    // std::cout << "===" << std::endl;

    // for(auto i: loader.LoadedIndices) {
    //     std::cout << i << std::endl;
    // }
   

    return 0;
}
