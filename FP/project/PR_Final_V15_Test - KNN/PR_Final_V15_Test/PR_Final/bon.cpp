#include "work.h"
#include "gabor.h"

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

void stage_2(string folder, string file_name){
	vector<FP> FaceFP;              //Alignment後存放
	vector<FaceInfo> CropFaceSet;   //包含Crop後要偵測的資訊
	vector<CvRect> faceWindow; //store each face's rect position
	vector<CvMat*> alignmentResult; // stroe facial points
	vector<string> names;
	names = get_file_names(folder);
  	mkdir("TEST"); //create a folder
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
			if(x < 0 || y < 0 || x+w > img.cols-1 || y+h > img.rows-1) return false; 
			Rect roi(x, y, w, h);
			Mat temp = img(roi);
			result.push_back(temp);
		}
	}
	return true;
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


LDA_custom LDA_proc(string name, Mat src){
	int temp;
	fstream fp, LDA_cat_file;
	vector<int> ldacat;
	fp.open(name+".cat", ios::in);
	while(fp >> temp){
			ldacat.push_back(temp);
	}
	fp.close();
	LDA_custom lda(src, ldacat);
	return lda;

}

Mat LDA_proj(Mat ldaset, LDA_custom lda){
	Mat projed = lda.project(ldaset);
	return projed;
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

void KNN_compare(string answer, string output){
	fstream ans, out;
	ans.open(answer, ios::in);
	out.open(output, ios::in);
	int temp1, temp2, total=0, correct=0;
	while(ans >> temp1){
		out >> temp2;
		total++;
		if(temp1==temp2) correct++;
	}
	cout << "correct: " << correct << endl;
	cout << "total: " << total << endl;
	cout << "success rate: " << double(correct)/total *100 << endl;
}

void normalize_custom(Mat &output, Mat input){
	Mat mean, stddev;
	meanStdDev(input, mean, stddev);
	double mean_val = *(double *)mean.data;
	double stddev_val = *(double *)stddev.data;
	double *mat_data = output.ptr<double>(0);
	for(int i=0; i<output.cols; i++){
		mat_data[i]-=mean_val;
		mat_data[i]/=stddev_val;
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
	fs << "mean" << pca.mean;
	fs << "Eigenvalues" << pca.eigenvalues;
	fs << "Eigenvector" << pca.eigenvectors;
}