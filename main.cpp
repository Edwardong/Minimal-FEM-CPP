#include "formula.hpp"
#include "object.hpp"
#include <iostream>
#include <eigen3/Eigen/Eigen>
#include <sys/types.h>
#include <sys/stat.h>

#define N_STEPS 400 // number of iterations
#define stepPerFrame 4

int main(){
    //std::vector<Eigen::Vector3d> ref_X, def_X;
    //std::vector<Eigen::Vector4i> tetra;

    // object loader should load to nodes
    // init nodes
    //std::vector<Object> objects;
    
    // Object obj = load("models/redundant_unit_cube.obj");
    // std::cout<< "Finished loading the first." <<std::endl;
    // obj.export_obj(1);
    // std::cout<< "Finished exporting the first." <<std::endl;

    // Object obj = load("models/redundant_unit_cube.obj");
    // std::cout<< "Finished loading the second." <<std::endl;
    // obj2.export_obj(2);
    // std::cout<< "Finished exporting the second." <<std::endl;
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

    std::vector<Eigen::Vector3d> def_X, F;
    std::vector<Eigen::Matrix3d> B;
    std::vector<double> W;

    // 1. load *.obj to Object obj.
    Object obj = load("models/redundant_unit_cube.obj");
    std::cout<< "Finished loading the second." <<std::endl;

    // 2. Deform the object. Convert the object nodes to def_X.
    def_X = deform(obj.nodes);
    // 3. Precompute B and W.
    precompute(obj.nodes, obj.tetras, B, W);
    // 4. Init all velocities to zero.
    obj.initVelocitiesToZero();

    int count = 1;
    for (size_t i = 0; i < N_STEPS; i++){

        // 5. Update positions and velocities.
        F = update_XV(def_X, obj.tetras, obj.velocities, B, W);
        // compute forces for debug
        // for (auto f : F) std::cout << "computed force:\n" << f << std::endl;
        if(i % stepPerFrame == 0){
            // 4. Export to *.obj
            obj.export_obj(count);
            count++;
        }
        obj.nodes = def_X;
    }

    return 0;
}