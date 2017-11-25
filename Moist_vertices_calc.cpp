// This file generate the machine direction in barycentric coordinate of each face and store in MDinFace.dmat
// The first NTri rows of Moisture is for the upper surface the other half(Ntri+1-2*Ntri) is for the lower surface 
// Read out MDinFace.dmat using readDMAT and each row of the matrix is the barycentric coordinate representation of MD 
#include <iostream>
#include <fstream>
#include <cmath>
#include <igl/readOBJ.h>
#include <igl/writeOBJ.h>
#include <igl/readDMAT.h>
#include <igl/writeDMAT.h>
using namespace std;
using namespace Eigen;
MatrixXd V;// ENbar is the edge normal of Vbar
MatrixXi F; // Eash row of EdgeNV(3*NTri,4) stores the index off the four vertices that are related to one edge
int main (){
    igl::readOBJ("V_Moist.obj", V, F);
    igl::writeOBJ("spoon.obj", V, F);
    VectorXd Moisture(V.rows()*2);
    int mode;
    cout << "Which Mode : 0- Top/Bottom Uniformly Different, 1- Wet Rim, 2- Wet Center, 3- Y coord Dependence" << endl;
    cout << "4- Specific Wet Area 5- Point Source" << endl;
    cin >> mode;
    double topmoist, botmoist, moist, rad;    
    double xup, xdown, yup, ydown, zup, zdown, moist_top, moist_bottom;
    switch(mode){
	case 0: 
	      //-----------The following code generates the uniform distributed moisture level----------------------
    	    cout << "What is the top surface moisture level(%):(positive for lower moisture level and negative for higher)" << endl;
    	    cin >> topmoist;
    	    cout << "What is the bottom surface moisture level(%):(positive for lower moisture level and negative for higher)" << endl;
    	    cin >> botmoist;
            for(int i=0; i< V.rows(); i++){
	        Moisture(i)=topmoist;
	        Moisture(i+V.rows())=botmoist;
            }
            break;
         case 1:
	    //--------- The following lines generate the moisture level based on the distance from a circular disc.------- 
            cout << "What is the radius:(usually is 1)" << endl;
            cin >> rad;
    	    cout << "What is the moisture difference(%):(positive for lower moisture level and negative for higher)" << endl;
    	    cin >> moist;
            for(int i=0; i<V.rows(); i++){
                Vector3d q;
                q = V.row(i);
	        //center has zero moist and the rim has moisture leverl at moist
	        Moisture(i)=q.norm()*moist/rad;
	        Moisture(i+V.rows())=Moisture(i);
            }
	    break;
         case 2:
	    //--------- The following lines generate the moisture level based on the distance from a circular disc.------- 
            cout << "What is the radius:(usually is 1)" << endl;
            cin >> rad;
    	    cout << "What is the moisture difference(%):(positive for lower moisture level and negative for higher)" << endl;
    	    cin >> moist;
            for(int i=0; i<V.rows(); i++){
                Vector3d q;
                q = V.row(i);
	        //the rim  has zero moist and the center has moisture level at moist
	        Moisture(i)=moist-q.norm()*moist/rad;
	        Moisture(i+V.rows())=Moisture(i);
            }
	    break;
	 case 3:
             // --------------------The Moisture content is dependent on the y coordinate-------------------------
             cout << "What is the upper limit of the y coordinate that should be wet" << endl;
             cin >> yup;
             cout << "What is the lower limit of the y coordinate that should be wet" << endl;
             cin >> ydown;
             cout << "What is moisture content of the affected area(%):(positive for lower moisture level and negative for higher)" << endl;
             cin >> moist;
	     Moisture.setZero();
	     for (int i=0; i<V.rows(); i++){
		if(V(i,1)<=yup && V(i,1)>=ydown){
	           Moisture(i)=moist;
	           Moisture(i+V.rows())=moist;
		}
	     }
             break;
         
        case 4:
             cout << "What is the upper limit of the x coordinate that should be wet" << endl;
             cin >> xup;
             cout << "What is the lower limit of the x coordinate that should be wet" << endl;
             cin >> xdown;
             cout << "What is the upper limit of the y coordinate that should be wet" << endl;
             cin >> yup;
             cout << "What is the lower limit of the y coordinate that should be wet" << endl;
             cin >> ydown;
             cout << "What is the upper limit of the z coordinate that should be wet" << endl;
             cin >> zup;
             cout << "What is the lower limit of the z coordinate that should be wet" << endl;
             cin >> zdown;
             cout << "What is moisture content of the bottom area(%):(positive for lower moisture level and negative for higher)" << endl;
             cin >> moist_bottom;
             cout << "What is moisture content of the top area(%):(positive for lower moisture level and negative for higher)" << endl;
             cin >> moist_top;
	     Moisture.setZero();
	     for (int i=0; i<V.rows(); i++){
		if(V(i,0)<=xup && V(i,0)>=xdown && V(i,1)<=yup && V(i,1)>=ydown && V(i,2)<=zup && V(i,2)>=zdown){
	           Moisture(i)=moist_top;
	           Moisture(i+V.rows())=moist_bottom;
		}
	     }
             break;
        // The moisture(temperature)is inversely porpotional to the distance to the power defined by user from the source
        case 5:
             RowVector3d Source;
             double s, dis, power;
             cout << "What is the x coordinate of the source" << endl;
             cin >> s;
             Source(0) = s;
             cout << "What is the y coordinate of the source" << endl;
             cin >> s;
             Source(1) = s;
             cout << "What is the z coordinate of the source" << endl;
             cin >> s;
             Source(2) = s;
             cout << "Moisture is porportional to the distance of what power?" << endl;
             cin >> power;
             cout << "What is moisture content at the source(%):(positive for lower moisture level and negative for higher)" << endl;
             cin >> moist;
             for(int i =0; i<V.rows(); i++){
               dis = (V.row(i)-Source).norm();
               Moisture(i) = moist / pow(dis,power);
               Moisture(i+V.rows()) = moist / pow(dis,power);
             }
             break;
    }
  
  igl::writeDMAT("MoistureLevel_v.dmat",Moisture,1);
    return 0;
}
