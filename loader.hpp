// #include <eigen3/Eigen/Eigen>
#include "object.hpp"
#include "stdlib.h"
#include <vector>

// typedef std::vector<Eigen::Vector3d> Object;

// Read vertices and polygons from an .obj file
void readObjFile(const char* filename, std::vector<double>& vertices, std::vector<std::vector<int>>& polygons) ;

// Create tetrahedral mesh with Tetgen library
// input:  vertices, polygons (of .obj file)
// output: nodes, tetras (for Objectd class)
void tetrahedralize(std::vector<double> vertices, std::vector<std::vector<int>> polygons, std::vector<Eigen::Vector3d>& nodes, std::vector<Eigen::Vector4i>& tetras);

// Combination of above
Object load(char* filename);
