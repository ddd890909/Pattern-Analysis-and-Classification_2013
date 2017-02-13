#include "work.h"

PCA PCA_proc(Mat pcaSet, int down=0){
	cout << "PCA processing" << endl;
	PCA pca(pcaSet, Mat(), CV_PCA_DATA_AS_ROW, down);
	return pca;
}

Mat PCA_proj(Mat pcaSet, PCA pca){
	Mat projed = pca.project(pcaSet);	
	return projed;
}

string int2str(int i){
	stringstream ss;
	ss << i;
	return ss.str();
}

Mat svm_to_Mat(string name){
	fstream data,cat;
	data.open(name, ios::in);
	cat.open("TEST/TEST.cat", ios::in);
	
	int tmp, cnt=0;
	while(cat >> tmp){
		cnt++;
	}
	Mat svm_mat(cnt, 6, CV_32F, 1);
	string *ker = new string;
	getline(data, *ker);
	delete ker;
	for(int i=0;i<cnt;i++){
		string kerker;
		getline(data, kerker);
		stringstream ss(kerker);
		ss >> tmp;
		for(int j=0;j<6;j++){
			ss >> svm_mat.at<float>(i,j);
		}
	}
	return svm_mat;
}

void Mat_to_svm(Mat src ,string name){
	fstream cat, output;
	cat.open("TEST/TEST.cat", ios::in);
	output.open(name+".svm", ios::out);
	for(int i=0;i<src.rows;i++){
		int cat_i; 
		cat >> cat_i;
		output << cat_i << " ";
		for(int j=0;j<src.cols;j++){
			output << j+1 << ":" << src.at<double>(i,j) << " ";
		}
		output << endl;
	}
}

PCA pca_load(const FileStorage& fs){
	PCA pca;
	fs["mean"] >> pca.mean;
	fs["Eigenvalues"] >> pca.eigenvalues;
	fs["Eigenvector"] >> pca.eigenvectors;
	return pca;
}
void pca_save(FileStorage& fs, PCA pca){
	fs<< "mean" << pca.mean;
	fs << "Eigenvalues" << pca.eigenvalues;
	fs << "Eigenvector" << pca.eigenvectors;
}

void normalize_custom(Mat &output, Mat input){
	Mat mean, stddev;
	meanStdDev(input, mean, stddev);
	float mean_val = mean.data[0];
	float stddev_val = stddev.data[0];

	float *data = output.ptr<float>(0);
	for(int i=0; i<output.cols; i++){
		data[i]-=mean_val;
		data[i]/=stddev_val;
	}
}