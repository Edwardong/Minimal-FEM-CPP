#include "formula.hpp"
#include "object.hpp"
#include <iostream>
#include <eigen3/Eigen/Eigen>

#define N_STEPS 25 // number of iterations


int main(){
    std::vector<Eigen::Vector3d> ref_X, def_X;
    std::vector<Eigen::Vector4i> tetra;

    // object loader should load to ref_X
    // init ref_X
    load_obj(ref_X, tetra);
    // do I need shape?
    std::cout << ref_X.size() << " nodes." << std::endl;
    std::cout << tetra.size() << " tetrahedral." << std::endl;

    def_X = deform(ref_X);      // compute deformed X

    std::vector<Eigen::Matrix3d> B;
    std::vector<double> W;
    precompute(ref_X, tetra, B, W); // compute B and W

    std::vector<Eigen::Vector3d> V(ref_X.size(), Eigen::Vector3d(0, 0, 0)); // velocity
    std::vector<Eigen::Vector3d> F; // force

    // init all velocities to 0

    for (size_t i = 1; i <= N_STEPS; i++){
        F = update_XV(def_X, tetra, V, B, W);

        // compute forces for debug
        for (auto f : F) std::cout << "computed force:\n" << f << std::endl;
    }

    return 0;
}