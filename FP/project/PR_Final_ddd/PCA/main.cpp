//PCA

#include <fstream> 

#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;
#define min(a,b) (a<b)?a:b

int dimensionality=105840;
int dataNumber=6;
int dimensionalityDowm=3;

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

int main()
{
	fstream f_pca;
	f_pca.open("f_ddd",ios::in);
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
	CvMat *Vector_pca;
	CvMat *AvgVector;
	CvMat *EigenValue_Row;
	CvMat *EigenVector;

	Vector1=cvCreateMat(dataNumber,dimensionality,CV_32FC1);
	cvSetData(Vector1,&feature[0],Vector1->step);
	//cout<<Vector1->data.fl[635039];
	AvgVector=cvCreateMat(1,dimensionality,CV_32FC1);
	EigenValue_Row=cvCreateMat( min(dataNumber,dimensionalityDowm) ,1,CV_32FC1);
	EigenVector=cvCreateMat( min(dataNumber,dimensionalityDowm) ,dimensionality,CV_32FC1);
	Vector_pca=cvCreateMat( min(dataNumber,dimensionalityDowm) ,dataNumber,CV_32FC1);
	
	cvCalcPCA(Vector1,AvgVector,EigenValue_Row,EigenVector,CV_PCA_DATA_AS_ROW);
	//cvProjectPCA(Vector1,AvgVector,EigenVector,Vector_pca);

	printf("==========\n");
	PrintMatrix(AvgVector,1,dimensionality);
	
	printf("\nEigne Value:\n");
	PrintMatrix(EigenValue_Row, min(dataNumber,dimensionalityDowm) ,1);
	
	printf("\nEigne Vector:\n");
	PrintMatrix(EigenVector, min(dataNumber,dimensionalityDowm) ,dimensionality);
	
	cvWaitKey(0);
}