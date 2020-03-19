#include "formula.hpp"
#include "object.hpp"
#include <iostream>
#include <eigen3/Eigen/Eigen>
#include <sys/types.h>
#include <sys/stat.h>

#define N_STEPS 25 // number of iterations


int main(){
    std::vector<Eigen::Vector3d> ref_X, def_X;
    std::vector<Eigen::Vector4i> tetra;

    // object loader should load to nodes
    // init nodes
    std::vector<Object> objects;
    Object obj = load("cube2x2x2.obj");
    // obj.preCompute() // compute B and W
    // objects.push_back(obj);
    // // do I need shape?
    // std::cout << obj.nodes.size() << " nodes." << std::endl;
    // std::cout << obj.tetras.size() << " tetrahedrals." << std::endl;

    // for(size_t i = 0; i < N_STEPS; i++){
    //     Object newObj, prevObj = objects.begin() + i;
    //     newObj.nodes = deform(obj.nodes);
    //     newObj.tetras = prevObj.tetras;
    //     newObj.initVelocitiesToZero();
    //     update_XV(newObj.nodes, newObj.tetras, newObj.velocities, newObj.B, newObj.W); // FIX! newObj.B/W or prevObj.B/W
    //     objects.push_back(newObj);
    // }

    // // make a directory "out" to store object files.
    // if (mkdir("out", 0777) == -1) std::cerr << "Error :  " << strerror(errno) << std::endl; 
    // else std::cout << "Directory created\n"; 

    // for (size_t i = 1; i <= N_STEPS; i++){
    //     F = update_XV(obj.deformNodes, obj.tetras, V, B, W);

    //     // compute forces for debug
    //     // for (auto f : F) std::cout << "computed force:\n" << f << std::endl;
        
    // }

    return 0;
}