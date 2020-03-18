#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <vector>
#include <eigen3/Eigen/Eigen>

#define SIZE 1

std::vector<Eigen::Vector3d> deform(std::vector<Eigen::Vector3d> ref_X);

int serialize(int x, int y, int z);

void load_obj(std::vector<Eigen::Vector3d> &nodes, std::vector<Eigen::Vector4i> &tetras);



// Object class encapsulates nodes, tetras, and velocities of an object
class Object {
  public:
    std::vector<Eigen::Vector3d> nodes;
    std::vector<Eigen::Vector4i> tetras;
    std::vector<Eigen::Vector3d> velocities;
    
    void initVelocitiesToZero();
    void translate(Eigen::Vector3d displacement);
};



#endif // OBJECT_HPP
