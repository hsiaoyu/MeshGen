#include <stdlib.h>
#include <igl/readOBJ.h>
#include <igl/writeOBJ.h>
#include <Eigen/Geometry>
using namespace std;
int main() {
    Eigen::MatrixXd V;
    Eigen::MatrixXi F;
    igl::readOBJ("V_ini.obj",V,F);
    int NNode;
    NNode = V.rows();
    for (int i=0; i<NNode; i++){
	int random = rand() % 9;
        V(i,2) += random * 0.00001;
    }
    igl::writeOBJ("V_perturb.obj",V,F);
    return 0;
}
