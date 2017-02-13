#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
//defined library
#include "work.h"
//library
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <fstream>
#include <stack>
#include <direct.h>

using namespace cv;
using namespace std;
int local_class_num = 66;

string folder_name = "TEST/LocalSVM/local";


int main(){
	vector<Mat>  local_classifier;
	//load local classifiers
	for(int i=0;i<local_class_num;i++){
		local_classifier.push_back(svm_to_Mat(folder_name+"_"+int2str(i)+".result"));

	}
	//concat to global_classifier
	Mat global_classifier;
	hconcat(local_classifier, global_classifier);
	//do PCA
	FileStorage fs_pca("TEST/GlobalPCA.yml", FileStorage::READ);
	PCA pca = pca_load(fs_pca);
	fs_pca.release();
	global_classifier = pca.project(global_classifier);
	//do LDA
	LDA_custom lda;
	FileStorage fs_lda("TEST/GlobalLDA.yml", FileStorage::READ);
	lda.load(fs_lda); 
	fs_lda.release();
	global_classifier = LDA_proj(global_classifier, lda);
	//for(int i=0;i<global_classifier.rows; i++)
	//	cv::normalize(global_classifier.row(i), global_classifier.row(i), -1, 1, NORM_MINMAX, CV_64F);
	//create .svm file 
	mkdir("TEST/GlobalSVM");
	Mat_to_svm(global_classifier, "TEST/GlobalSVM/GlobalSVMTEST");
}