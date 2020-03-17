#include "object.hpp"

std::vector<Eigen::Vector3d> deform(std::vector<Eigen::Vector3d> ref_X){
    std::vector<Eigen::Vector3d> result;
    for (auto item : ref_X) result.push_back(Eigen::Vector3d(item[0], item[1], item[2] * 1.5));
    return result;
}

int serialize(int x, int y, int z){
    return x*(SIZE + 1)*(SIZE + 1) + y*(SIZE + 1) + z;
}

void load_obj(std::vector<Eigen::Vector3d> &nodes, std::vector<Eigen::Vector4i> &tetras){

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

}