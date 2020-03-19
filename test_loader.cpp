#include "loader.hpp"
#include <iostream>



int main(){
    const std::string FILENAME = "models/redundant_unit_cube.obj";
    // load(FILENAME);
    // return 0;
    
    // Test readObjFile()
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

    std::vector<Eigen::Vector3d> nodes;
    std::vector<Eigen::Vector4i> tetras;
    tetrahedralize(vertices, polygons, nodes, tetras);

    std::cout<<"nodes.size() = "<<nodes.size()<<std::endl;
    std::cout<<"tetras.size() = "<<tetras.size()<<std::endl;

    return 0;
}
