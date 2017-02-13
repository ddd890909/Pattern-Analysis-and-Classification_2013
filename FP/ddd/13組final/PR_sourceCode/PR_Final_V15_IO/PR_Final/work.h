#include<string>
#include<vector>
#include<sstream>
#include "FaceDetection.h"
#include "OpenCVASMAlignment.h"
#include "FaceCrop.h"
#include "faceDefine.h"
#include "gabor.h"
#include "lda.h"
#include <stack>

using namespace cv;
using namespace std;


//bon
void stage_1(string, string);

//queenhashin
string int2str(int ); //將int轉str
vector<string> get_file_names(string); //給資料夾檔名回傳相對的路徑的檔名 ex. haha/123.jpg
vector<string> get_real_file_names(string);



