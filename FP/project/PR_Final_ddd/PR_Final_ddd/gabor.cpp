#include "gabor.h"
#include <iostream>

using namespace std;

int kernel_size = 21;
int pos_sigma = 5;
int pos_lm = 50;
int pos_th = 0;
int pos_psi = 90;
Mat src_f;
Mat dest;

void win(Mat image){
	Mat src;
	cvtColor(image, src, CV_BGR2GRAY);
	namedWindow("Src", 1);
	imshow("Src", image);
	image.convertTo(src_f, CV_32F, 1.0/255, 0);
	if(!kernel_size%2){
		kernel_size+=1;
	}
	namedWindow("Process window", 1);
	createTrackbar("Sigma", "Process window", &pos_sigma, kernel_size, Process);
	createTrackbar("Lambda", "Process window", &pos_lm, 100, Process);
	createTrackbar("Theta", "Process window", &pos_th, 180, Process);
	createTrackbar("Psi", "Process window", &pos_psi, 360, Process);
	Process(0,0);
	waitKey(0);
}

Mat mkKernel(int ks, double sig, double th, double gm, double lm, double ps){
	int hks = (ks-1)/2;
	double theta = th*CV_PI/180;
	double psi = ps*CV_PI/180;
	double del = 2.0/(ks-1);
	double gamma = gm;
	double lmbd = lm;
	double sigma = sig/ks;
	double x_theta;
	double y_theta;
	cv::Mat kernel(ks,ks, CV_32F);
	for(int y=-hks; y<=hks; y++){
		for(int x=-hks; x<=hks; x++){
			x_theta = x*del*cos(theta)+y*del*sin(theta);
			y_theta = -x*del*sin(theta)+y*del*cos(theta);
			kernel.at<float>(hks+y,hks+x) = (float)exp(-0.5*(pow(x_theta,2)+pow(gamma*y_theta,2))/pow(sigma,2))* cos(2*CV_PI*x_theta/lmbd + psi);
		}
	}
	return kernel;
}

void Process(int , void *){
	double sig = pos_sigma;
	double gm = 1.0;
	double lm = 0.5+pos_lm/100.0;
	double th = pos_th;
	double ps = pos_psi;
	Mat kernel = mkKernel(kernel_size, sig, th, gm, lm, ps);
	filter2D(src_f, dest, CV_32F, kernel);
	imshow("Process window", dest);
	Mat Lkernel(kernel_size*20, kernel_size*20, CV_32F);
	resize(kernel, Lkernel, Lkernel.size());
	Lkernel /= 2.;
	Lkernel += 0.5;
	imshow("Kernel", Lkernel);
	Mat mag;
	pow(dest, 2.0, mag);
	imshow("Mag", mag);
}

void filter_generate(vector<Mat> &wavelet, int *size, int orient, int sz){
	double sigma, theta=0, gamma = 0.5, lambda, psi=0;
	double dt_theta = 180 /orient;
	int S;
	for(int i=0; i<sz; i++){
		S = size[i];
		for(int j=0; j<orient; j++){
			theta = dt_theta*j;
			sigma = 0.0036*S*S + 0.35*S + 0.18;
			lambda = sigma/0.8;
			Mat temp = mkKernel(S, sigma, theta, gamma, lambda, psi);
			//cout << temp << endl;
			//system("pause");
			wavelet.push_back(temp);
		}
	}
}

void circular(vector<float> &feature, Mat img){
	int rad = img.cols < img.rows ? img.cols:img.rows;
	rad /= (radi*2);
	int rmax=-1, gmax=-1;
	float grids[radi][grid] = {0.0};
	int count[radi][grid] = {0};
	int sz = img.rows * img.cols;
	float *data = img.ptr<float>(0);
	for(int j=0; j<img.rows; j++){
		for(int i=0; i<img.cols; i++){
			float tempx = i-img.cols/2;
			float tempy = j-img.rows/2;
			int r, g;
			float gfloat;
			r = floor(sqrt(tempx*tempx+tempy*tempy)/rad);
			gfloat = atan2(tempy, tempx);
			if(gfloat < 0) gfloat+=(2*PI);
			g = floor(gfloat/(2*CV_PI/grid));
			if(r<radi && g<grid){
				grids[r][g] += data[i+j*img.cols];
				count[r][g]++;
			}
			//if(r > rmax) rmax = r;
			//if(g > gmax) gmax = g;
		}
	}
	//cout << "rmax = " << rmax << endl << "gmax = " << gmax << endl;
	//system("pause");
	for(int i=0; i<radi; i++){
		for(int j=0; j<grid; j++){
			float ttt=grids[i][j]/count[i][j];
			if(count[i][j] == 0) ttt = 0;
			if(ttt >= 0)
				feature.push_back(ttt);
			else{
				cout << i << "," << j << endl;
				cout << grids[i][j] << "\t" << count[i][j] << endl;
			}
		}
	}
}

void gabor(vector<float> &feature, vector<Mat> img){
	int size[3]={11, 21, 29};
	Point anchor;
	vector<Mat> wavelet;
	filter_generate(wavelet, size, 8, 3);
	//namedWindow("filter");
	//namedWindow("patch");
	//namedWindow("response");
	for(vector<Mat>::iterator it_i=img.begin(); it_i!=img.end(); it_i++){	
		for(vector<Mat>::iterator it_j=wavelet.begin(); it_j!=wavelet.end(); it_j++){
			int tempx = ((*it_j).cols+1)/2, tempy = ((*it_j).rows+1)/2;
			anchor = Point(-1*tempx, -1*tempy);
			Mat temp, src_f;
			(*it_i).convertTo(src_f, CV_32F, 1.0/255, 0);
			//imshow("filter", *it_j);
			//imshow("patch", src_f);
			filter2D(src_f, temp, CV_32F, (*it_j));
			circular(feature, temp);
			//imshow("response", temp);
			//waitKey(0);
		}
	}
}