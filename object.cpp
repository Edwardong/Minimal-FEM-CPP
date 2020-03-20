#include "object.hpp"
#include "loader.hpp"
#include <iostream>

std::vector<Eigen::Vector3d> deform(std::vector<Eigen::Vector3d> ref_X){
    // Only one dimension "y" changed.
    std::vector<Eigen::Vector3d> result;
    for (auto item : ref_X) result.push_back(Eigen::Vector3d(item[0], item[1], item[2] * 1.1));
    return result;
}

int serialize(int x, int y, int z){
    return x*(SIZE + 1)*(SIZE + 1) + y*(SIZE + 1) + z;
}

// Jim's function that loads a single cube.
Object load_obj(){

    std::vector<Eigen::Vector3d> nodes;
    std::vector<Eigen::Vector4i> tetras;

    // init nodes
    for( size_t x = 0; x <= SIZE; x++)
        for( size_t y = 0; y <= SIZE; y++)
            for( size_t z = 0; z <= SIZE; z++)
                nodes.push_back(Eigen::Vector3d(x, y, z));

    // init tetras
    for( size_t ix = 0; ix < SIZE; ix++){
        for( size_t iy = 0; iy < SIZE; iy++){
            for( size_t iz = 0; iz < SIZE; iz++){
                int node000 = serialize(ix, iy, iz);
                int node001 = serialize(ix, iy, iz+1);
                int node010 = serialize(ix, iy+1, iz);
                int node011 = serialize(ix, iy+1, iz+1);
                int node100 = serialize(ix+1, iy, iz);
                int node101 = serialize(ix+1, iy, iz+1);
                int node110 = serialize(ix+1, iy+1, iz);
                int node111 = serialize(ix+1, iy+1, iz+1);

                tetras.push_back(Eigen::Vector4i(node000, node110, node100, node101));
                tetras.push_back(Eigen::Vector4i(node000, node110, node010, node011));
                tetras.push_back(Eigen::Vector4i(node011, node101, node001, node000));
                tetras.push_back(Eigen::Vector4i(node011, node101, node111, node110));
                tetras.push_back(Eigen::Vector4i(node000, node110, node011, node101));
            }
        }
    }

    return Object(nodes, tetras);
}



Object::Object(){}

Object::Object(std::vector<Eigen::Vector3d> nodes, std::vector<Eigen::Vector4i> tetras) {
    this->nodes = nodes;
    this->tetras = tetras;
    initVelocitiesToZero();
}

void Object::initVelocitiesToZero() {
    for(int i = 0; i < nodes.size(); i++) {
        velocities.push_back(Eigen::Vector3d(0,0,0));
    }
}

void Object::translate(Eigen::Vector3d displacement) {
    for(auto n: nodes) {
        n += displacement;
    }
}

double Object::volumn() {
    double volumn = 0;
    for(auto t : tetras) {
        Eigen::Vector3d a = nodes[t[0]];
        Eigen::Vector3d b = nodes[t[1]];
        Eigen::Vector3d c = nodes[t[2]];
        Eigen::Vector3d d = nodes[t[3]];
        volumn += abs((b-a).dot((c-a).cross(d-a))) / 6;
    }
    return volumn;
}

std::ostream& operator<<(std::ostream& os, const Object& obj) {
    os  << obj.nodes.size() << " nodes, " 
        << obj.tetras.size() << " tetras." << std::endl;
    // for(auto n : obj.nodes) {
    //     os << "( " << n[0] << " " << n[1] << " " << n[2] << " )" << std::endl;
    // }
    // for(auto t : obj.tetras) {
    //     os << "< " << t[0] << " " << t[1] << " " << t[2] << " " << t[3] << " >" << std::endl;
    // }
    return os;
}


Object load(std::string filename) {
    std::vector<double> vertices;
    std::vector<std::vector<int>> polygons;
    readObjFile(filename, vertices, polygons);
    std::vector<Eigen::Vector3d> nodes;
    std::vector<Eigen::Vector4i> tetras;
    tetrahedralize(vertices, polygons, nodes, tetras);
    return Object(nodes, tetras);
}

void Object::export_obj(int index, std::string out_dir){
    // 1. create file.
    std::ofstream myfile;
    std::string name = out_dir + "/out_" + std::to_string(index) + ".obj";
    myfile.open(name);

    // 2. write to file.
    // // 2.1 write vertex info 
    myfile << "g default\n";
    for(auto vertex : nodes){
        std::string v = "v ";
        v += std::to_string(vertex[0]) + " " + std::to_string(vertex[1]) + " " + std::to_string(vertex[2]) + "\n";
        myfile << v;
    }
    // // 2.2 write tetrahedral faces
    myfile << "g tetras\n";
    for(auto tetra : tetras){
        std::string f = "f ";
        f += std::to_string(1 + tetra[0]) + " " + std::to_string(1 + tetra[1]) + " " + std::to_string(1 + tetra[2]) + "\n";
        f += "f " + std::to_string(1 + tetra[1]) + " " + std::to_string(1 + tetra[2]) + " " + std::to_string(1 + tetra[3]) + "\n";
        f += "f " + std::to_string(1 + tetra[0]) + " " + std::to_string(1 + tetra[2]) + " " + std::to_string(1 + tetra[3]) + "\n";
        f += "f " + std::to_string(1 + tetra[0]) + " " + std::to_string(1 + tetra[1]) + " " + std::to_string(1 + tetra[3]) + "\n";
        myfile << f;
    }
    // 3. close the file.
    myfile.close();
}

// std::vector<Eigen::Vector3d> gravity(Object obj, int t){
//     double g = -1.5;  // assume each node has 1
//     std::vector<Eigen::Vector3d> result, ref_X = obj.nodes, velocities = obj.velocities;
//     // find midpoint
//     Eigen::Vector3d midPoint = Eigen::Vector3d( *(ref_X.end()) - *(ref_X.begin())) / 2.0;
//     double midNorm = midPoint.norm();

//     for (auto item : ref_X){
//         result.push_back(Eigen::Vector3d(   item[0],
//                                             item[1] + g*t*t/10 + std::abs(item.norm() - midNorm)/10,
//                                             item[2]));
//     }
//     return result;
// }