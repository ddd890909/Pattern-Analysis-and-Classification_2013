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
void patch(Mat, vector<Mat> &, int, int); //���ι�
void stage_1(string, string); //facecropping �Ĥ@��input����ϸ�Ƨ��W �ĤG�Ӭ���X��m����Ƨ��ɦW
void stage_2(string, string);//��input����Ƨ����Ҧ����ϰ�feature extract�æ^��libsvm�榡��feature vector
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
string int2str(int ); //�Nint��str
string float2str(float ); //float��str
bool cat_matching(string, string);
vector<string> get_file_names(string); //����Ƨ��ɦW�^�Ǭ۹諸���|���ɦW ex. haha/123.jpg
vector<string> get_real_file_names(string);
void feature_to_string(vector<Mat>&,string);//�Nfeature_extract�o�쪺feature�নlibsvm���榡
void feature_extract(string, string, vector<CvMat*>);//�N��@�ϰ�feature extract�æ^��libsvm��feature�榡
void feature_extract_old(string, string);
void DATA_to_SVM(string, string, int);
PCA PCA_proc(Mat, int down);
Mat PCA_proj(Mat, PCA);
Mat get_cat(string);
void seperate_class(string, string, string, int);
void local_lda_to_mat(string, Mat &, Mat &, int, int );


//ddd
//string feature_extract_ddd(string); //�N��@�ϰ�feature extract�æ^�ǭ�榡
//vector<string> stage_ddd(string); //��input����Ƨ����Ҧ����ϰ�feature extract�A�Apca�æ^�ǭ�榡��feature vector
//fstream feature_pca(string, int ,int, int); //��pca
//void PrintMatrix(CvMat,int,int); //�L�x�}
//string feature_to_string_ddd(vector<float>& feature,string name); //�Nfeature_extract�o�쪺feature����榡�s
//vector<float> project_pca(vector<float> feature,string name); //�D��v���G
//
Mat KNN_proc(Mat,Mat,Mat);
Mat KNN_dist(Mat,Mat,Mat);

