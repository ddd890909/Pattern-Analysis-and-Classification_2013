#include<string>
#include<vector>
#include<sstream>
#include "FaceDetection.h"
#include "OpenCVASMAlignment.h"
#include "FaceCrop.h"
#include "faceDefine.h"
#include "gabor.h"

using namespace cv;
using namespace std;

string int2str(int ); //將int轉str
string float2str(float ); //float轉str
vector<string> get_file_names(string); //給資料夾檔名回傳相對的路徑的檔名 ex. haha/123.jpg
vector<string> get_real_file_names(string); //給資料夾檔名回傳真實檔名 ex. 123.jpg
void patch(Mat, vector<Mat> &, int, int); //切割圖
void stage_1(string, string); //facecropping 第一個input為原圖資料夾名 第二個為輸出位置的資料夾檔名
string feature_to_string(vector<Mat>&,string);//將feature_extract得到的feature轉成libsvm的格式
string feature_extract(string);//將單一圖做feature extract並回傳libsvm的feature格式
vector<string> stage_2(string);//對input的資料夾內所有的圖做feature extract並回傳libsvm格式的feature vector