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

string int2str(int ); //�Nint��str
string float2str(float ); //float��str
vector<string> get_file_names(string); //����Ƨ��ɦW�^�Ǭ۹諸���|���ɦW ex. haha/123.jpg
vector<string> get_real_file_names(string); //����Ƨ��ɦW�^�ǯu���ɦW ex. 123.jpg
void patch(Mat, vector<Mat> &, int, int); //���ι�
void stage_1(string, string); //facecropping �Ĥ@��input����ϸ�Ƨ��W �ĤG�Ӭ���X��m����Ƨ��ɦW
string feature_to_string(vector<Mat>&,string);//�Nfeature_extract�o�쪺feature�নlibsvm���榡
string feature_extract(string);//�N��@�ϰ�feature extract�æ^��libsvm��feature�榡
vector<string> stage_2(string);//��input����Ƨ����Ҧ����ϰ�feature extract�æ^��libsvm�榡��feature vector