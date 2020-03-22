#include "formula.hpp"
#include "object.hpp"
#include <iostream>
#include <eigen3/Eigen/Eigen>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/stat.h> 
#include <sys/types.h>

#define N_STEPS 1000 // number of iterations
#define stepPerFrame 20

int main(int argc, char *argv[]){
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

    // make a directory "out" to store object files.
    if (mkdir("out", 0777) == -1) 
        std::cerr << "Error :  " << strerror(errno) << std::endl; 
    else 
        std::cout << "Directory created\n"; 

    std::string filename = "models/redundant_unit_cube.obj";
    std::string argFilename(argv[argc - 1]);
    if(argFilename.substr(argFilename.length() - 4, 4) == ".obj") 
        filename = argFilename;
    std::cout<<filename<<std::endl;
    

    // 1. load *.obj to Object obj.
    Object obj = load(filename);
    // Object obj = load_obj(); // for debugging
    //std::cout<< "Finished loading the second." <<std::endl;

    // 2. Deform the object. Convert the object nodes to def_X.
    obj.refNodes = scale(obj.refNodes);
    obj.nodes = deform(obj.refNodes);

    std::cout<<obj<<std::endl;

    // 3. Precompute B and W.
    precompute(obj.refNodes, obj.tetras, B, W);

    // std::vector<Object> objects;
    // objects.push_back(ori_obj);

    // for(size_t i = 0; i < N_STEPS; i++){
    //     Object prevObj = *(objects.begin() + i);
    //     Object newObj(deform(prevObj.nodes),prevObj.tetras);
    //     // newObj.nodes = deform(prevObj.nodes);
    //     // newObj.tetras = prevObj.tetras;
    //     newObj.velocities = prevObj.velocities;
    //     //newObj.initVelocitiesToZero();
    //     update_XV(newObj.nodes, newObj.tetras, newObj.velocities, B, W); // FIX! newObj.B/W or prevObj.B/W
    //     objects.push_back(newObj);
    // }
    int count = 1;
    for (size_t i = 0; i < N_STEPS; i++){

        // 5. Update positions and velocities.
        F = update_XV(obj.nodes, obj.tetras, obj.velocities, B, W);
        // compute forces for debug
        // for (auto f : F) std::cout << "computed force:\n" << f << std::endl;
        if(i % stepPerFrame == 0){
            // 4. Export to *.obj
            obj.export_obj(count, "out");
            if (count==1 ||count % 10 == 0){
                std::cout<<"exported the " << count << "-th/"<< N_STEPS/stepPerFrame << " obj" <<std::endl;
            }
            count++;
            //std::cout<<i<<" / "<<N_STEPS<<std::endl;
        }
    }

    return 0;
}