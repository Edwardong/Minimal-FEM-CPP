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

void readObjFile(const char* filename, std::vector<double>& vertices, std::vector<std::vector<int>>& polygons) {

    std::ifstream file(filename);
    if(file.bad()) return;
    while(!file.eof()) {
        std::string line;
        getline(file, line);

        std::string datatype = cutAt(line, ' ');
        clean(line, ' ');
        // vertice 
        if(datatype.length() >= 2 && datatype[0] == 'v' && datatype[1] == ' ') {
            for(int i = 0; i < 3; i++) {
                std::string v = cleanCut(line, ' ');
                vertices.push_back(std::stod(v));
            }
        }

        // polygon
        // format: v | v/vn | v/vn/vt
        if(datatype.length() >= 2 && datatype[0] == 'f' && datatype[1] == ' ') {
            std::vector<int> polygon;
            while(!line.empty()) {
                std::string vTuple = cleanCut(line, ' ');
                std::string v = cutAt(vTuple, '/');
                v.pop_back();
                polygon.push_back(std::stoi(v));
            }
            polygons.push_back(polygon);
        }
    }
    file.close();
    std::cout<<"loadObjFile: "<<vertices.size()<<" vertices, "<<polygons.size()<<" polygons."<<std::endl;
}

void tetgen(char* filename, std::vector<double> vertices, std::vector<std::vector<int>> polygons) {

    tetgenio in, out;
    in.firstnumber = 1;
    // nodes
    in.numberofpoints = vertices.size() / 3;
    // in.pointlist = vertices.data(); // tetgen REAL == double
    in.pointlist = new REAL[vertices.size()];
    // std::copy(vertices.begin(), vertices.end(), in.pointlist);
    for(int i = 0; i < vertices.size(); i++) {
        in.pointlist[i] = vertices[i];
    }


    // faces
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
            p->vertexlist[j] = polygon[j];
        }
    }

    // for(int i = 0 ; i < in.numberoffacets; i++) {
    //     tetgenio::facet* f = &(in.facetlist[i]);
    //     tetgenio::polygon* p = &f->polygonlist[0];
        
    //     for(int j = 0 ; j < p->numberofvertices; j++) {
    //         int index = p->vertexlist[j] - 1;
    //         std::cout<<'('<<in.pointlist[index*3]<<' '<<in.pointlist[index*3+1]<<' '<<in.pointlist[index*3+2]<<')';
    //     }
    //     std::cout<<std::endl;
    // }

    in.facetmarkerlist = new int[in.numberoffacets];
    for(int i = 0; i < in.numberoffacets; i++)
        in.facetmarkerlist[i] = 0;

    // Output the PLC to files 'barin.node' and 'barin.poly'.
    // in.save_nodes(filename);
    // in.save_poly(filename);

    // Tetrahedralize the PLC. Switches are chosen to read a PLC (p),
    //   do quality mesh generation (q) with a specified quality bound
    //   (1.414), and apply a maximum volume constraint (a0.1).
    tetrahedralize("pq1.414a0.1k", &in, &out);

    std::string outputFilenameString = std::string("tmp_") + std::string(filename);
    char* outputFilename = new char[outputFilenameString.size() + 1];
    outputFilenameString.copy(outputFilename, outputFilenameString.size() + 1);
    out.save_nodes(outputFilename);
    out.save_elements(outputFilename);
    // out.save_faces(outputFilename);

}

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




bool load(std::vector<Object>* X, const char* filename) {

    
    std::vector<double> vertices;
    std::vector<std::vector<int>> polygons;
    loadObjFile(filename, vertices, polygons);


}

