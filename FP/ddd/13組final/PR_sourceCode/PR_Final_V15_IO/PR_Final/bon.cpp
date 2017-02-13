#include "work.h"
#include "gabor.h"

void stage_1(string category, string source_folder){
	vector<FP> FaceFP;              //Alignment後存放
	vector<FaceInfo> CropFaceSet;   //包含Crop後要偵測的資訊
	vector<IplImage*> FaceSet;      //存放Crop後灰階影像
	vector<CvRect> faceWindow; //store each face's rect position
	vector<CvMat*> alignmentResult; // stroe facial points
	extern FaceDetection *face_detector;
	extern OpenCVASMAlignment *myAlignment;

	IplImage *img;
	IplImage *drawimg;

	fstream cato;
	cato.open("TEST/GlobalSVM/GlobalSVMTEST.result", ios::in);

	fstream result;
	result.open(source_folder+"/ExpResults.txt", ios::out);

	vector<string> file_names, real_file_names;

	file_names = get_file_names(source_folder);
	real_file_names = get_real_file_names(source_folder);

	for(int j=0;j<file_names.size();j++){
		if(  (img = cvLoadImage(file_names[j].c_str())) ==0 )// Load img
			printf("%s cannot open!\n", file_names[j].c_str());
		else{
			int cat;
			cato >> cat;
			drawimg = cvCloneImage(img);
 			faceWindow = face_detector->Detect(img);//人臉偵測
			if(faceWindow.size() != 0){ //若有偵測到一個以上的人臉
				CvScalar color;
				if(cat == 1) color = CV_RGB(255, 0, 0);
				else if(cat == 2) color = CV_RGB(0, 255, 0);
				else if(cat == 3) color = CV_RGB(0, 0, 255);
				else if(cat == 4) color = CV_RGB(255, 255, 0);
				else if(cat == 5) color = CV_RGB(255, 0, 255);
				else if(cat == 6) color = CV_RGB(0, 255, 255);
				for(int i=0; i<faceWindow.size(); i++)
					cvRectangle(drawimg,cvPoint(faceWindow[i].x, faceWindow[i].y),cvPoint(faceWindow[i].x+faceWindow[i].width, faceWindow[i].y+faceWindow[i].height),color);
				myAlignment->SetImage(img);
				for(int i=0; i<faceWindow.size(); i++)	
					alignmentResult.push_back(myAlignment->calcAlignment(faceWindow[i]));
				for(int i=0; i<alignmentResult.size(); i++){
					//for (int k = 0; k < alignmentResult[i]->rows; k++)
						//if(k == 31 ||k == 36 || k == 66 || k == 67)
							//cvCircle(drawimg, cvPoint(cvmGet(alignmentResult[i], k, 0), cvmGet(alignmentResult[i], k, 1)), 1, CV_RGB(255, 0, 0), 2);
						//else
							//cvCircle(drawimg, cvPoint(cvmGet(alignmentResult[i], k, 0), cvmGet(alignmentResult[i], k, 1)), 1, CV_RGB(0, 255, 0), 2);
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
/*				cvLine(drawimg,CropFaceSet[i].leftdown,CropFaceSet[i].leftTop,color,2);
				cvLine(drawimg,CropFaceSet[i].leftTop,CropFaceSet[i].rightTop,color,2);
				cvLine(drawimg,CropFaceSet[i].rightTop,CropFaceSet[i].rightdown,color,2);
				cvLine(drawimg,CropFaceSet[i].rightdown,CropFaceSet[i].leftdown,color,2)*/;
				//cvShowImage("test", FaceSet[i]);
				result << real_file_names[j] << " " << cat-1 << endl;
				// 將原附檔名刪除
				stack<char> tail;
				for(int k=real_file_names[j].size()-1; k>=0;k--){
					if(real_file_names[j][k]=='.'){
						tail.push(real_file_names[j][k]);
						real_file_names[j].pop_back();
						real_file_names[j].push_back('_');
						real_file_names[j].push_back(cat-1+48);
						while(!tail.empty()){
							real_file_names[j].push_back(tail.top());
							tail.pop();
						}
						break;
					}
					else{
						tail.push(real_file_names[j][k]);
						real_file_names[j].pop_back();
					}
				}
				string resultdir;
				resultdir = source_folder +"/"+ "Result_" + real_file_names[j];
				cvSaveImage( resultdir.c_str(), drawimg);
			}
			alignmentResult.clear();	
			faceWindow.clear();
			FaceFP.clear();	
			cvReleaseImage(&img);
			cvReleaseImage(&drawimg);	
		}
	}
	cato.close();
	result.close();
}