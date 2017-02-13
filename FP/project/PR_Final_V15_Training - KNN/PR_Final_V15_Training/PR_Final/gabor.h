#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

#define radi 5
#define grid 18
#define PI 3.14159265

#include <cmath>
#include <vector>


void win(Mat);
Mat mkKernel(int, double , double , double , double, double);
void Process(int , void *);
void filter_generate(vector<Mat> &, int *, int, int );
void circular(vector<float> &, vector<Mat>);
void gabor(vector<float> &, vector<Mat>);