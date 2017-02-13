#include "work.h"
#include "gabor.h"

string int2str(int i){
	string s;
	stringstream ss(s);
	ss << i;
	return ss.str();
}

string float2str(float i){
	string s;
	stringstream ss(s);
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
			cat.open(file_name+".cat", ios::app);
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

		/*
		//no PCA version
		if(category!=-1){
			fstream fp;
			fp.open(file_name, ios::app);
			if(!fp)
				cout<<"Fail to open file " <<endl;
			//write data
			else{
				fp << category << " ";
				for(int i=0;i<feature.size();i++){
					fp<< i+1 << ":" << feature[i] << " ";
				}
				fp << endl;
				fp.close();
			}
		}*/
		//return result;

		//cout << category << endl;
		//return result;
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
void stage_2_old(string folder, string file_name){
	vector<string> names;
	names = get_file_names(folder);
	for(int i=0;i<names.size();i++){
		Mat image = imread(names[i]);
		if(!image.empty()){
			feature_extract_old(names[i], file_name);
		}
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

void stage_2(string folder, string file_name){
	vector<FP> FaceFP;              //Alignment後存放
	vector<FaceInfo> CropFaceSet;   //包含Crop後要偵測的資訊
	vector<CvRect> faceWindow; //store each face's rect position
	vector<CvMat*> alignmentResult; // stroe facial points
	vector<string> names;
	names = get_file_names(folder);
	extern FaceDetection *face_detector;
	extern OpenCVASMAlignment *myAlignment;
    
	IplImage *img;
	IplImage *drawimg;

	for(int j=0;j<names.size();j++){
		if(  (img = cvLoadImage(names[j].c_str())) ==0 )// Load img
			printf("%s cannot open!\n", names[j].c_str());
		else{
			drawimg = cvCloneImage(img);
 			faceWindow = face_detector->Detect(img);//人臉偵測
			//cvNamedWindow("ivFace");
			if(faceWindow.size() != 0){ //若有偵測到一個以上的人臉
				for(int i=0; i<faceWindow.size(); i++)
					cvRectangle(drawimg,cvPoint(faceWindow[i].x, faceWindow[i].y),cvPoint(faceWindow[i].x+faceWindow[i].width, faceWindow[i].y+faceWindow[i].height),CV_RGB(255,0,0));
				myAlignment->SetImage(img);
				for(int i=0; i<faceWindow.size(); i++)	
					alignmentResult.push_back(myAlignment->calcAlignment(faceWindow[i]));
				for(int i=0; i<alignmentResult.size(); i++){
					for (int k = 0; k < alignmentResult[i]->rows; k++)
						if(k == 31 ||k == 36 || k == 66 || k == 67){
							cvCircle(drawimg, cvPoint(cvmGet(alignmentResult[i], k, 0), cvmGet(alignmentResult[i], k, 1)), 1, CV_RGB(255, 0, 0), 2);
							//cout << k << " red (" << cvmGet(alignmentResult[i], k, 0) << "," << cvmGet(alignmentResult[i], k, 1) << ")" << endl;
							//cvShowImage("ivFace",drawimg);
							//cvWaitKey(0);
						}
						else{
							cvCircle(drawimg, cvPoint(cvmGet(alignmentResult[i], k, 0), cvmGet(alignmentResult[i], k, 1)), 1, CV_RGB(0, 255, 0), 2);
							//cout <<  k << " grn (" << cvmGet(alignmentResult[i], k, 0) << "," << cvmGet(alignmentResult[i], k, 1) << ")" << endl;
							//cvShowImage("ivFace",drawimg);
							//cvWaitKey(0);
						}
					FP AlignOK;
					AlignOK.righteye.x = cvmGet(alignmentResult[i], 36, 0);
					AlignOK.righteye.y = cvmGet(alignmentResult[i], 36, 1);
					AlignOK.lefteye.x = cvmGet(alignmentResult[i], 31, 0);
					AlignOK.lefteye.y = cvmGet(alignmentResult[i], 31, 1);;
					AlignOK.mouth.x = cvmGet(alignmentResult[i], 66, 0);
					AlignOK.mouth.y = cvmGet(alignmentResult[i], 66, 1);
					AlignOK.nose.x = cvmGet(alignmentResult[i], 67, 0);
					AlignOK.nose.y = cvmGet(alignmentResult[i], 67, 1);
					FaceFP.push_back(AlignOK);
				}
			}
			//else{
			//	cout << "Cannot find ASM landmark points of " << names[j] << "in stage 2. " << endl;
			//	system("pause");
			//}
			//cvShowImage("ivFace",drawimg);
			//cvWaitKey(0);
			Mat image = imread(names[j]);
			if(!image.empty()){
				feature_extract(names[j], file_name, alignmentResult);
			}
			alignmentResult.clear();	
			//faceWindow.clear();
			//FaceFP.clear();	
			cvReleaseImage(&img);
			cvReleaseImage(&drawimg);	
		}
	}

}


void patch(Mat img, vector<Mat> &result, int m, int n){
	int w = img.cols / ((m+1)/2);
	int h = img.rows / ((n+1)/2);
	int x=0, y=0;
	for(int j=0; j<n; j++){
		for(int i=0; i<m; i++){
			Rect roi(x, y, w, h);
			Mat temp = img(roi);
			result.push_back(temp);
			x += (w/2);
		}
		x=0;
		y += (h/2);
	}
}

bool cut(Mat img, vector<Mat> &result, vector<CvMat*> alignmentResult, int w, int h){
	int index[37] = {15,16,17,18,21,22,23,24,27,28,
						29,30,32,33,34,35,37,38,39,40,
						41,42,43,44,45,48,49,50,51,52,
						53,54,55,56,57,58,59};
	int index_no_nose[28] = {15,16,17,18,21,22,23,24,27,28,
								29,30,32,33,34,35,48,49,50,51,
								52,53,54,55,56,57,58,59};
	int index_few_lips[22] = {15,16,17,18,21,22,23,24,27,28,
								29,30,32,33,34,35,48,50,52,54,
								56,58};
	for(int i=0; i<alignmentResult.size(); i++){
		for (int k = 0; k < 22; k++){
			int x=cvmGet(alignmentResult[i], index_few_lips[k], 0)-w/2, y=cvmGet(alignmentResult[i], index_few_lips[k], 1)-h/2;
			if(x < 0 || y < 0 || x+w/2 > img.cols-1 || y+h/2 > img.rows-1) return false; 
			Rect roi(x, y, w, h);
			Mat temp = img(roi);
			result.push_back(temp);
		}
	}
	return true;
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

void stage_1(string source_folder, string dest_folder){
	vector<FP> FaceFP;              //Alignment後存放
	vector<FaceInfo> CropFaceSet;   //包含Crop後要偵測的資訊
	vector<IplImage*> FaceSet;      //存放Crop後灰階影像
	vector<CvRect> faceWindow; //store each face's rect position
	vector<CvMat*> alignmentResult; // stroe facial points
	extern FaceDetection *face_detector;
	extern OpenCVASMAlignment *myAlignment;

	IplImage *img;
	IplImage *drawimg;

	vector<string> file_names, real_file_names;
  	mkdir(dest_folder.c_str()); //create a folder

	file_names = get_file_names(source_folder);
	real_file_names = get_real_file_names(source_folder);

	for(int j=0;j<file_names.size();j++){
		if(  (img = cvLoadImage(file_names[j].c_str())) ==0 )// Load img
			printf("%s cannot open!\n", file_names[j].c_str());
		else{
			drawimg = cvCloneImage(img);
 			faceWindow = face_detector->Detect(img);//人臉偵測
			if(faceWindow.size() != 0){ //若有偵測到一個以上的人臉
				for(int i=0; i<faceWindow.size(); i++)
					cvRectangle(drawimg,cvPoint(faceWindow[i].x, faceWindow[i].y),cvPoint(faceWindow[i].x+faceWindow[i].width, faceWindow[i].y+faceWindow[i].height),CV_RGB(255,0,0));
				myAlignment->SetImage(img);
				for(int i=0; i<faceWindow.size(); i++)	
					alignmentResult.push_back(myAlignment->calcAlignment(faceWindow[i]));
				for(int i=0; i<alignmentResult.size(); i++){
					for (int k = 0; k < alignmentResult[i]->rows; k++)
						if(k == 31 ||k == 36 || k == 66 || k == 67)
							cvCircle(drawimg, cvPoint(cvmGet(alignmentResult[i], k, 0), cvmGet(alignmentResult[i], k, 1)), 1, CV_RGB(255, 0, 0), 2);
						else
							cvCircle(drawimg, cvPoint(cvmGet(alignmentResult[i], k, 0), cvmGet(alignmentResult[i], k, 1)), 1, CV_RGB(0, 255, 0), 2);
					FP AlignOK;
					AlignOK.righteye.x = cvmGet(alignmentResult[i], 36, 0);
					AlignOK.righteye.y = cvmGet(alignmentResult[i], 36, 1);
					AlignOK.lefteye.x = cvmGet(alignmentResult[i], 31, 0);
					AlignOK.lefteye.y = cvmGet(alignmentResult[i], 31, 1);;
					AlignOK.mouth.x = cvmGet(alignmentResult[i], 66, 0);
					AlignOK.mouth.y = cvmGet(alignmentResult[i], 66, 1);
					AlignOK.nose.x = cvmGet(alignmentResult[i], 67, 0);
					AlignOK.nose.y = cvmGet(alignmentResult[i], 67, 1);
					FaceFP.push_back(AlignOK);
				}
			}
			FaceCropping(img, CropFaceSet,FaceSet, FaceFP);//人臉裁切
			for(int i=0; i<CropFaceSet.size()  ;i++){
				cvLine(drawimg,CropFaceSet[i].leftdown,CropFaceSet[i].leftTop,CV_RGB(255,255,0),2);
				cvLine(drawimg,CropFaceSet[i].leftTop,CropFaceSet[i].rightTop,CV_RGB(255,255,0),2);
				cvLine(drawimg,CropFaceSet[i].rightTop,CropFaceSet[i].rightdown,CV_RGB(255,255,0),2);
				cvLine(drawimg,CropFaceSet[i].rightdown,CropFaceSet[i].leftdown,CV_RGB(255,255,0),2);
				//cvShowImage("test", FaceSet[i]);

				// 將原附檔名刪除
				for(int k=real_file_names[j].size()-1; k>=0;k--){
					if(real_file_names[j][k]=='.'){
						real_file_names[j].pop_back();
						break;
					}
					else{
						real_file_names[j].pop_back();
					}
				}
				string resultdir;
				resultdir = dest_folder +"/"+ real_file_names[j] + ".tiff";
				cvSaveImage( resultdir.c_str(), FaceSet[i]);
			}
			//cvNamedWindow("ivFace");
			//cvShowImage("ivFace",drawimg);
			//cvWaitKey(0);
			alignmentResult.clear();	
			faceWindow.clear();
			FaceFP.clear();	
			cvReleaseImage(&img);
			cvReleaseImage(&drawimg);	
		}
	}
}

PCA PCA_proc(Mat pcaSet, int down=0){
	cout << "PCA processing" << endl;
	cout << "dim" << pcaSet.size() << endl;
	system("PAUSE");
	PCA pca(pcaSet, Mat(), CV_PCA_DATA_AS_ROW, down);
	return pca;
}

Mat PCA_proj(Mat pcaSet, PCA pca){
	Mat projed = pca.project(pcaSet);	
	return projed;
}

Mat LDA_proc(string name, Mat src, int sample_num, int down){
	int temp;
	cout << "Reading " << name << ".cat" << endl;
	fstream fp, LDA_cat_file;
	vector<int> ldacat;
	fp.open(name+".cat", ios::in);
	for(int i=0;i<sample_num;i++){
			fp >> temp;
			ldacat.push_back(temp);
	}
	fp.close();

	cout << "LDA processing" << endl;
	LDA lda(src, ldacat, down);
	
	Mat projed = lda.project(src);
	cout << "LDA done" << endl;
	return projed;

}

void DATA_to_SVM(string name, int vec_num){
	fstream origin, cat, result;
	cat.open(name+".cat",ios::in);
	origin.open(name+".pca",ios::in);
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

void divide(vector<Mat> &result, Mat src, int cutcol){
	Mat temp;
	for(int i=0; i<src.cols/cutcol; i++){
		Rect roi(i*cutcol, 0, cutcol, src.rows);
		temp = src(roi);
		result.push_back(temp);
	}
}

void concat(Mat &result, vector<Mat> src){
	hconcat(src, result);
}


/*
//////////////////

int dimensionality=0;
int dataNumber=0;
int dimensionalityDowm=5;

void PrintMatrix(CvMat *Matrix,int Rows,int Cols)
{
	for(int i=0;i<Rows;i++)
	{
		for(int j=0;j<Cols;j++)
		{
			printf("%.2f ",cvGet2D(Matrix,i,j).val[0]);
		}
		printf("\n");
	}
}

fstream feature_pca(string name, int dimensionality, int dataNumber, int dimensionalityDowm)
{
	fstream f_pca;
	f_pca.open(name,ios::in);
	if(!f_pca)
	{//如果開啟檔案失敗，fddd為0；成功，fddd為非0
        cout<<"Fail to open file pca" <<endl;
    }
	vector<float> feature;
	float f;

	while(f_pca>>f) 
	{
		feature.push_back(f);
		//cout<<feature[i]<<endl;
	}
	f_pca.close();

	CvMat *Vector1;
	CvMat *Project;
	CvMat *AvgVector;
	CvMat *EigenValue_Row;
	CvMat *EigenVector;

	dataNumber=dataNumber-2;
	Vector1=cvCreateMat(dataNumber,dimensionality,CV_32FC1);
	cvSetData(Vector1,&feature[0],Vector1->step);

	AvgVector=cvCreateMat(1,dimensionality,CV_32FC1);
	EigenValue_Row=cvCreateMat( min(dataNumber,dimensionalityDowm) ,1,CV_32FC1);
	EigenVector=cvCreateMat( min(dataNumber,dimensionalityDowm) ,dimensionality,CV_32FC1);
	
	cvCalcPCA(Vector1,AvgVector,EigenValue_Row,EigenVector,CV_PCA_DATA_AS_ROW);
	//cvProjectPCA(Vector1,AvgVector,EigenVector,Project);

	//printf("==========\n");
	//PrintMatrix(AvgVector,1,dimensionality);
	//
	//printf("\nEigne Value:\n");
	//PrintMatrix(EigenValue_Row,dimensionalityDowm,1);
	//
	//printf("\nEigne Vector:\n");
	//PrintMatrix(EigenVector,dimensionalityDowm,dimensionality);

	fstream result_pca;
	result_pca.open(name+"_PCA",ios::out);
	if(!result_pca)
	{//如果開啟檔案失敗，fddd為0；成功，fddd為非0
        cout<<"Fail to open file result_pca" <<endl;
	}

	for(int i=0;i<( (EigenVector->cols)*(EigenVector->rows) );i++)
	{
		result_pca<<EigenVector->data.fl[i];
	}
	
	result_pca.close();
	return result_pca;
}

string feature_to_string_ddd(vector<float>& feature,string name)
{
	cout << name << " feature to string ddd" << endl;
	string result_ddd;
	result_ddd="";

	dimensionality=feature.size();
	cout<<dimensionality<<endl;

	for(int i=0;i<feature.size();i++)
	{
		result_ddd = result_ddd + float2str(feature[i]) + " ";
	}
	return result_ddd;
}

string feature_extract_ddd(string name)
{
	vector<Mat> imgPatch;
	vector<float> feature;
	Mat image = imread(name.c_str(),0);
	if(!image.empty())
	{
		cout << name << " processing ddd" << endl;
		patch(image, imgPatch, 7, 7);
		destroyAllWindows();
		gabor(feature, imgPatch);
		destroyAllWindows();
	}
	return feature_to_string_ddd(feature,name);
}

vector<string> stage_ddd(string folder)
{
	vector<string> result_ddd, names;
	string result_pca;
	names = get_file_names(folder);
	dataNumber=names.size();
	cout<<dataNumber<<endl;
	for(int i=0;i<names.size();i++)
	{
		Mat image = imread(names[i]);
		if(!image.empty())
		{
			result_ddd.push_back(feature_extract_ddd(names[i]));
		}
	}

	fstream f_ddd;
    f_ddd.open("result_ddd", ios::out);
	if(!f_ddd)
	{//如果開啟檔案失敗，fddd為0；成功，fddd為非0
        cout<<"Fail to open file ddd" <<endl;
    }

	for(int i=0;i<result_ddd.size();i++)
	{
		f_ddd<<result_ddd[i]<<endl;
	}
    f_ddd.close();

//	feature_pca("result_ddd");

	return result_ddd;
}

vector<float> project_pca(vector<float> feature,string name)
{
	fstream result_pca;
	vector<float> eigenvector;
	vector<float> project;

	result_pca.open(name,ios::in);
	if(!result_pca)
	{//如果開啟檔案失敗，fddd為0；成功，fddd為非0
        cout<<"Fail to open file pca" <<endl;
    }
	float f;

	//vector<float> eigenvector;
	while(result_pca>>f) 
	{
		eigenvector.push_back(f);
		//cout<<eigenvector[i]<<endl;
	}
	result_pca.close();

	CvMat *Vector1;
	CvMat *Project;
	CvMat *AvgVector;
	CvMat *EigenValue_Row;
	CvMat *EigenVector;

	dataNumber=dataNumber-2;
	Vector1=cvCreateMat(dimensionality,1,CV_32FC1);
	cvSetData(Vector1,&feature[0],Vector1->step);

	AvgVector=cvCreateMat(1,dimensionality,CV_32FC1);
	//EigenValue_Row=cvCreateMat( min(dataNumber,dimensionalityDowm) ,1,CV_32FC1);
	EigenVector=cvCreateMat( min(dataNumber,dimensionalityDowm) ,dimensionality,CV_32FC1);
	cvSetData(EigenVector,&eigenvector[0],EigenVector->step);

	//cvCalcPCA(Vector1,AvgVector,EigenValue_Row,EigenVector,CV_PCA_DATA_AS_ROW);
	Project=cvCreateMat(min(dataNumber,dimensionalityDowm),1,CV_32FC1);
	cvProjectPCA(Vector1,AvgVector,EigenVector,Project);

	//printf("==========\n");
	//PrintMatrix(AvgVector,1,dimensionality);
	//
	//printf("\nEigne Value:\n");
	//PrintMatrix(EigenValue_Row,dimensionalityDowm,1);
	//
	//printf("\nEigne Vector:\n");
	//PrintMatrix(EigenVector,dimensionalityDowm,dimensionality);

	for(int i=0;i<( (Project->cols)*(Project->rows) );i++)
	{
		project.push_back( Project->data.fl[i] );
	}
	
	return project;
}

*/