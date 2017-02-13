#include "work.h"
#include "gabor.h"

string int2str(int i){
	stringstream ss;
	ss << i;
	return ss.str();
}

string float2str(float i){
	stringstream ss;
	ss << i;
	return ss.str();
}


bool cat_matching(string name, string cat){
	std::size_t found = name.find(cat);
	if(found!=std::string::npos){
		return true;
	}
	else return false;
}

void feature_to_string(vector<float>& feature,string name2, string file_name){
	//get real name
	if(feature.size() > 0){
		unsigned found = name2.find_last_of("/");
		size_t match;
		string name = name2.substr(found+1);
		int category=-1;
		if(cat_matching(name,"AN")||cat_matching(name,"_0_")){
			category=1;
		}
		else if(cat_matching(name,"DI")||cat_matching(name,"_1_")){
			category=2;
		}
		else if(cat_matching(name,"FE")||cat_matching(name,"_2_")){
			category=3;
		}
		else if(cat_matching(name,"HA")||cat_matching(name,"_3_")){
			category=4;
		}
		else if(cat_matching(name,"SA")||cat_matching(name,"_4_")){
			category=5;
		}
		else if(cat_matching(name,"SU")||cat_matching(name,"_5_")){
			category=6;
		}

		cout << category << " " << name << " feature to string" << endl;

		//PCA version
		if(category!=-1){
			fstream cat, feature_data;
			cat.open("TEST/TEST.cat", ios::app);
			feature_data.open(file_name+".feature", ios::app);
			if(!cat || !feature_data)
				cout << "Fail to open" << file_name << endl;
			else{
				cat << category << endl;
				for(int i=0; i<feature.size();i++){
					feature_data << feature[i] << " ";
				}
				feature_data << endl;
				cat.close();
				feature_data.close();
			}
		}
	}
}


void feature_extract(string name, string file_name, vector<CvMat*> alignmentResult){
	vector<Mat> imgPatch;
	vector<float> feature;
	Mat image = imread(name.c_str(),0);
	if(!image.empty()){
		cout << name << " processing" << endl;
		//patch(image, imgPatch, 7, 7);
			destroyAllWindows();
		if(cut(image, imgPatch, alignmentResult, 22, 22)){
			gabor(feature, imgPatch);
			destroyAllWindows();
		}
		feature_to_string(feature, name, file_name);
	}
}

void feature_extract_old(string name, string file_name){
	vector<Mat> imgPatch;
	vector<float> feature;
	Mat image = imread(name.c_str(),0);
	if(!image.empty()){
		cout << name << " processing" << endl;
		patch(image, imgPatch, 7, 7);
		destroyAllWindows();
		gabor(feature, imgPatch);
		destroyAllWindows();
		feature_to_string(feature, name, file_name);
	}
}


vector<string> get_file_names(string name){
	char filename[150];
	vector<string> names;
	string folder = name+"/*.*";
	string folderwithout = name+"/";

	size_t origsize = folder.size() + 1;
    const size_t newsize = 100;
    size_t convertedChars = 0;
    wchar_t wcstring[newsize];
    mbstowcs_s(&convertedChars, wcstring, origsize, folder.c_str(), _TRUNCATE);

    WIN32_FIND_DATA FindFileData;
    HANDLE FileHandle;
	FileHandle = FindFirstFile(wcstring, &FindFileData);
    WideCharToMultiByte( CP_ACP, 0, FindFileData.cFileName, -1,filename, sizeof(filename), NULL, NULL ); 
	SetLastError(0);
	int count =0;
	while(GetLastError()!=ERROR_NO_MORE_FILES){	
		if(count++>1){
			string dir = folderwithout+filename; 
			names.push_back(dir);
		}
		FindNextFile(FileHandle,&FindFileData);
		WideCharToMultiByte( CP_ACP, 0, FindFileData.cFileName, -1,filename, sizeof(filename), NULL, NULL );
	}
	return names;

}

vector<string> get_real_file_names(string name){
	char filename[150];
	vector<string> names;
	string folder = name+"/*.*";
	string folderwithout = name+"/";

	size_t origsize = folder.size() + 1;
    const size_t newsize = 100;
    size_t convertedChars = 0;
    wchar_t wcstring[newsize];
    mbstowcs_s(&convertedChars, wcstring, origsize, folder.c_str(), _TRUNCATE);

    WIN32_FIND_DATA FindFileData;
    HANDLE FileHandle;
	FileHandle = FindFirstFile(wcstring, &FindFileData);
    WideCharToMultiByte( CP_ACP, 0, FindFileData.cFileName, -1,filename, sizeof(filename), NULL, NULL ); 
	SetLastError(0);
	int count = 0;
	while(GetLastError()!=ERROR_NO_MORE_FILES){	
		if(count++>1)
			names.push_back(filename);
		FindNextFile(FileHandle,&FindFileData);
		WideCharToMultiByte( CP_ACP, 0, FindFileData.cFileName, -1,filename, sizeof(filename), NULL, NULL );
	}
	return names;

}


PCA PCA_proc(Mat pcaSet, int down=0){
	cout << "PCA processing" << endl;
	PCA pca(pcaSet, Mat(), CV_PCA_DATA_AS_ROW, down);
	return pca;
}

Mat PCA_proj(Mat pcaSet, PCA pca){
	Mat projed = pca.project(pcaSet);	
	return projed;
}


void DATA_to_SVM(string dest, string name, int vec_num){
	fstream origin, cat, result;
	origin.open(dest+".lda",ios::in);
	cat.open("TEST/TEST.cat",ios::in);
	result.open(dest+".svm",ios::out);
	vector<int> category;
	int tmp;
	while(cat>>tmp){
		category.push_back(tmp);
	}
	for(int i=0;i<category.size();i++){
		//cout << category[i] << endl;
		string text,value;
		int count=0;
		getline(origin,text);
		stringstream ss(text);
		result << category[i] << " ";
		for(int j=0;j<vec_num;j++){
			ss >> value;
			result << ++count << ":" << value << " ";
		}
		result << endl;
	}
}

Mat get_cat(string folder_name){
	fstream fp;
	//fp.open("TRAIN/TRAIN.cat",ios::in);
	fp.open(folder_name,ios::in);
	vector<float> cat;
	float tmp;
	while(fp>>tmp){
		cat.push_back(tmp);
	}
	Mat result(cat.size(),1,CV_32FC1);
	for(int i=0;i<cat.size();i++){
		result.at<float>(i,0)=cat[i];
	}
	return result;
}

void seperate_class(string svm_name, string cat_name, string knn_name,int class_num)
{
	fstream file, cat, out_file, out_cat;
	file.open(svm_name, ios::in);
	cat.open(cat_name, ios::in);
	out_file.open(knn_name+int2str(class_num), ios::out);
	out_cat.open(knn_name+int2str(class_num)+".cat", ios::out);
	int cat_num;
	while(cat >> cat_num){
		//cout << cat_num << endl;
		//system("PAUSE");
		string kerker;
		getline(file, kerker);
		if(cat_num == class_num){
			out_cat << cat_num << endl;
			out_file << kerker << endl; 
		}
	}
	file.close();
	cat.close();
	out_file.close();
	out_cat.close();
}

void local_lda_to_mat(string knn_name,Mat &knnTrain,Mat &classes,int i,int j )
{
	fstream lda, cat;
	lda.open(knn_name+int2str(i)+".lda"+int2str(j), ios::in);
	classes = get_cat(knn_name+int2str(i)+".lda"+int2str(j)+".cat");
	Mat result(classes.rows, 5, CV_32F);
	for(int i=0;i<classes.rows;i++)
	{
		string kerker;
		getline(lda, kerker);
		stringstream ss(kerker);
		for(int j=0;j<5;j++)
		{
			ss >> result.at<float>(i,j);
		}
	}
	lda.close();
	knnTrain = result.clone();
}

void lda_to_mat(string svm_name, Mat &knnTrain, int i){
	fstream pt;
	pt.open(svm_name+int2str(i)+".lda", ios::in);
	double tmp;
	int cnt=0;
	while(pt >> tmp){
		cnt++;
	}
	cnt/=5;
	pt.close();
	pt.open(svm_name+int2str(i)+".lda", ios::in);
	Mat result(cnt, 5, CV_32F, 1);
	for(int i=0;i<cnt;i++){
		string kerker;
		getline(pt, kerker);
		stringstream ss(kerker);
		for(int j=0; j<5;j++){
			ss >> result.at<float>(i,j);
		}
	}
	knnTrain = result.clone();
	pt.close();
}