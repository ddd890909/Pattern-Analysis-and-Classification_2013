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
string folder_name="JAFFE";
string train_folder="TRAIN";


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
	stage_1(folder_name,folder_name+"/cropped");
	stage_2(folder_name+"/cropped", folder_name+"/"+folder_name);


	//read data to mat before PCA

		//for training data
	cout << "Reading " << folder_name << ".feature" << endl;
	fstream fp;
	vector<int> category;
	fp.open(train_folder+"/"+train_folder+".cat", ios::in);
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
	//		normalize_custom(src_pca_div[i].row(j), src_pca_div[i].row(j));
	//	}
	//}

	string dest_folder = train_folder+"/LocalPCA_egnvec/";
  	mkdir(dest_folder.c_str()); //create a folder
	cout << "PCA processing for each 18*5*8" << endl;
	int k=0;
	vector<PCA> pcas;
	for(int i = 0; i < src_pca_div.size(); i++){
		cout << k++ << "-";
		PCA temp = PCA_proc(src_pca_div[i], src_pca_div[i].rows-6);
		FileStorage fs(dest_folder+"LocalPCA_"+int2str(i)+".yml", FileStorage::WRITE);
		pca_save(fs, temp);
		pcas.push_back(temp);
		fs.release();
		//pcas.push_back(PCA_proc(*it, (*it).rows-6));
	}

	cout << "PCA processing for training data" << endl;
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

	//LDA
	dest_folder = train_folder+"/LocalLDA_egnvec/";
  	mkdir(dest_folder.c_str()); //create a folder
	cout << "LDA processing for each feature vector" << endl;
	k=0;
	vector<LDA_custom> ldas;
	for(int i=0;i<rslt_pca_div.size();i++){
		cout << k++ << "-LDA processing" << endl;
		LDA_custom temp;
		temp = LDA_proc(train_folder+"/"+train_folder, rslt_pca_div[i]);
		FileStorage fs(dest_folder+"LocalLDA_"+int2str(i)+".yml", FileStorage::WRITE);
		temp.save(fs);
		ldas.push_back(temp);
		fs.release();
		//rslt_lda_div.push_back(rslt);
	}

	cout << "LDA processing for training and testing data" << endl;
	for(int i=0;i<ldas.size();i++){
		rslt_lda_div.push_back(LDA_proj(rslt_pca_div[i], ldas[i]));
	}

	cout << "normalize for each 18*5*8" << endl;
	for(int i=0;i<rslt_lda_div.size();i++){
		for(int j=0;j<rslt_lda_div[i].rows;j++){
			cv::normalize(rslt_lda_div[i].row(j), rslt_lda_div[i].row(j), -1, 1, NORM_MINMAX, CV_64F);
			//normalize_custom(rslt_lda_div[i].row(j), rslt_lda_div[i].row(j));
		}
	}

	//	
	//cout << "normalize train LDA for each 18*5*8" << endl;
	//for(int i=0;i<rslt_lda_div.size();i++){
	//	for(int j=0;j<rslt_lda_div[i].rows;j++){
	//		cv::normalize(rslt_lda_div[i].row(j), rslt_lda_div[i].row(j), 0.0, 1.0, NORM_MINMAX, CV_64F);
	//		//normalize_custom(rslt_lda_div[i].row(j), rslt_lda_div[i].row(j));
	//	}
	//}	
	dest_folder = train_folder+"/LocalSVM/";
  	mkdir(dest_folder.c_str()); //create a folder
	for(int i=0;i<rslt_lda_div.size();i++){
		fstream OUT_file;
		OUT_file.open(dest_folder+"Local_"+int2str(i)+".lda",ios::out);
		for(int j=0;j<rslt_lda_div[i].rows;j++){
			double *data_lda=rslt_lda_div[i].ptr<double>(j);
			for(int k=0;k<rslt_lda_div[i].cols;k++){
				OUT_file << data_lda[k] << " ";
			}
			OUT_file << endl;
		} 
		OUT_file.close();
	}
	for(int i = 0; i < rslt_lda_div.size(); i++){
		vec_num=rslt_lda_div[i].cols;
		cout << "Translating trainning data to svm format" << endl;
		DATA_to_SVM(dest_folder+"Local_"+int2str(i), folder_name, vec_num);
	}	
}


