//#include "stdlib.h"
#include "formula.hpp"

void precompute(std::vector<Eigen::Vector3d> X,
                std::vector<Eigen::Vector4i> T,
                std::vector<Eigen::Matrix3d> &B,
                std::vector<double> &W){
    for (auto &t : T){
        Eigen::Vector3d i =  X[t[0]];
        Eigen::Vector3d j =  X[t[1]];
        Eigen::Vector3d k =  X[t[2]];
        Eigen::Vector3d l =  X[t[3]];
        Eigen::Matrix3d D;
        D<< i[0]-l[0], j[0]-l[0], k[0]-l[0],
            i[1]-l[1], j[1]-l[1], k[1]-l[1],
            i[2]-l[2], j[2]-l[2], k[2]-l[2];
        B.push_back(D.inverse());
        W.push_back(abs(D.determinant() / 6.0));
    }
    return;
}

//constitutive law
Eigen::Matrix3d compute_P(Eigen::Matrix3d F){
    Eigen::Matrix3d E = 0.5 * (F.transpose() * F - I3);
    return (F * (2*MU*E + LAMBDA*E.trace()*I3));
}

std::vector<Eigen::Vector3d> negative_V(std::vector<Eigen::Vector3d> V){
    std::vector<Eigen::Vector3d> nV;
    for(std::vector<Eigen::Vector3d>::iterator it = V.begin(); it != V.end(); ++it)
        nV.push_back(-*it);
    return nV;
}

std::vector<Eigen::Vector3d> compute_F( std::vector<Eigen::Vector3d> def_X,
                                        std::vector<Eigen::Vector4i> T,
                                        std::vector<Eigen::Matrix3d> B,
                                        std::vector<double> W){
    //TO-Check
    //std::vector<Eigen::Vector3d> f(def_X.size(), Eigen::Vector3d(0,0,0));
    std::vector<Eigen::Vector3d> f;
    for(size_t i =0; i < def_X.size(); i++){
        Eigen::Vector3d temp(0,0,0);
        f.push_back(temp);
    }
    for(size_t m = 0; m <T.size(); m++){
        Eigen::Vector3d i = def_X[T[m][0]];
        Eigen::Vector3d j = def_X[T[m][1]];
        Eigen::Vector3d k = def_X[T[m][2]];
        Eigen::Vector3d l = def_X[T[m][3]];
        Eigen::Matrix3d D;
        D<< i[0]-l[0], j[0]-l[0], k[0]-l[0],
            i[1]-l[1], j[1]-l[1], k[1]-l[1],
            i[2]-l[2], j[2]-l[2], k[2]-l[2];
        Eigen::Matrix3d F = D * B[m];
        Eigen::Matrix3d P = compute_P(F);

        //use row 0, 1, 2 as h1,h2,h3
        Eigen::Matrix3d H = ((-1.0) * W.at(m)) * P * B[m].transpose();
        f[T[m][0]] += H.col(0);
        f[T[m][1]] += H.col(1);
        f[T[m][2]] += H.col(2);
        f[T[m][3]] += ((-1) * H.col(0) - H.col(1) - H.col(2));
    }
    return f;
}


Eigen::Matrix3d compute_dP(Eigen::Matrix3d F, Eigen::Matrix3d dF){
    //St. VK
    Eigen::Matrix3d E = 0.5 * (F.transpose() * F - I3);
    Eigen::Matrix3d dE = 0.5 * (dF.transpose() * F + F.transpose() * dF);
    Eigen::Matrix3d dP = dF * (2*MU*E + LAMBDA*E.trace()*I3) + F*
                        (2*MU*dE + LAMBDA*dE.trace()*I3);
    return dP;
}


std::vector<Eigen::Vector3d> compute_dF(std::vector<Eigen::Vector3d> def_X,
                                        std::vector<Eigen::Vector3d> dX,
                                        std::vector<Eigen::Vector4i> T,
                                        std::vector<Eigen::Matrix3d> B,
                                        std::vector<double> W){
    //TO-Check
    //std::vector<Eigen::Vector3d> f(def_X.size(), Eigen::Vector3d(0,0,0));
    std::vector<Eigen::Vector3d> df;
    for(size_t i =0; i < def_X.size(); i++){
        Eigen::Vector3d temp(0,0,0);
        df.push_back(temp);
    }

    for(size_t m = 0; m <T.size(); m++){
        Eigen::Vector3d i = def_X[T[m][0]];
        Eigen::Vector3d j = def_X[T[m][1]];
        Eigen::Vector3d k = def_X[T[m][2]];
        Eigen::Vector3d l = def_X[T[m][3]];
        Eigen::Matrix3d D;
        D<< i[0]-l[0], j[0]-l[0], k[0]-l[0],
            i[1]-l[1], j[1]-l[1], k[1]-l[1],
            i[2]-l[2], j[2]-l[2], k[2]-l[2];

        i = dX[T[m][0]];
        j = dX[T[m][1]];
        k = dX[T[m][2]];
        l = dX[T[m][3]];
        Eigen::Matrix3d dD;
        dD<<i[0]-l[0], j[0]-l[0], k[0]-l[0],
            i[1]-l[1], j[1]-l[1], k[1]-l[1],
            i[2]-l[2], j[2]-l[2], k[2]-l[2];
        Eigen::Matrix3d F = D * B[m];
        Eigen::Matrix3d dF = dD * B[m];
        Eigen::Matrix3d dP = compute_dP(F,dF);
        //use row 0, 1, 2 as h1,h2,h3
        Eigen::Matrix3d dH = ((-1.0) * W.at(m)) * dP * B[m].transpose();

        df[T[m][0]] += dH.col(0);
        df[T[m][1]] += dH.col(1);
        df[T[m][2]] += dH.col(2);
        df[T[m][3]] += ((-1) * dH.col(0) - dH.col(1) - dH.col(2));
    }
    return df;
}

std::vector<Eigen::Vector3d> update_XV( std::vector<Eigen::Vector3d> &def_X,
                                        std::vector<Eigen::Vector4i> T,
                                        std::vector<Eigen::Vector3d> &V,
                                        std::vector<Eigen::Matrix3d> B,
                                        std::vector<double> W){
    std::vector<Eigen::Vector3d> Fe = compute_F(def_X, T, B, W);
    //debug
    std::vector<Eigen::Vector3d> nV = negative_V(V);
    // std::cout << "this is V" << std::endl;
    // std::cout << V[0] << std::endl;
    // std::cout << "this is nV" << std::endl;
    // std::cout << nV[0] << std::endl;

    std::vector<Eigen::Vector3d> Fd = compute_dF(def_X, nV, T, B, W);
    std::vector<Eigen::Vector3d> F;
    for (size_t i =0; i < Fd.size(); i++){
        Fd[i] *= (-1) * GAMMA;
        //F.push_back(Fe[i]);
        F.push_back(Fe[i] + Fd[i]);
    }
    
    for (size_t j = 0; j < def_X.size(); j++){
        Eigen::Vector3d acceleration = F[j] / MASS;
        acceleration += GRAVITY;
        // v' = v + dt * F / M
        V[j] += DELTA_TIME * acceleration;
        // x' = x + dt * v
        def_X[j] += DELTA_TIME * V[j];

        //ground
        if (def_X[j][2] < 0){
            def_X[j][2] = 0;
            V[j][2] *= -COEF_OF_RESTITUTION;
        } 
    }
    return Fe;
}