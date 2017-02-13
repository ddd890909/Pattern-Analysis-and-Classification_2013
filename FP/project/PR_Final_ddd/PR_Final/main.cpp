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

#define _test 

FaceDetection *face_detector;
OpenCVASMAlignment *myAlignment;

int vec_num = 22*18*5*3*8;
string folder_name="faceExpression2011_JAFFE";
string folder_name_test="faceExpression2012";


//int main(){
//	Mat a = get_cat(folder_name);
//	cout << a.at<float>(0,0) << endl;
//	cout << a.at<float>(1,0) << endl;
//	cout << a.at<float>(2,0) << endl;
//}


int main(){
	face_detector = new FaceDetection("cascade Data\\haarcascade.xml");
    myAlignment = new OpenCVASMAlignment();
	myAlignment->setModelPath("ASM Data/FrontalFace_best.amf");
	if (myAlignment->loadModel() == -1) {
		fprintf(stderr,"Could not load ASM model...\n");
	}
	

	//stage_1 and stage_2
	//faceExpression用 會巢狀探測一個資料夾
	/*vector<string> folder_1 = get_file_names(folder_name);
	for(int i=0;i<folder_1.size();i++){
		stage_1(folder_1[i], folder_1[i]+"/cropped");
		stage_2(folder_1[i]+"/cropped", folder_name);
	}*/
	
		//for training data
	stage_1(folder_name,folder_name+"/cropped");
	stage_2(folder_name+"/cropped", folder_name);
#ifdef _test
		//for testing data
	stage_1(folder_name_test,folder_name_test+"/cropped");
	stage_2(folder_name_test+"/cropped", folder_name_test);
#endif
	//read data to mat before PCA
	cout << folder_name << " to mat" <<endl;
	vector<string> feature_name = get_file_names(folder_name+"/feature");
	int sample_num=feature_name.size();
	Mat pcaSet(sample_num, vec_num, CV_32F,1);
	for(int i=0;i<feature_name.size();i++){
		vector<float> feature_value = mat_to_feature(feature_name[i]);
		for(int j=0;j<feature_value.size();j++){
			pcaSet.at<float>(i,j)=feature_value[j];
		}
	}
#ifdef _test
	cout << folder_name_test << " to mat" <<endl;
	vector<string> feature_name_test = get_file_names(folder_name_test+"/feature");
	int sample_num_test=feature_name_test.size();
	//cout << sample_num_test << endl;
	Mat pcaSet_test(sample_num_test, vec_num, CV_32F,1);
	for(int i=0;i<feature_name_test.size();i++){
		//cout << i << endl;
		vector<float> feature_value = mat_to_feature(feature_name_test[i]);
		for(int j=0;j<feature_value.size();j++){
			//cout << feature_value.size() << endl;
			pcaSet_test.at<float>(i,j)=feature_value[j];
		}
	}
#endif

	//do PCA and LDA
	vector<Mat> src_pca_div, src_test_pca_div, rslt_pca_div,rslt_test_pca_div, rslt_lda_div, rslt_test_lda_div;
	divide(src_pca_div, pcaSet, 18*5*8);
		//normalize
	cout << "normalize PCA for each 18*5*8" << endl;
	for(int i=0;i<src_pca_div.size();i++){
		for(int j=0;j<src_pca_div[i].rows;j++){
			cv::normalize(src_pca_div[i].row(j), src_pca_div[i].row(j), 0, 1, NORM_MINMAX, CV_32F);
		}
	}
#ifdef _test
	divide(src_test_pca_div, pcaSet_test, 18*5*8);
	cout << "normalize test for each 18*5*8" << endl;
	for(int i=0;i<src_test_pca_div.size();i++){
		for(int j=0;j<src_test_pca_div[i].rows;j++){
			cv::normalize(src_test_pca_div[i].row(j), src_test_pca_div[i].row(j), 0, 1, NORM_MINMAX, CV_32F);
		}
	}
#endif

	cout << "PCA processing for each 18*5*8" << endl;
	int k=0;
	vector<PCA> pcas;
	for(vector<Mat>::iterator it=src_pca_div.begin(); it!=src_pca_div.end(); it++){
		cout << k++ << "-";
		pcas.push_back(PCA_proc(*it, (*it).rows-6));
	}

	cout << "PCA processing for training and testing data" << endl;
	for(int i=0;i<pcas.size();i++){
		rslt_pca_div.push_back(PCA_proj(src_pca_div[i], pcas[i]));
#ifdef _test
		rslt_test_pca_div.push_back(PCA_proj(src_test_pca_div[i], pcas[i]));
#endif
	}
	
	Mat temp_pca, temp_test_pca;
	cout << "concating every 18*5*8 training" << endl;
	col_concat(temp_pca, rslt_pca_div);
	//cout << temp_pca.size();
#ifdef _test
	cout << "concating every 18*5*8 testing" << endl;
	col_concat(temp_test_pca, rslt_test_pca_div);
	//cout << temp_test_pca.size();
#endif
	
	//write PCA to file
	cout << "writing "<< folder_name <<".pca to file" << endl;
	fstream PCA_proj;
	PCA_proj.open(folder_name+".pca",ios::out);
	for(int i=0;i<temp_pca.rows;i++){
		float *data_pca=temp_pca.ptr<float>(i);
		for(int j=0;j<temp_pca.cols;j++){
			PCA_proj << data_pca[j] << " ";
		}
		PCA_proj << endl;
	} 
#ifdef _test
	//write trainning data PCA to file
	cout << "writing "<< folder_name_test <<".pca to file" << endl;
	fstream PCA_proj_test;
	PCA_proj_test.open(folder_name_test+".pca",ios::out);
	for(int i=0;i<temp_test_pca.rows;i++){
		float *data_test_pca=temp_test_pca.ptr<float>(i);
		for(int j=0;j<temp_test_pca.cols;j++){
			PCA_proj_test << data_test_pca[j] << " ";
		}
		PCA_proj_test << endl;
	} 
#endif

	//LDA
	cout << "LDA processing for each feature vector" << endl;
	k=0;
	vector<LDA> ldas;
	for(vector<Mat>::iterator it=rslt_pca_div.begin(); it!=rslt_pca_div.end(); it++){
		cout << k++ << "-LDA processing" << endl;
		ldas.push_back(LDA_proc(folder_name, *it));
		//rslt_lda_div.push_back(rslt);
	}

	cout << "PCA processing for training and testing data" << endl;
	for(int i=0;i<ldas.size();i++){
		rslt_lda_div.push_back(LDA_proj(rslt_pca_div[i], ldas[i]));
#ifdef _test
		rslt_test_lda_div.push_back(LDA_proj(rslt_test_pca_div[i], ldas[i]));
#endif
	}
	
		//normalize train
	cout << "normalize train LDA for each 18*5*8" << endl;
	for(int i=0;i<rslt_lda_div.size();i++){
		for(int j=0;j<rslt_lda_div[i].rows;j++){
			cv::normalize(rslt_lda_div[i].row(j), rslt_lda_div[i].row(j), 0.0, 1.0, NORM_MINMAX, CV_64F);
		}
	}
#ifdef _test
		//normalize test
	cout << "normalize test LDA for each 18*5*8" << endl;
	for(int i=0;i<rslt_test_lda_div.size();i++){
		for(int j=0;j<rslt_test_lda_div[i].rows;j++){
			cv::normalize(rslt_test_lda_div[i].row(j), rslt_test_lda_div[i].row(j), 0.0, 1.0, NORM_MINMAX, CV_64F);
		}
	}
#endif

	Mat temp_lda, temp_lda_test;
	col_concat(temp_lda, rslt_lda_div);
#ifdef _test
	col_concat(temp_lda_test, rslt_test_lda_div);
#endif
	fstream OUT_file;
	OUT_file.open(folder_name+".lda",ios::out);
	for(int i=0;i<temp_lda.rows;i++){
		double *data_lda=temp_lda.ptr<double>(i);
		for(int j=0;j<temp_lda.cols;j++){
			OUT_file << data_lda[j] << " ";
		}
		OUT_file << endl;
	} 
	OUT_file.close();

#ifdef _test
	OUT_file.open(folder_name_test+".lda",ios::out);
	for(int i=0;i<temp_lda_test.rows;i++){
		double *data_lda=temp_lda_test.ptr<double>(i);
		for(int j=0;j<temp_lda_test.cols;j++){
			OUT_file << data_lda[j] << " ";
		}
		OUT_file << endl;
	} 
	OUT_file.close();
#endif

	vec_num=temp_lda.cols;
	//LDA end

	//KNN
		Mat knnResult;
		Mat knnTrain;
		temp_lda.convertTo(knnTrain,CV_32FC1,1.0,0);
		Mat calsses=get_cat(folder_name);
		Mat knnTest;
		temp_lda_test.convertTo(knnTest,CV_32FC1,1.0,0);
		knnResult=KNN_proc(knnTrain,calsses,knnTest);
	//KNN end

	cout << "Translating trainning data to svm format" << endl;
	DATA_to_SVM(folder_name, vec_num);
#ifdef _test
	cout << "Translating testing data to svm format" << endl;
	DATA_to_SVM(folder_name_test, vec_num);
#endif
}