#include "object.hpp"
#include <iostream>
#include <eigen3/Eigen/Eigen>

int main() {
    auto x = load_obj();
    std::cout<<x.volumn()<<std::endl;
    x.translate(Eigen::Vector3d(5,5,5));
    std::cout<<x.volumn()<<std::endl;
    return 0;
}

