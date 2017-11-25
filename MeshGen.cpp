#include <cmath>
#include <iostream>
#include <fstream>
#include <igl/readOBJ.h>
#include <igl/viewer/Viewer.h>
#include <Eigen/Geometry>
using namespace std;
int main() {
    ifstream Infile1("In.node"), Infile2("In.ele");
    string filename;
    cout << "Enter output file name(please include .obj):" << endl;
    cin >> filename;
    ofstream Outfile(filename);
    int NNode, NTri;
    double dummy, vpos1, vpos2, vpos3;
    Infile1 >> NNode >> dummy >> dummy >> dummy;
    for (int i=0; i<NNode; i++){
    	Infile1 >> dummy >> vpos1 >> vpos2 >> dummy ;    	
        Outfile << "v " << vpos1 << " " << vpos2 << " 0" << endl;
	//cout << vpos1 <<" " << vpos2 << endl;
    }
    Infile2 >> NTri >> dummy >> dummy;
    for (int i=0; i<NTri; i++){
	int v1, v2, v3;
    	Infile2 >> dummy >> v1 >> v2 >> v3;    	
        Outfile << "f " << v1 << " " << v2 << " " << v3 << endl;
    }
    Eigen::MatrixXd V;
    Eigen::MatrixXi F;
    igl::readOBJ(filename,V,F);
    igl::viewer::Viewer viewer;
    viewer.data.set_mesh(V, F);
    viewer.launch();
    return 0;
}
