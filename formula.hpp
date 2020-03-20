#ifndef FORMULA_HPP
#define FORMULA_HPP

#include <eigen3/Eigen/Eigen>
#include <iostream>
#include <vector>

//using namespace std;


// #define MU 0.1758
// #define LAMBDA 0.7141
// #define GAMMA 1
// #define DELTA_TIME 1
// #define MASS 100.0
#define MU 0.3
#define LAMBDA 0.3
#define GAMMA 8
#define DELTA_TIME 1
#define MASS 20.0
#define I3 Eigen::Matrix3d::Identity()
#define GRAVITY Eigen::Vector3d(0,0,-0.0002)
#define COEF_OF_RESTITUTION 0
//remind
//input two empty vectors B, W
void precompute(std::vector<Eigen::Vector3d> X,
                               std::vector<Eigen::Vector4i> T,
                               std::vector<Eigen::Matrix3d> &B,
                               std::vector<double> &W);

Eigen::Matrix3d compute_P(Eigen::Matrix3d F);

std::vector<Eigen::Vector3d> compute_F(std::vector<Eigen::Vector3d> def_X,
                                       std::vector<Eigen::Vector4i> T,
                                       std::vector<Eigen::Matrix3d> B,
                                       std::vector<double> W);


Eigen::Matrix3d compute_dP(Eigen::Matrix3d F, Eigen::Matrix3d dF);

std::vector<Eigen::Vector3d> compute_dF(std::vector<Eigen::Vector3d> def_X,
                                        std::vector<Eigen::Vector3d> dX,
                                        std::vector<Eigen::Vector4i> T,
                                        std::vector<Eigen::Matrix3d> B,
                                        std::vector<double> W);

std::vector<Eigen::Vector3d> update_XV(std::vector<Eigen::Vector3d>& def_X,
                                        std::vector<Eigen::Vector4i> T,
                                        std::vector<Eigen::Vector3d>& V,
                                        std::vector<Eigen::Matrix3d> B,
                                        std::vector<double> W);

std::vector<Eigen::Vector3d> negative_V(std::vector<Eigen::Vector3d> V);

#endif //FORMULA_HPP