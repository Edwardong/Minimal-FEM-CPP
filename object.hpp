#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <vector>
#include <eigen3/Eigen/Eigen>

#define SIZE 10

std::vector<Eigen::Vector3d> deform(std::vector<Eigen::Vector3d> ref_X);

// Object class encapsulates nodes, tetras, and velocities of an object
class Object {
  public:
    std::vector<Eigen::Vector3d> nodes;
    std::vector<Eigen::Vector4i> tetras;
    std::vector<Eigen::Vector3d> velocities;
    
    Object();
    Object(std::vector<Eigen::Vector3d> nodes, std::vector<Eigen::Vector4i> tetras);

    // Initialize velocities to zeros after setting nodes
    void initVelocitiesToZero();
    // Translate all nodes for a 'displacement'
    void translate(Eigen::Vector3d displacement);
    // Total volumn of the object
    double volumn();
};

// Jim's function that loads a single cube of SIZE.
Object load_obj();



#endif // OBJECT_HPP


