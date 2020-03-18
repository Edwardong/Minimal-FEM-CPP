// #include <eigen3/Eigen/Eigen>
#include "stdlib.h"
#include <vector>

// typedef std::vector<Eigen::Vector3d> Object;

bool loadObjFile(const char* filename, std::vector<double>& vertices, std::vector<std::vector<int>>& polygons) ;
void tetgen(char* filename, std::vector<double> vertices, std::vector<std::vector<int>> polygons);
// bool load(std::vector<Object>* X, const char* filename);

