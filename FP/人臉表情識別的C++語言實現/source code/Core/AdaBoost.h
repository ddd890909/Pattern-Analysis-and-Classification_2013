/****************************************************************************
Copyright (c) 2007.  All Rights Reserved.            
  
--------------------------------------------------------------------
Permission to use, copy, or modify this software and its documentation
for educational and research purposes only and without fee is hereby
granted, provided that this copyright notice appear on all copies and
supporting documentation.  For any other uses of this software, in
original or modified form, including but not limited to distribution
in whole or in part, specific prior permission must be obtained from
NJU Magic and the authors.  These programs shall not be used, rewritten,
or adapted as the basis of a commercial software or hardware product
without first obtaining appropriate licenses from NJU Magic. NJU Magic 
makes no representations about the suitability of this software for any
purpose.  It is provided "as is" without express or implied warranty.

---------------------------------------------------------------------

 File: adaboost.h                                                                         
 Authors: Yao Wei  
 Date Created    : 2007-8-11
                                          
****************************************************************************/

#ifndef ADA_BOOST_H
#define ADA_BOOST_H

#include "WeakLearner.h"

class CORE_API Boosting  
{
private:
	int n_samples;         //number of train sample
	int n_input;            //dimension of each sample
	int max_iter;       //max number of boost step
	WeakLearner *leaner;   //all weaker leaner
	
	//information of train set contains: #class, #each class and each class item
	//that is to say, if we have label={1,1,2,6,4,1,4,4,6,1,6,2,1,1,1,4,4}; we will get 
    //# class: count=4
	//concrete label:      eachlabel={1,2,6,4}
	//each count of label: eachcount={7,2,3,5} 
	/* it acts as a map: std::map<int, int> */
	struct CORE_API taginfo
	{
		taginfo()
		{
			count = 0;
			eachlabel = 0;
			eachcount = 0;
		}
		int count;                   //class count;
		int *eachcount;              //each class's count
		double *eachlabel;           //store all label
	}info;
	
	Mat *submat;    //sub-sample correspond to each class


public:
		
	// Free memory
	void Free();

	//predict a sample using majority voting
	double Vote(double *feature,int n_input);
	
	//train Boosting from a file
	void Train(const char * filename);
	
	//test from a file
	void Test(const char * filename);
	
	//predict from a file
	void Predict(const char * filename, const char * outputfile);
	
	//save model of Boosting
	void SaveModel(const char * filename);
	
	//load model of Boosting
	int LoadModel(const char * filename);
	
	//set iteration of Boosting
	void SetIteration(int _max_iter){max_iter = _max_iter;}
	
	Boosting();    //Construction

	virtual ~Boosting();

private:
	//train Multi-Class Boosting
	void DoTrain(); 
	
	//train Boosting 1 vs 1
	WeakLearner TrainOneVsOne
		(const Mat &mat1,const Mat &mat2,double label1,double label2);
	
	//read data from a file
	void ReadData(const char *filename,Mat &data,Vec &label);
	
	//scan file to get size
	void GetSize(const char *filename, int &m, int &n);
	
	//get information of train data
	void GetClassInfo(const Vec &responses);
	
	//preprocess train data
	void Preprocess(const Mat &train,const Vec &responses);
	
	//unroll data according to each class label
	void UnrollData(const Mat &train,const Vec &responses);
	
	int Find(double label)
	{
		int count = info.count;
		for(int j = 0; j<count; j++)
			if(info.eachlabel[j] == label)
				return j;
		return -1;
	}	
};

#endif //ADA_BOOST_H
