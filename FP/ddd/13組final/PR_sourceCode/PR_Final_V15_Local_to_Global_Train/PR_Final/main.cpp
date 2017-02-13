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

string folder_name = "TRAIN/LocalSVM/Local";


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
	mkdir("TEST");
	PCA pca(global_classifier, Mat(), CV_PCA_DATA_AS_ROW, global_classifier.rows-6);
	FileStorage fs_pca("TEST/GlobalPCA.yml", FileStorage::WRITE);
	pca_save(fs_pca, pca);
	fs_pca.release();
	global_classifier = pca.project(global_classifier);
	//do LDA
	LDA_custom lda = LDA_proc(global_classifier);
	FileStorage fs_lda("TEST/GlobalLDA.yml", FileStorage::WRITE);
	lda.save(fs_lda); 
	fs_lda.release();
	global_classifier = LDA_proj(global_classifier, lda);
	//normalize for each row
	//for(int i=0;i<global_classifier.rows; i++)
	//	cv::normalize(global_classifier.row(i), global_classifier.row(i), -1, 1, NORM_MINMAX, CV_64F);
	//create .svm file 
	mkdir("TRAIN/GlobalSVM");
	Mat_to_svm(global_classifier, "Train/GlobalSVM/GlobalSVM");
}