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
string int2str(int ); //�Nint��str
vector<string> get_file_names(string); //����Ƨ��ɦW�^�Ǭ۹諸���|���ɦW ex. haha/123.jpg
vector<string> get_real_file_names(string);



