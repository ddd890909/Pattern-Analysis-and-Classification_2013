// PR 2011 fall sample code
// T.A. Chen Shen-Shi 
// d98922030@csie.ntu.edu.tw

#include "faceDefine.h"

// include toolkit class
#include "FaceDetection.h"
#include "OpenCVASMAlignment.h"
#include "FaceCrop.h"

FaceDetection *face_detector;
OpenCVASMAlignment *myAlignment;
FaceCrop *FaceCropper;
vector<FaceInfo> CropFaceSet;   //包含Crop後要偵測的資訊
vector<IplImage*> FaceSet;      //存放Crop後灰階影像
vector<FP> FaceFP;              //Alignment後存放
char tmp[100];
char folder[100]; //待處理影像的資料夾
char studentID[100]; //學生ID
char type[100];
char folderwithout[100];
char filename[150];
char dir[300];
char resultdir[200];
char savingfolder[200];

void FaceCropping(IplImage *img){
	        IplImage *ProcessImage = cvCloneImage(img);
			CropFaceSet.clear();
			FaceSet.clear(); 
			
			FaceCropper = new FaceCrop();
			FaceCropper->Initial(ProcessImage, FaceFP);
			FaceCropper->setParameter("CropConfig.txt");
			//FaceCropper ->setParameter(0.9,1.5,2.0,80,100,5.0);
            //FaceCropper->CropAllGrayLevelFace(&CropFaceSet);
			FaceCropper->CropAllFace(&CropFaceSet, &FaceSet );
			cvReleaseImage(&ProcessImage);
            delete FaceCropper;
			
}

void main()
{
	vector<CvRect> faceWindow; //store each face's rect position
	vector<CvMat*> alignmentResult; // stroe facial points

	face_detector = new FaceDetection("cascade Data\\haarcascade.xml");
    myAlignment = new OpenCVASMAlignment();
	myAlignment->setModelPath("ASM Data/FrontalFace_best.amf");
	if (myAlignment->loadModel() == -1) {
		fprintf(stderr,"Could not load ASM model...\n");
		return;
	}
    

	IplImage *img;
	IplImage *drawimg;

	cout << "Please input your folder: ";
	cin >> tmp;
	cout << "Enter your student ID (ex:d98922030):";
	cin >> studentID;
	sprintf(savingfolder,"./%s_%s",tmp, studentID); 
  	mkdir(savingfolder);

	sprintf(folder, "%s/*.*", tmp);
	sprintf(folderwithout, "%s/", tmp);

	size_t origsize = strlen(folder) + 1;
    const size_t newsize = 100;
    size_t convertedChars = 0;
    wchar_t wcstring[newsize];
    mbstowcs_s(&convertedChars, wcstring, origsize, folder, _TRUNCATE);

    WIN32_FIND_DATA FindFileData;
    HANDLE FileHandle;
	FileHandle = FindFirstFile(wcstring, &FindFileData);
    WideCharToMultiByte( CP_ACP, 0, FindFileData.cFileName, -1,filename, sizeof(filename), NULL, NULL ); 
	while(GetLastError()!=ERROR_NO_MORE_FILES){	
	   		   sprintf(dir,"%s/%s", folderwithout, filename); 
			   if(  (img = cvLoadImage(dir)) ==0 )// Load img
					   printf("%s cannot open!\n", dir);			
				else{
					        drawimg = cvCloneImage(img);
 					        faceWindow = face_detector->Detect(img);//人臉偵測
	                        if(faceWindow.size() != 0) //若有偵測到一個以上的人臉
							{
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
							FaceCropping(img);//人臉裁切
							for(int i=0; i<CropFaceSet.size()  ;i++){
								cvLine(drawimg,CropFaceSet[i].leftdown,CropFaceSet[i].leftTop,CV_RGB(255,255,0),2);
								cvLine(drawimg,CropFaceSet[i].leftTop,CropFaceSet[i].rightTop,CV_RGB(255,255,0),2);
								cvLine(drawimg,CropFaceSet[i].rightTop,CropFaceSet[i].rightdown,CV_RGB(255,255,0),2);
								cvLine(drawimg,CropFaceSet[i].rightdown,CropFaceSet[i].leftdown,CV_RGB(255,255,0),2);
								cvShowImage("test", FaceSet[i]);

								// 將原附檔名刪除
                                char *node = strrchr(filename, '.');
			                    if(*node != NULL) *node ='\0';
								sprintf(resultdir,"%s/%s.tiff", savingfolder, filename); 
							    cvSaveImage( resultdir, FaceSet[i]);
							 }
							cvNamedWindow("ivFace");
							cvShowImage("ivFace",drawimg);
							cvWaitKey(0);
							alignmentResult.clear();	
							faceWindow.clear();
							FaceFP.clear();	
							cvReleaseImage(&img);
			                cvReleaseImage(&drawimg);		
			   }
			   
			   FindNextFile(FileHandle,&FindFileData);
			   WideCharToMultiByte( CP_ACP, 0, FindFileData.cFileName, -1,filename, sizeof(filename), NULL, NULL );
	  }
}