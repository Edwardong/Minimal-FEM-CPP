#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <eigen3/Eigen/Eigen>
#include <vector>
#include <iostream>
#include <fstream>

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
    // Export as object file
    void export_obj(int index);

    // Print Object
    friend std::ostream& operator<<(std::ostream& os, const Object& obj);
};


// Jim's function that loads a single cube of SIZE.
Object load_obj();

// The ultimately simplified loading function
Object load(std::string filename);

#endif // OBJECT_HPP


