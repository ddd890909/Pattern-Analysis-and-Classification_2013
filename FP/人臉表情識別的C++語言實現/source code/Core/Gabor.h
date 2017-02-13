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

 File: gabor.h                                                                         
 Authors: Yao Wei  
 Date Created    : 2007-8-11
                                          
****************************************************************************/


/************************************************************************/
/* Notice:																*/
/* Yao Wei revised some BUGs of the GWTLib                              */
/************************************************************************/


#ifndef GABOR_H
#define GABOR_H

#include "global.h"


/*
GWTLib(Gabor Wavelets Transform for Face Recognition) LICENSE
Copyright (c) 2003-2005 Jackie, Jianke Zhu.
http://www.cse.cuhk.edu.hk/~jkzhu

The software is partly based on the following libraries:
  - The Intel(tm) OpenCV Library

$Revision: 1.1 $ 
$Date: 2005/12/02 $ 
*/


class CORE_API Gabor  
{
public:
	// interface: extract gabor features of image
	double* Extract_Features(const IplImage* img, int& nsize);

	// write data in bin mode  
	int WriteData(double* v, int length, FILE* fh);

	// write data in text mode
	int SaveData(double* v, int length, FILE* fh);

	// free memory
	void Free();
	
	
	Gabor();

	virtual ~Gabor();	

private:
	//Initilize, pre-load gabor filter bank FFT
	void Init();
	
	//Save pre-computed fft of gabor filter
	int Gabor_Filter(char* base_fld);

	//Gabor Wavelet Transform filter generator
	int Gabor_Kernel(int scale, int orientation, int mask_size,
		double kmax, double sigma, char* filename);
	
	//Reads double array from file binary mode
	double* GetData(FILE* fh, int &length);
	
	//perform gabor wavelet transform on the input image
	int Gabor_Extraction(IplImage* img,double* object);
	
	//Release the gabor filter bank
	void UnloadGaborFFT(CvMat** mGabor);
	
	// Load gabor filter bank FFT
	int LoadGaborFFT(char* fldname);
	
	//Writes cvMat into file binary mode
	int WriteMat(CvMat* m, FILE *fh, BOOL isVector );
	
	//Get cvMat from file binary mode
	CvMat* GetMat( FILE *fh, BOOL isVector );
	
	//Calculate the zero mean unit length of a vector length of vSize
	void ZeroMeanUnitLength( double* v, int vSize);
	
	//Calculate the mean value of a vector at length of vSize
	double MeanVector(double* v, int vSize);
	
	//Save the CvMat to an bmp image
	void Save_Mat_Image(CvMat* mat, char* name);
	
private:
	//gabor fft (real part)
	CvMat** mGabor2;
	
	//gabor fft (imaginary part)
	CvMat** mGabor;

};


#endif  //GABOR_H