#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <eigen3/Eigen/Eigen>
#include <vector>
#include <iostream>
#include <fstream>

#define SIZE 10


// Object class encapsulates nodes, tetras, and velocities of an object
class Object {
  public:
    std::vector<Eigen::Vector3d> nodes;
    std::vector<Eigen::Vector3d> refNodes;
    std::vector<Eigen::Vector4i> tetras;
    std::vector<Eigen::Vector3d> velocities;
    
    // Object(); // This constructor is too dangerous. Deprecated.
    Object(std::vector<Eigen::Vector3d> nodes, std::vector<Eigen::Vector4i> tetras);

    // Translate all nodes for a 'displacement'
    void translate(Eigen::Vector3d displacement);
    // Total volumn of the object
    double volumn() const;
    // Export as object file
    void export_obj(int index,std::string out_dir);
    // Generate faces "f" and normals "vn", for export
    void generateFacesAndNormals(std::vector<Eigen::Vector3d>& normals, std::vector<Eigen::Vector3i>& faces);
    // Print Object
    friend std::ostream& operator<<(std::ostream& os, const Object& obj);
};

std::vector<Eigen::Vector3d> scale(std::vector<Eigen::Vector3d> nodes);
std::vector<Eigen::Vector3d> deform(std::vector<Eigen::Vector3d> ref_X);

std::vector<Eigen::Vector3d> gravity(Object obj, int t);

// Jim's function that loads a single cube of SIZE.
Object load_obj();

// The ultimately simplified loading function
Object load(std::string filename);

#endif // OBJECT_HPP


