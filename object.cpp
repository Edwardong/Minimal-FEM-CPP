#include "object.hpp"
#include "loader.hpp"
#include <iostream>


std::vector<Eigen::Vector3d> scale(std::vector<Eigen::Vector3d> nodes){
    double min = nodes[0][2];
    double max = nodes[0][2];
    for(auto n : nodes) {
        if(n[2] < min) min = n[2];
        if(n[2] > max) max = n[2];
    }
    double height = max - min;
    std::vector<Eigen::Vector3d> result;
    for (auto n : nodes) {
        result.push_back(n / height * SIZE);
    }
    return result;
}


std::vector<Eigen::Vector3d> deform(std::vector<Eigen::Vector3d> ref_X){
    // Move (z direction) so that its bottom is 20% of its height above ground (xy-plane)
    double min = ref_X[0][2];
    double max = ref_X[0][2];
    for(auto n : ref_X) {
        if(n[2] < min) min = n[2];
        if(n[2] > max) max = n[2];
    }
    double height = max - min;
    std::vector<Eigen::Vector3d> result;
    for (auto item : ref_X) {
        result.push_back(Eigen::Vector3d(item[0], item[1], (item[2] - min + height*0.20)));
    }
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



// Object::Object(){}

Object::Object(std::vector<Eigen::Vector3d> nodes, std::vector<Eigen::Vector4i> tetras) {
    this->nodes = nodes;
    this->tetras = tetras;
    this->refNodes = nodes;
    for(int i = 0; i < nodes.size(); i++) {
        velocities.push_back(Eigen::Vector3d(0,0,0));
    }
}

void Object::translate(Eigen::Vector3d displacement) {
    for(auto n: nodes) {
        n += displacement;
    }
}

double Object::volumn() const {
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
        << obj.tetras.size() << " tetras. " 
        << "Volumn: " << obj.volumn() << ". " << std::endl;
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


// facea[i] has vn of normals[i] for all 3 vertices
// for some obj viewer with backface culling, the front normal of ABC is ABxAC
void Object::generateFacesAndNormals(std::vector<Eigen::Vector3d>& normals, std::vector<Eigen::Vector3i>& faces) {
    Eigen::Vector3d n;
    for(auto tetra : tetras){
        // face 012
        n = (nodes[tetra[1]] - nodes[tetra[0]]).cross(nodes[tetra[2]] - nodes[tetra[0]]).normalized();
        if(n.dot(nodes[tetra[3]] - nodes[tetra[0]]) < 0) {
            normals.push_back(n);
            faces.push_back(Eigen::Vector3i(tetra[0], tetra[1], tetra[2]));
        } else {
            normals.push_back(-n);
            faces.push_back(Eigen::Vector3i(tetra[0], tetra[2], tetra[1]));
        }
        // face 013
        n = (nodes[tetra[1]] - nodes[tetra[0]]).cross(nodes[tetra[3]] - nodes[tetra[0]]).normalized();
        if(n.dot(nodes[tetra[2]] - nodes[tetra[0]]) < 0) {
            normals.push_back(n);
            faces.push_back(Eigen::Vector3i(tetra[0], tetra[1], tetra[3]));
        } else {
            normals.push_back(-n);
            faces.push_back(Eigen::Vector3i(tetra[0], tetra[3], tetra[1]));
        }
        // face 023
        n = (nodes[tetra[2]] - nodes[tetra[0]]).cross(nodes[tetra[3]] - nodes[tetra[0]]).normalized();
        if(n.dot(nodes[tetra[1]] - nodes[tetra[0]]) < 0) {
            normals.push_back(n);
            faces.push_back(Eigen::Vector3i(tetra[0], tetra[2], tetra[3]));
        } else {
            normals.push_back(-n);
            faces.push_back(Eigen::Vector3i(tetra[0], tetra[3], tetra[2]));
        }
        // face 123
        n = (nodes[tetra[2]] - nodes[tetra[1]]).cross(nodes[tetra[3]] - nodes[tetra[1]]).normalized();
        if(n.dot(nodes[tetra[0]] - nodes[tetra[1]]) < 0) {
            normals.push_back(n);
            faces.push_back(Eigen::Vector3i(tetra[1], tetra[2], tetra[3]));
        } else {
            normals.push_back(-n);
            faces.push_back(Eigen::Vector3i(tetra[1], tetra[3], tetra[2]));
        }
    }
}

void Object::export_obj(int index, std::string out_dir){
    // 1. create file.
    std::ofstream myfile;
    std::string name = out_dir + "/out_" + std::to_string(index) + ".obj";
    myfile.open(name);

    // 2. write to file.
    // // 2.1 write vertices  
    myfile << "g default\n";
    for(auto vertex : nodes){
        myfile << "v " << vertex[0] << " " << vertex[1] << " " << vertex[2] << std::endl;
    }
    myfile << std::endl;

    // // 2.2 write vertex normals and tetrahedral faces
    // myfile << "g tetras\n";
    std::vector<Eigen::Vector3d> normals;
    std::vector<Eigen::Vector3i> faces;
    generateFacesAndNormals(normals, faces);
    // facea[i] has vn of normals[i] for all 3 vertices
    for(auto n : normals) {
        myfile << "vn " << n[0] << " " << n[1] << " " << n[2] << std::endl;
    }
    myfile << std::endl;

    // Turn on vn
    for(int i = 0; i < faces.size(); i++) {
        myfile << "f " 
            << faces[i][0]+1 << "//" << i+1 << " " 
            << faces[i][1]+1 << "//" << i+1 << " " 
            << faces[i][2]+1 << "//" << i+1 << std::endl;
    } 
    myfile << std::endl;

    // // Turn off vn
    // for(int i = 0; i < faces.size(); i++) {
    //     myfile << "f " 
    //         << faces[i][0]+1 << " " 
    //         << faces[i][1]+1 << " " 
    //         << faces[i][2]+1 << std::endl;
    // } 

    // 3. close the file.
    myfile.close();
}

