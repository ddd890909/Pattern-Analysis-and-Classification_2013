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

#ifndef TRAINER_H
#define TRAINER_H

#include "Gabor.h"
#include "AdaBoost.h"

class CORE_API Trainer  
{
public:
	//free memory
	void Free();	
				
	//train all the images in the path
	void TrainModel(char *path);
	
	Trainer();

	virtual ~Trainer();

private:
	// run AdaBoost-Train to get model
	void AdaBoostTrain(int max_iter, char *trainfile, char *modelfile);
	
	// extract the gabor features of image in the path
	// and save the features to the file
	void Extract_All_Features(char *pathname, char *filename);
	
	// extract the gabor features of image in the path
	// and save the features to the file stream
	void Extract_All_Features(char *pathname, FILE *f);
		
private:
	 Boosting boost;
	 Gabor gabor;
	 int len;		//size of gabor feature

	
};

#endif // TRAINER_H
