// #include <eigen3/Eigen/Eigen>
#include "object.hpp"
#include "stdlib.h"
#include <vector>

// typedef std::vector<Eigen::Vector3d> Object;

// Read vertices and polygons from an .obj file
void readObjFile(const char* filename, std::vector<double>& vertices, std::vector<std::vector<int>>& polygons) ;

// Create tetrahedral mesh with Tetgen library, and output as a file
void tetgen(char* filename, std::vector<double> vertices, std::vector<std::vector<int>> polygons);

// Read the Tetgen output file and create and Object
Object readTetgenFile(char* filename);

// TODO
// load function runs all the three functions above
// bool load(std::vector<Object>* X, const char* filename);

