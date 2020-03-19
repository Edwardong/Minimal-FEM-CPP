#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <vector>
#include <eigen3/Eigen/Eigen>
#include <iostream>
#include <fstream>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>

#define SIZE 1

std::vector<Eigen::Vector3d> deform(std::vector<Eigen::Vector3d> ref_X);

int serialize(int x, int y, int z);

void load_obj(  std::vector<Eigen::Vector3d> &nodes, 
                std::vector<Eigen::Vector4i> &tetras);

void export_obj(std::vector<Eigen::Vector3d> def_X,
                std::vector<Eigen::Vector4i> T,
                int index);

#endif // OBJECT_HPP