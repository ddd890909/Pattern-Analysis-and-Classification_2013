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

string feature_to_string(vector<float>& feature,string name){
	cout << name << " feature to string" << endl;
	string result;
	int category=-1;
	int in_cat=0;
	for(int i=0;i+1<name.size();i++){
		if(name[i]=='A' && name[i+1]=='N'){
			category=1;
			in_cat=1;
		}
		else if(name[i]=='D' && name[i+1]=='I'){
			category=2;
			in_cat=1;
		}
		else if(name[i]=='F' && name[i+1]=='E'){
			category=3;
			in_cat=1;
		}
		else if(name[i]=='H' && name[i+1]=='A'){
			category=4;
			in_cat=1;
		}
		else if(name[i]=='S' && name[i+1]=='A'){
			category=5;
			in_cat=1;
		}
		else if(name[i]=='S' && name[i+1]=='U'){
			category=6;
			in_cat=1;
		}
		if(in_cat==1)
			break;
	}
	//cout << category << endl;
	result+=int2str(category);
	result+=" ";
	for(int i=0;i<feature.size();i++){
		result = result + int2str(i+1) + ":" + float2str(feature[i]) + " ";
	}
	return result;
}

string feature_extract(string name){
	vector<Mat> imgPatch;
	vector<float> feature;
	Mat image = imread(name.c_str(),0);
	if(!image.empty()){
		cout << name << " processing" << endl;
		patch(image, imgPatch, 7, 7);
		destroyAllWindows();
		gabor(feature, imgPatch);
		destroyAllWindows();
	}
	return feature_to_string(feature,name);
}

vector<string> stage_2(string folder){
	vector<string> result, names;
	names = get_file_names(folder);
	for(int i=0;i<names.size();i++){
		Mat image = imread(names[i]);
		if(!image.empty()){
			result.push_back(feature_extract(names[i]));
		}
	}

	fstream fp;
    fp.open("result", ios::out);
    if(!fp){//如果開啟檔案失敗，fp為0；成功，fp為非0
        cout<<"Fail to open file " <<endl;
    }
	for(int i=0;i<result.size();i++){
		fp<<result[i]<<endl;
	}
    fp.close();
	return result;
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
	while(GetLastError()!=ERROR_NO_MORE_FILES){	
		string dir = folderwithout+filename; 
		names.push_back(dir);
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
	while(GetLastError()!=ERROR_NO_MORE_FILES){	
		string dir = filename; 
		names.push_back(dir);
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
	FaceDetection *face_detector;
	OpenCVASMAlignment *myAlignment;

	face_detector = new FaceDetection("cascade Data\\haarcascade.xml");
    myAlignment = new OpenCVASMAlignment();
	myAlignment->setModelPath("ASM Data/FrontalFace_best.amf");
	if (myAlignment->loadModel() == -1) {
		fprintf(stderr,"Could not load ASM model...\n");
		return;
	}
    
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
				cvShowImage("test", FaceSet[i]);

				// 將原附檔名刪除
				for(int k=real_file_names.size()-1; k>=0;k--){
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
				cout << resultdir << endl;
				//cvNamedWindow("haha");
				//cvShowImage("haha",FaceSet[i]);
				//cvWaitKey(0);
				cvSaveImage( resultdir.c_str(), FaceSet[i]);
			}
			cvNamedWindow("ivFace");
			cvShowImage("ivFace",drawimg);
			//cvWaitKey(0);
			alignmentResult.clear();	
			faceWindow.clear();
			FaceFP.clear();	
			cvReleaseImage(&img);
			cvReleaseImage(&drawimg);	
		}
	}
}