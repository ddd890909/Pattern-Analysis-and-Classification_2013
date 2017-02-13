#include<string>
#include<vector>
#include<sstream>
#include "lda.h"
#include <fstream>
#include <iostream>

using namespace cv;
using namespace std;


//bon
LDA_custom LDA_proc(Mat);
Mat LDA_proj(Mat , LDA_custom );


//queenhashin
PCA PCA_proc(Mat, int down);
Mat PCA_proj(Mat, PCA);
PCA pca_load(const FileStorage&);
void pca_save(FileStorage&, PCA);

string int2str(int);
Mat svm_to_Mat(string);
void Mat_to_svm(Mat, string, vector<int>);
void normalize_custom(Mat &, Mat);