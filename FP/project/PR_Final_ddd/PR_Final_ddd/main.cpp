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
#include<fstream>


using namespace cv;
using namespace std;


int main(int argc, char** argv){
	string source_folder, dest_folder;
	cout << "Please input your source folder: ";
	cin >> source_folder;
	cout << "Please input your source folder: ";
	cin >> dest_folder;
	stage_1(source_folder, dest_folder);
	stage_2(dest_folder);
}
