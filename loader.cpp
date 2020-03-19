#include "loader.hpp"
#include "object.hpp"
#include "include/tetgen/tetgen.h" 
#include <eigen3/Eigen/Eigen>
#include <iostream>
#include <fstream>
#include <string>

// !!! IMPORTANT: ONLY THE FIRST SHAPE WILL BE LOADED

// cut string %%%a$$$ into %%%a and $$$
std::string cutAt(std::string& s, char a) {
    int index = s.find_first_of(a);
    if(index == std::string::npos)
        index = s.size();
    else
        index ++; // also cut char a
    std::string ret = s.substr(0, index);
    s = s.substr(index, s.size() - index);
    return ret;
}

// clean string aa%%% into %%%
std::string& clean(std::string& s, char a) {
    while(s.size() > 0 && s[0] == a)
        s = s.substr(1, s.size()-1);
    return s;
}

// cut string %%%aa$$$ into %%% and $$$
std::string cleanCut(std::string& s, char a) {
    for(int i = 0; i < s.size(); i++) {
        if(s[i] == a) {
            std::string ret = s.substr(0, i);
            while(s[i] == a)
                i++;
            s = s.substr(i, s.size() - i);
            return ret;
        }
    }
    std::string sCopy = s;
    s = std::string("");
    return sCopy;
}

void readObjFile(const std::string filename, std::vector<double>& vertices, std::vector<std::vector<int>>& polygons) {

    std::ifstream file(filename);
    if(!file.good()) return;
    while(!file.eof()) {
        std::string line;
        getline(file, line);

        // extract this line's datatype
        std::string datatype = cutAt(line, ' ');
        clean(line, ' ');

        // vertice "v "
        if(datatype.length() >= 2 && datatype[0] == 'v' && datatype[1] == ' ') {
            for(int i = 0; i < 3; i++) {
                std::string v = cleanCut(line, ' ');
                vertices.push_back(std::stod(v));
            }
        }

        // polygon "f "
        // format: v | v/vt | v/vt/vn
        if(datatype.length() >= 2 && datatype[0] == 'f' && datatype[1] == ' ') {
            std::vector<int> polygon;
            while(!line.empty()) {
                std::string vTuple = cleanCut(line, ' '); // vTuple looks like "1/0/1"
                std::string v = cleanCut(vTuple, '/');
                polygon.push_back(std::stoi(v));
            }
            polygons.push_back(polygon);
        }
    }

    file.close();
    std::cout << "readObjFile: " << vertices.size() << " vertices, " << polygons.size() << " polygons." << std::endl;
}


void tetrahedralize(std::vector<double> vertices, std::vector<std::vector<int>> polygons, std::vector<Eigen::Vector3d>& nodes, std::vector<Eigen::Vector4i>& tetras) {

    tetgenio in, out;

    // Prepare input for tetgen
    // nodes
    in.numberofpoints = vertices.size() / 3;
    in.pointlist = new REAL[vertices.size()];
    for(int i = 0; i < vertices.size(); i++) {
        in.pointlist[i] = vertices[i];
    }

    // faces
    in.firstnumber = 0; // node index starts from 0
    in.numberoffacets = polygons.size();
    in.facetlist = new tetgenio::facet[in.numberoffacets];
    for(int i = 0; i < polygons.size(); i++) {
        std::vector<int> polygon = polygons[i];
        tetgenio::facet* f = &(in.facetlist[i]);
        f->numberofpolygons = 1;
        f->numberofholes = 0;
        f->polygonlist = new tetgenio::polygon[1];
        f->holelist = NULL;
        tetgenio::polygon* p = &f->polygonlist[0];
        p->numberofvertices = polygon.size();
        p->vertexlist = new int[p->numberofvertices];
        for(int j = 0; j < p->numberofvertices; j++) {
            p->vertexlist[j] = polygon[j] - 1; // -1 because .obj file starts from 1
        }
    }

    // Print for debugging 
    // for(int i = 0 ; i < in.numberoffacets; i++) {
    //     tetgenio::facet* f = &(in.facetlist[i]);
    //     tetgenio::polygon* p = &f->polygonlist[0];
    //     for(int j = 0 ; j < p->numberofvertices; j++) {
    //         int index = p->vertexlist[j] - 1;
    //         std::cout<<'('<<in.pointlist[index*3]<<' '<<in.pointlist[index*3+1]<<' '<<in.pointlist[index*3+2]<<')';
    //     }
    //     std::cout<<std::endl;
    // }

    // facetmarkerlist. unimportant
    in.facetmarkerlist = new int[in.numberoffacets];
    for(int i = 0; i < in.numberoffacets; i++)
        in.facetmarkerlist[i] = 0;

    // Tetrahedralize the PLC. Switches are chosen to read a PLC (p),
    //   do quality mesh generation (q) with a specified quality bound
    //   (1.414), and apply a maximum volume constraint (a0.1).
    tetrahedralize("pq2a0.01O/7", &in, &out);

    // Output files for debugging
    // out.save_nodes("tetgen_tmp");
    // out.save_faces("tetgen_tmp");
    // out.save_edges("tetgen_tmp");

    // Create nodes and tetras from out
    for(int i = 0; i < out.numberofpoints; i++) {
        double x = out.pointlist[i*3];
        double y = out.pointlist[i*3+1];
        double z = out.pointlist[i*3+2];
        Eigen::Vector3d n(x,y,z);
        nodes.push_back(n);
    }
    for(int i = 0; i < out.numberoftetrahedra; i++) {
        int x = out.tetrahedronlist[i*4];
        int y = out.tetrahedronlist[i*4+1];
        int z = out.tetrahedronlist[i*4+2];
        int w = out.tetrahedronlist[i*4+3];
        Eigen::Vector4i t(x,y,z,w);
        tetras.push_back(t);
    }

}

// deprecated
/*
Object readTetgenFile(char* filename) {
    Object obj;

    std::ifstream nodeFile(std::string(filename) + ".node");
    if(!nodeFile.good() || nodeFile.eof()) {
        std::cout<<"Error in loader.cpp::readTetgenFile: nodeFile is not good or eof."<<std::endl;
        return obj;
    }
    std::string line;
    getline(nodeFile, line); // first line
    int numVertices = std::stoi(cleanCut(line, ' '));
    for(int ln = 0; ln < numVertices && !nodeFile.eof(); ln++) {
        getline(nodeFile, line);
        cleanCut(line, ' '); // useless index
        double x = stod(cleanCut(line, ' '));
        double y = stod(cleanCut(line, ' '));
        double z = stod(cleanCut(line, ' '));
        Eigen::Vector3d v(x,y,z);
        obj.nodes.push_back(v);
    }
    nodeFile.close();

    std::ifstream eleFile(std::string(filename) + ".ele");
    if(!eleFile.good() || eleFile.eof()) {
        std::cout<<"Error in loader.cpp::readTetgenFile: eleFile is not good or eof."<<std::endl;
        return obj;
    }
    std::string line;
    getline(eleFile, line); // first line
    int numVertices = std::stoi(cleanCut(line, ' '));
    for(int ln = 0; ln < numVertices && !eleFile.eof(); ln++) {
        getline(eleFile, line);
        cleanCut(line, ' '); // useless index
        int x = stod(cleanCut(line, ' '));
        int y = stod(cleanCut(line, ' '));
        int z = stod(cleanCut(line, ' '));
        int w = stod(cleanCut(line, ' '));
        Eigen::Vector4i t(x,y,z,w);
        obj.tetras.push_back(t);
    }
    eleFile.close();

    return obj;
}
*/
