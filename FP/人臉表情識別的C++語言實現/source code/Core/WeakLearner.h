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

 File: WeakLearner.h                                                  
 Authors: Yao Wei  
 Date Created    : 2007-8-11
 
****************************************************************************/

#ifndef WEAKLEARNER_H
#define WEAKLEARNER_H

#include "global.h"

//vector
typedef struct CORE_API Vec  
{
public:
	//free memory
	void Free();

	//set the length of vector, allocate memory
	void Set(int n);

	//get dimension of vector
	int length() const { return	n; }

	//operator []: so you can use vec[i]
	double &operator [](int i)const
	{
		return vector[i];
	}

	//get the raw vector data
	operator double* ()const {return vector;}
	/* double * getdata() const {return vector;} */

	Vec();

	Vec(int _n);

	virtual ~Vec();

private:
	//dimension
	int n;
	//data
	double *vector;
	//flag
	bool isfree;

}Vec;

//matrix
typedef struct CORE_API Mat
{
public:
	//set the row&col of matrix, allocate memory
	void Set(int row, int col);
	
	//free memory
	void Free();

	//Add a row vector to the matrix
	void AddRow(const Vec &feature); 
	
	//operator []: so you can use matrix[i][j] & matirx[i]
	Vec &operator[](int i)const { return matrix[i]; }

	//get col of matrix
	int cols () const {	return col; }

	//get row of matrix
	int rows () const { return row;	}
	
	Mat(int _row, int _col);

	Mat();

	virtual ~Mat();
	

private:	
	//row
	int row;
	//col
	int col;
	//data
	Vec *matrix;
	//flag
	bool isfree;
	//iterator
	int _iterator;

}Mat;



class WeakLearner;

/*
	simple threshold function h(x) on a feature x :
	h(x) = 1   if p.x < p.threshold
	h(x) = -1   othrewise
	where p is a parity to indicate the direction of the inequality
*/
class  CORE_API DecisionStump
{
	friend class WeakLearner;
	
public:
	DecisionStump();
	
	~DecisionStump();
	
	// Perform a round training
	void RoundTrain(const Mat &mat1, const Mat &mat2, double* _weights);
	
	// Classify which is a simple threshold function
	const int Classify(double *feature, int n_input)const
	{
		return feature[_d] > _threshold ? _sign : -_sign;
	}
	
private:
	// QuickSort Algorithm
	void QuickSort(double* values,int* indices,const int l,const int r);
	
private :
	// The data dimension
	int _d;
	// The threshold
	double _threshold;
	// The sign of the classifier (+1 or -1)
	int _sign;


};

//a set of simple threshold function called weaklearner
class  CORE_API WeakLearner
{
public:
	WeakLearner();
	
	~WeakLearner();

	// Allocate memory & intialise
	void Init(int num, double label1, double label2);
	
	// Free memory
	void Free();
	
	// Add a stump-based weaklearner to WeakLearner-Pool
	void AddStump(const DecisionStump& stump, double _alpha);
	
	// Classify a feature vector
	double Classify(double * feature, int n_input);
	
	// Save data to a file 
	void Save(FILE *f);

	// Load data from a file
	void Load(FILE *f);

private:
	DecisionStump * decisionstump;
	
	double *alpha;

	//current number of weaklearner
	int _iterator;
	//max number of weaklearner
	int maxnum;

	// + label
	double positive_label; 
	// - label
	double negative_label;
};

#endif //WEAKLEARNER_H
