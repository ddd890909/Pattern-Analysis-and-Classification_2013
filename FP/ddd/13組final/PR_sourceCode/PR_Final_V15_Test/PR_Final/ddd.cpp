#include "gabor.h"
#include "faceDefine.h"
#include "work.h"
#include "ml.h"
	
Mat KNN_proc(Mat train,Mat classes,Mat test)
{
	//KNN 
		//get class mat
	Mat trainClasses;
	trainClasses=classes;
		//learn classifier
	cout << "train KNN for 147 5-dimensional vector " << endl;
	int maxK=1;

	Mat trainData=train;
	CvKNearest knn(trainData,trainClasses, Mat(), false, maxK);
	
		//find nearest
	Mat testData=test;
	Mat knnNearests;
	Mat knnResult;
	Mat knnDists;
	float response;
	response = knn.find_nearest(testData,maxK,knnResult,knnNearests,knnDists); 
	
		//write KNN result to file
	cout << "writing knnReslt.knn to file" << endl;
	fstream knn_result;
	knn_result.open("knnReslt.knn",ios::out);
	for(int i=0;i<knnResult.rows;i++)
	{
		float *data_knn=knnResult.ptr<float>(i);
		for(int j=0;j<knnResult.cols;j++)
		{
			knn_result << data_knn[j] << " ";
		}
		knn_result << endl;
	} 

	return knnResult;
	//KNN end
}