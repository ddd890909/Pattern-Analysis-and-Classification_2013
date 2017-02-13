#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
//defined library
#include "gabor.h"
#include "faceDefine.h"
#include "work.h"
//library
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <fstream>
#include <stack>


using namespace cv;
using namespace std;


FaceDetection *face_detector;
OpenCVASMAlignment *myAlignment;

int vec_num = 22*18*5*3*8;
string folder_name="JAFFE_test";


int main(int argc, char* argv[]){
	if(argc > 1) folder_name = argv[1];
	face_detector = new FaceDetection("cascade Data\\haarcascade.xml");
    myAlignment = new OpenCVASMAlignment();
	myAlignment->setModelPath("ASM Data/FrontalFace_best.amf");
	if (myAlignment->loadModel() == -1) {
		fprintf(stderr,"Could not load ASM model...\n");
	}
	

	//stage_1 and stage_2
	/*//faceExpression用 會巢狀探測一個資料夾
	vector<string> folder_1 = get_file_names(folder_name);
	for(int i=0;i<folder_1.size();i++){
		stage_1(folder_1[i], folder_1[i]+"/cropped");
		stage_2(folder_1[i]+"/cropped", folder_name);
	}
	*/
		//for training data
	string category = "TEST/GlobalSVM/GlobalSVMTEST.result";
	stage_1(category, folder_name);
	return 0;
}


