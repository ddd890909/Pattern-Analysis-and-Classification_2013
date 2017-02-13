#include<string>
#include<vector>
#include<sstream>
#include "FaceDetection.h"
#include "OpenCVASMAlignment.h"
#include "FaceCrop.h"
#include "faceDefine.h"
#include "gabor.h"
#include "lda.h"

using namespace cv;
using namespace std;


//bon
bool cut(Mat, vector<Mat> &, vector<CvMat*>, int, int);
void patch(Mat, vector<Mat> &, int, int); //切割圖
void stage_1(string, string); //facecropping 第一個input為原圖資料夾名 第二個為輸出位置的資料夾檔名
void stage_2(string, string);//對input的資料夾內所有的圖做feature extract並回傳libsvm格式的feature vector
void stage_2_old(string, string);
LDA_custom LDA_proc(string, Mat);
Mat LDA_proj(Mat , LDA_custom );
void divide(vector<Mat> &, Mat, int);
void concat(Mat &result, vector<Mat> src);
void KNN_compare(string, string);
void normalize_custom(Mat &, Mat);
PCA pca_load(const FileStorage&);
void pca_save(FileStorage&, PCA);

//queenhashin
string int2str(int ); //將int轉str
string float2str(float ); //float轉str
bool cat_matching(string, string);
vector<string> get_file_names(string); //給資料夾檔名回傳相對的路徑的檔名 ex. haha/123.jpg
vector<string> get_real_file_names(string);
void feature_to_string(vector<Mat>&,string);//將feature_extract得到的feature轉成libsvm的格式
void feature_extract(string, string, vector<CvMat*>);//將單一圖做feature extract並回傳libsvm的feature格式
void feature_extract_old(string, string);
void DATA_to_SVM(string, string, int);
PCA PCA_proc(Mat, int down);
Mat PCA_proj(Mat, PCA);
Mat get_cat(string);
void seperate_class(string, string, string, int);
void local_lda_to_mat(string, Mat &, Mat &, int, int );


//ddd
//string feature_extract_ddd(string); //將單一圖做feature extract並回傳原格式
//vector<string> stage_ddd(string); //對input的資料夾內所有的圖做feature extract，再pca並回傳原格式的feature vector
//fstream feature_pca(string, int ,int, int); //做pca
//void PrintMatrix(CvMat,int,int); //印矩陣
//string feature_to_string_ddd(vector<float>& feature,string name); //將feature_extract得到的feature按原格式存
//vector<float> project_pca(vector<float> feature,string name); //求投影結果
//
Mat KNN_proc(Mat,Mat,Mat);
Mat KNN_dist(Mat,Mat,Mat);

