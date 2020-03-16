#include <iostream>
#include <eigen3/Eigen/Eigen>
#include "formula.hpp"
#include "object.hpp"
#define N_STEPS 25 // number of iterations

int main(){
    std::vector<Eigen::Vector3d> ref_X, def_X;
    std::vector<Eigen::Vector4i> tetra;
    // shape??
    std::cout << ref_X.size() << " nodes." << std::endl;
    std::cout << tetra.size() << " tetrahedral." << std::endl;

    def_X = deform(ref_X);      // compute deformed X

    std::vector<Eigen::Matrix3d> B;
    std::vector<double> W;
    precompute(ref_X, tetra, B, W); // compute B and W

    std::vector<Eigen::Vector3d> V; // velocity
    std::vector<Eigen::Vector3d> F; // force

    for (size_t i = 1; i <= N_STEPS; i++){
        F = update_XV(def_X, tetra, V, B, W);
        // std::cout << "computed force: " << F << std::endl;
    }

    return 0;
}