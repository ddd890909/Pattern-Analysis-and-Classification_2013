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

string folder_name = "TRAIN/LocalKNN/Local_0.lda";

int main(){
	fstream con, cat;
	vector<int> all_cat;
	con.open("TRAIN/LocalKNN/Local_knnReslt.knn",ios::in);
	for(int i=0;i<6;i++){
		cat.open(folder_name+int2str(i+1)+".cat",ios::in);
		int tmp;
		while(cat >> tmp){
			all_cat.push_back(tmp);
		}
		cat.close();
	}

	Mat feature(all_cat.size(), 66*6, CV_32F, 1);

	for(int i=0;i<all_cat.size();i++){
		string kerker;
		getline(con, kerker);
		stringstream ss(kerker);
		for(int j=0;j<66*6;j++){
			ss >> feature.at<float>(i,j); 
		}
	}

	con.close();

	mkdir("TEST");
	PCA pca(feature, Mat(), CV_PCA_DATA_AS_ROW, feature.rows-6);
	FileStorage fs_pca("TEST/GlobalPCA.yml", FileStorage::WRITE);
	pca_save(fs_pca, pca);
	fs_pca.release();
	feature = pca.project(feature);

	LDA_custom lda = LDA_proc(feature);
	FileStorage fs_lda("TEST/GlobalLDA.yml", FileStorage::WRITE);
	lda.save(fs_lda); 
	fs_lda.release();
	feature = LDA_proj(feature, lda);

	mkdir("TRAIN/GlobalSVM");
	Mat_to_svm(feature, "Train/GlobalSVM/GlobalSVM", all_cat);

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