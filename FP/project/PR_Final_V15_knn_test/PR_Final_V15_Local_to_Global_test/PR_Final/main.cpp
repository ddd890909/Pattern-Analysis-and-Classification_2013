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

string folder_name = "Local_0.lda";

int main(){
	fstream con, cat;
	vector<int> all_cat;
	cat.open("TEST/TEST.cat", ios::in);
	int tmp;
	while(cat >> tmp ){
		all_cat.push_back(tmp);
	}
	cat.close();
	Mat feature(all_cat.size(), 66*6, CV_32F, 1);
	con.open("TEST/LocalKNN/knnResult.knn", ios::in);
	for(int i=0;i<all_cat.size();i++){
		string kerker;
		getline(con, kerker);
		stringstream ss(kerker);
		for(int j=0;j<6*66;j++){
			ss >> feature.at<float>(i,j);
		}
	}


	FileStorage fs_pca("TEST/GlobalPCA.yml", FileStorage::READ);
	PCA pca = pca_load(fs_pca);
	fs_pca.release();
	feature = pca.project(feature);

	LDA_custom lda;
	FileStorage fs_lda("TEST/GlobalLDA.yml", FileStorage::READ);
	lda.load(fs_lda); 
	fs_lda.release();
	feature = LDA_proj(feature, lda);

	mkdir("TEST/GlobalSVM");
	Mat_to_svm(feature, "TEST/GlobalSVM/GlobalSVMTEST", all_cat);

	//for(int i=0;i<all_cat.size();i++){
	//	string kerker;
	//	getline(con, kerker);
	//	stringstream ss(kerker);
	//	svm << all_cat[i] << " ";
	//	for(int j=0;j<66*6;j++){
	//		float tmp;
	//		ss >> tmp;
	//		svm << j << ":" << tmp << " "; 
	//	}
	//	svm << endl;
	//}
}