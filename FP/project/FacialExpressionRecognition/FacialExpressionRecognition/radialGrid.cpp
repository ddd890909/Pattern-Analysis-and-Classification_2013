//radial grid 18*5 placed on an image

#include <iostream>
#include <vector>
#include <math.h>
#include <opencv.hpp>

using namespace std;
using namespace cv;

#define max(a,b) (a>b?a:b)
#define min(a,b) (a<b?a:b)
#define PI acos(-1.0)

void RadialGrid(Mat image)
{
	int channels = image.channels();
    int nRows = image.rows ;
    int nCols = image.cols* channels;

	int width = image.cols;
    int height = image.rows;
	int centreX = image.cols/2;
	int centreY = image.rows/2;
	double r=min(width,height) /2;
	
	vector<Vec3b> p[18][5];
	Vec3b v[18][5];
	//vector<double> p[18][5];
	//double v[18][5];

	for (int row = 0; row < image.rows; row++)
    {
        for (int col = 0; col < image.cols; col++)
        {   
			double distance=sqrt( pow( (col-centreX) , 2 ) +pow( (row-centreY) , 2 ) );
            int level= int( distance / (r/5.0) );

			double radian=acos( (col-centreX) / distance );
			if ( (row-centreY) < 0 ) 
			{
				radian = 2*PI-radian;
			}
			int angle= int( radian / (2*PI/18.0) );

			if(level<5)
				p[angle][level].push_back( image.at<Vec3b>(row, col) );			
        }
    }
	
	for(int i=0;i<18;i++)
	{
		for(int j=0;j<5;j++)
		{
			double vSumB=0;
			double vSumG=0;
			double vSumR=0;
			//double vSum=0;

			cout<< p[i][j].size() <<endl;
			for(int k=0;k<p[i][j].size();k++)
			{
				vSumB+=p[i][j].at(k)[0];
				vSumG+=p[i][j].at(k)[1];
				vSumR+=p[i][j].at(k)[2];
				//vSum+=p[i][j].at(k);
			}
			
			v[i][j][0]=vSumB/ ( p[i][j].size() );
			v[i][j][1]=vSumG/ ( p[i][j].size() );
			v[i][j][2]=vSumR/ ( p[i][j].size() );
			//v[i][j]=vSum/ ( p[i][j].size() );
			cout<< v[i][j] <<endl;
		}
	}
}


int main(int argc, char** argv)
{
	vector<Mat> feature;
	Mat result;
	Mat image = imread("test.jpg");
	
	RadialGrid(image);
	
	imshow("look", image);
	waitKey(0);
	
	return 0;
}