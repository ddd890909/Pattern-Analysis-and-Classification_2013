#include "work.h"

LDA_custom LDA_proc(Mat src){
	int temp;
	fstream fp, LDA_cat_file;
	vector<int> ldacat;
	fp.open("TRAIN/TRAIN.cat", ios::in);
	while(fp >> temp){
			ldacat.push_back(temp);
	}
	fp.close();
	LDA_custom lda(src, ldacat);
	return lda;

}

Mat LDA_proj(Mat ldaset, LDA_custom lda){
	Mat projed = lda.project(ldaset);
	return projed;
}