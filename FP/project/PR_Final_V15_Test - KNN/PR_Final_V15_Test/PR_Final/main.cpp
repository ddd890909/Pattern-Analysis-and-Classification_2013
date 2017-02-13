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
string folder_name="JAFFE_one";
string train_folder="TRAIN";
string test_folder="TEST";


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
		//for test data
	stage_1(folder_name,folder_name+"/cropped");
	stage_2(folder_name+"/cropped", folder_name+"/"+folder_name);


	//read data to mat before PCA

		//for testing data
	cout << "Reading " << folder_name << ".feature" << endl;
	fstream fp;
	vector<int> category;
	fp.open(test_folder+"/"+test_folder+".cat", ios::in);
	int tmp;
	while(fp >> tmp){
		category.push_back(tmp);
	}
	fp.close();
	int sample_num=category.size();

	cout << folder_name << " to mat" <<endl;
	Mat pcaSet(sample_num, vec_num, CV_32F,1);
	fp.open(folder_name+"/"+folder_name+".feature", ios::in);
	float *data_div = pcaSet.ptr<float>(0);
	for(int i=0;i<sample_num;i++){
		string kerker;
		getline(fp, kerker);
		stringstream ss(kerker);
		for(int j=0;j<vec_num;j++){
			ss >> data_div[j+i*vec_num];
		}
		cout << "data:" << i << "...";
	}
	fp.close();
	
//pca start

	//do PCA and LDA
	vector<Mat> src_pca_div, rslt_pca_div, rslt_lda_div;
	divide(src_pca_div, pcaSet, 18*5*8);
	//normalize
	//cout << "normalize for each 18*5*8" << endl;
	//for(int i=0;i<src_pca_div.size();i++){
	//	for(int j=0;j<src_pca_div[i].rows;j++){
	//		cv::normalize(src_pca_div[i].row(j), src_pca_div[i].row(j), 0, 1, NORM_MINMAX, CV_32F);
	//	}
	//}

	string src_folder = train_folder+"/LocalPCA_egnvec/";
	string dst_folder = test_folder+"/LocalPCA_egnvec/";
	cout << "PCA processing for each 18*5*8" << endl;
	int k=0;
	vector<PCA> pcas;
	for(int i = 0; i < src_pca_div.size(); i++){
		cout << k++ << "-PCA processing" << endl;
		FileStorage fs(src_folder+"LocalPCA_"+int2str(i)+".yml", FileStorage::READ);
		PCA temp;
		temp = pca_load(fs);
		pcas.push_back(temp);
		fs.release();
		//pcas.push_back(PCA_proc(*it, (*it).rows-6));
	}

	cout << "PCA processing for testing data" << endl;
	for(int i=0;i<pcas.size();i++){
		rslt_pca_div.push_back(PCA_proj(src_pca_div[i], pcas[i]));
	}
	
	//Mat temp_pca;
	//cout << "concating every 18*5*8 training" << endl;
	//concat(temp_pca, rslt_pca_div);
	//cout << temp_pca.size();
		
	//write PCA to file
	//cout << "writing "<< folder_name <<".pca to file" << endl;
	//fstream PCA_proj;
	//PCA_proj.open(folder_name+".pca",ios::out);
	//for(int i=0;i<temp_pca.rows;i++){
	//	float *data_pca=temp_pca.ptr<float>(i);
	//	for(int j=0;j<temp_pca.cols;j++){
	//		PCA_proj << data_pca[j] << " ";
	//	}
	//	PCA_proj << endl;
	//} 
	
//pca end
	
		//normalize train
	//cout << "normalize for each 18*5*8" << endl;
	//for(int i=0;i<rslt_pca_div.size();i++){
	//	for(int j=0;j<rslt_pca_div[i].rows;j++){
	//		cv::normalize(rslt_pca_div[i].row(j), rslt_pca_div[i].row(j), 0, 1, NORM_MINMAX, CV_32F);
	//		//normalize_custom(rslt_lda_div[i].row(j), rslt_lda_div[i].row(j));
	//	}
	//}

	//LDA
	src_folder = train_folder+"/LocalLDA_egnvec/";
	dst_folder = test_folder+"/LocalLDA_egnvec/";
	cout << "LDA processing for each feature vector" << endl;
	k=0;
	vector<LDA_custom> ldas;
	for(int i=0;i<rslt_pca_div.size();i++){
		cout << k++ << "-LDA processing" << endl;
		FileStorage fs(src_folder+"LocalLDA_"+int2str(i)+".yml", FileStorage::READ);
		LDA_custom temp;
		temp.load(fs);
		ldas.push_back(temp);
		fs.release();
		//rslt_lda_div.push_back(rslt);
	}

	cout << "LDA processing for testing data" << endl;
	for(int i=0;i<ldas.size();i++){
		rslt_lda_div.push_back(LDA_proj(rslt_pca_div[i], ldas[i]));
	}
		
	cout << "normalize train LDA for each 18*5*8" << endl;
	for(int i=0;i<rslt_lda_div.size();i++){
		for(int j=0;j<rslt_lda_div[i].rows;j++){
			//cv::normalize(rslt_lda_div[i].row(j), rslt_lda_div[i].row(j), -1, 1, NORM_MINMAX, CV_64F);
			normalize_custom(rslt_lda_div[i].row(j), rslt_lda_div[i].row(j));
		}
	}	

	dst_folder = test_folder+"/LocalSVM/";
  	mkdir(dst_folder.c_str()); //create a folder
	for(int i=0;i<rslt_lda_div.size();i++){
		fstream OUT_file;
		OUT_file.open(dst_folder+"Local_"+int2str(i)+".lda",ios::out);
		for(int j=0;j<rslt_lda_div[i].rows;j++){
			double *data_lda=rslt_lda_div[i].ptr<double>(j);
			for(int k=0;k<rslt_lda_div[i].cols;k++){
				OUT_file << data_lda[k] << " ";
			}
			OUT_file << endl;
		} 
		OUT_file.close();
	}
	

		//knn test start
	cout << "test local KNN for 66 5-dimensional vector " << endl;

	int local_class_num = 66;
	string test_folder="TEST";

	string dest_folder=test_folder+"/LocalKNN/";
	mkdir(dest_folder.c_str()); //create a folder
	string svm_name = "TEST/LocalSVM/Local_";
	string knn_name = "TRAIN/LocalKNN/Local_";

	//for(int i=0;i<local_class_num;i++)
	//{
	//	for(int j=0;j<6;j++)
	//	{
	//		seperate_class(svm_name+int2str(i)+".lda", cat_name, knn_name+int2str(i)+".lda", j+1);
	//	}
	//}

	//超牛逼


	Vector<Mat> knnTrain;
	Vector<Mat> knnClass;
	Mat knnTest;
	
	Mat knnT;
	Mat knnC;
	Mat knnD;
	Vector<Mat> knnDists;
	Mat knnP;
	Mat knn_sum;
	Mat knnSubResult;
	Mat knnResult;

	for(int i=0;i<local_class_num;i++) //66次
	{
		lda_to_mat(svm_name, knnTest, i);
		
		for(int j=0;j<6;j++)//train的6個class
		{
			local_lda_to_mat(knn_name, knnT, knnC, i, j+1 );
			knnD=KNN_dist(knnT,knnC,knnTest);
			//cout<<knnD<<endl;
			if(j==0)
			{
				//knnDists=knnD;
				knnDists.push_back(knnD);
				knn_sum=1.0/(1.0+knnD);
				//cout<<knn_sum<<endl;
			}
			else
			{
				//hconcat( knnDists, knnD, knnDists);
				knnDists.push_back(knnD);
				knn_sum=knn_sum+1.0/(1.0+knnD);
			}
		}

		for(int j=0;j<6;j++)
		{
			if(j==0)
				knnP=( 1.0/(1.0+knnDists[0]) )/ knn_sum;
			else
				hconcat( knnP, ( 1.0/(1.0+knnDists[j]) )/ knn_sum, knnP);	
			//cout<<knnP<<endl;
		}
		knnDists.clear();

		if(i==0)
			knnResult=knnP;
		else
			hconcat(knnResult,knnP,knnResult);

	}
	//cout<<knnResult.cols <<endl;

		//write local KNN result to file
	cout << "writing local knnResult.knn to file" << endl;
	fstream knn_local_result;
	knn_local_result.open(dest_folder+"knnResult.knn",ios::out);
	for(int i=0;i<knnResult.rows;i++)
	{
		float *data_knn=knnResult.ptr<float>(i);
		for(int j=0;j<knnResult.cols;j++)
		{
			knn_local_result << data_knn[j] << " ";
		}
		knn_local_result << endl;
	} 
	knn_local_result.close();

	//超牛逼

}


