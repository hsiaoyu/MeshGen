//This fuke refines the traingular mesh by replacing one triangular mesh by four tri meshes 
#include <iostream>
#include <cmath>
#include <igl/readOBJ.h>
#include <igl/writeOBJ.h>
using namespace std;
using namespace Eigen;
MatrixXd V;// ENbar is the edge normal of Vbar
MatrixXi F; // Eash row of EdgeNV(3*NTri,4) stores the index off the four vertices that are related to one edge
int main (){
    igl::readOBJ("Vin.obj", V, F);
    int ini_nTri = F.rows();
    int ini_nVerts = V.rows();
    int nTri, nVerts;
    nTri = ini_nTri;
    nVerts = ini_nVerts;
    MatrixXd Vnew(10*nVerts,3);
    MatrixXi Fnew(10*nTri,3);
    Vnew.block(0,0,nVerts,3) = V;
    Fnew.block(0,0,nTri,3) = F;
    for (int i =0; i<ini_nTri; i++){
        Vector3d curV1, curV2, curV3, newV1, newV2, newV3;
        int f1, f2, f3, nf[3], nrep;
        vector<Vector3d> newV;
        f1 = F(i,0);
        f2 = F(i,1);
        f3 = F(i,2);
        curV1 = V.row(f1);
        curV2 = V.row(f2);
        curV3 = V.row(f3);
        newV.push_back(0.5*(curV1+curV2));
        newV.push_back(0.5*(curV3+curV2));
        newV.push_back(0.5*(curV1+curV3));
        for(int j =0 ; j <3; j++){
           Vnew.row(nVerts) = newV[j];
           nf[j] = nVerts;
           for(int k = ini_nVerts; k < nVerts; k++){
              if(newV[j](0) == Vnew(k,0) && newV[j](1) == Vnew(k,1) && newV[j](2) == Vnew(k,2)){
                 nVerts --;
                 nf[j] = k;
              }
           }
           nVerts++;
        }

      
        Fnew.row(i) << nf[0], nf[1], nf[2];
        Fnew.row(nTri) << f1, nf[0], nf[2];  
        Fnew.row(nTri+1) << nf[0], f2, nf[1];  
        Fnew.row(nTri+2) << nf[2], nf[1], f3;  
        nTri = nTri+3;
   }	
   Vnew.conservativeResize(nVerts,3); 
   Fnew.conservativeResize(nTri,3); 
   igl::writeOBJ("V_Refined.obj",Vnew,Fnew);
   return 0;
}
