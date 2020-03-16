#include "object.hpp"

std::vector<Eigen::Vector3d> deform(std::vector<Eigen::Vector3d> ref_X){
    std::vector<Eigen::Vector3d> result;
    for (auto item : ref_X) result.push_back(Eigen::Vector3d(item[0], item[1], item[2] * 1.5));
    return result;
}