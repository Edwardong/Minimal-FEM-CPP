#include "object.hpp"
#include <iostream>
#include <eigen3/Eigen/Eigen>

int main() {
    const std::string FILENAME = "models/redundant_unit_cube.obj";
    // const std::string FILENAME = "models/unit_cube.obj";

    Object x = load_obj();
    std::cout<<x.volumn()<<std::endl;
    x.translate(Eigen::Vector3d(5,5,5));
    std::cout<<x.volumn()<<std::endl;
    std::cout<<x;

    // Test Object::load()
    Object obj = load(FILENAME);
    std::cout<<obj;
    std::cout<<obj.volumn()<<std::endl;
    return 0;
}

