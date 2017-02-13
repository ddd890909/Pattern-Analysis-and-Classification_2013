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

void feature_to_string(vector<float> feature,string name2, string file_name){
	if(feature.size() > 0){
		//get real name
		unsigned found = name2.find_last_of("/");
		size_t match;
		string name = name2.substr(found+1);
		string pos = name2.erase(found+1, name.size());
		found = pos.find_last_of("/", found-1);
		pos = pos.erase(found,pos.size()); //folder_name/
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
			//write cat and calculate the number of feature
			fstream cat;
			fstream catName;
			cat.open(file_name+".cat",ios::app);
			catName.open(file_name+".name",ios::app);
			if(!cat)
				cout << "Fail to open" << file_name << endl;
			else{
				cat << category << endl;
				catName<<category<<" "<<name<<endl;
			}
			cat.close();
			catName.close();
			cat.open(file_name+".cat",ios::in);
			int tmp, cnt=0;
			while(cat >> tmp){
				cnt++;
			}
			cat.close();

			string mat_name = file_name+"_"+int2str(cnt);
			feature_to_mat(feature, pos+"/feature", mat_name);
			/*fstream feature_data;
			feature_data.open(file_name+".feature", ios::app);
			if(!feature_data)
				cout << "Fail to open" << file_name << endl;
			else{
				for(int i=0; i<feature.size();i++){
					feature_data << feature[i] << " ";
				}
				feature_data << endl;
				feature_data.close();
			}*/
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
		if(cut(image, imgPatch, alignmentResult, 28, 28)){
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


void DATA_to_SVM(string name, int vec_num){
	fstream origin, cat, result;
	origin.open(name+".pca",ios::in);
	cat.open(name+".cat",ios::in);
	result.open(name+".svm",ios::out);
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


vector<float> mat_to_feature(string name){
	vector<float> feature;
	Mat temp = imread(name,-1);
	float *p_tmp;
	for(int i=0; i<temp.rows; i++) {
		for(int j=0; j<temp.cols; j++) {
			p_tmp = (float*)temp.ptr(i);
			feature.push_back(p_tmp[j]);
		}
	}
	return feature;
}

void feature_to_mat(vector<float> feature, string pos, string mat_name){
	mkdir(pos.c_str());
	Mat fout_depth(1,feature.size(),CV_8UC4);
	memcpy(fout_depth.data,&feature[0],feature.size()*sizeof(float));
	imwrite(pos+"/"+mat_name+".png",fout_depth);
}

Mat get_cat(string folder_name){
	fstream fp;
	fp.open(folder_name+".cat",ios::in);
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