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

 File: classifier.h                                                                         
 Authors: Yao Wei  
 Date Created    : 2007-8-11
                                          
****************************************************************************/


#ifndef CLASSIFIER_H
#define CLASSIFIER_H


#include "Gabor.h"
#include "AdaBoost.h"
#include <map>

class CORE_API Classifier  
{
public:
	//detect faces, recognize the expression of image
	char** DoRecognition(IplImage *image);
	
	//detect faces, recognize the expression of video
	char* DoRecognition2(IplImage *image);
	
	//load model
	void LoadModel(const char* modelname);

	//free memory
	void Free();

	// get the status of whether the model is loaded or not
	int IsLoad() const {    return isload;  }

	Classifier();

	virtual ~Classifier();


private:
	/* variables used in face detector */
	 char* cascade_name;
	 CvHaarClassifierCascade* cascade;
     CvMemStorage* storage; 
     IplImage *temp;			//image which used in pyramid
     CvSeq* faces;
     
	 CvFont font;
     
	 int isload;				//whether the train model is loaded or not? 
     
	 
	 Gabor gabor;
	 Boosting boost;


	 class CORE_API Queen
	 {
	 public:
		 Queen():rear(0), prestate(0)
		 {
			 memset(data, 0, sizeof(data));
			 mymap[0] = MAX_NUM;
		 }
		 
		 ~Queen()
		 {
		 }
		 
		 // Add a new state to the queen
		 void Add(int elem)
		 {
			 //delete the front elem of the queen
			 it = mymap.find(data[rear]);
			 it->second --;
			 
			 //add the new elem to the rear of the queen
			 data[rear] = elem;
			 
			 it = mymap.find(elem);
			 if(it == mymap.end())//find fail
				 mymap.insert(std::make_pair(elem,1));//i.e. mymap[elem] = 1;
			 else                //find succeed 
				 it->second++;

			 //the rear pointer forward 
			 rear = (rear == MAX_NUM-1) ? 0 : rear+1;
		 }
		 
		 //get the elem that contains maximum elem
		 int Get()
		 {
			int maxcount = 0, elem;
			for(i = mymap.begin(); i != mymap.end(); i++) 
			{
				if(i->second > maxcount)
				{
					maxcount = i->second;
					elem = i->first;
				}
			}
			
			return prestate = ((maxcount > MAX_NUM/2) ? elem : prestate);
		 }
	
	 private:
		 enum{MAX_NUM = 6};
		 //store state
		 int data[MAX_NUM];
		 //rear pointer of the queen
		 int rear;
		 //pre-state
		 int prestate;
		 //map
		 std::map<int, int> mymap;
		 //iterator
		 std::map<int, int>::iterator it;
		 //also iterator
		 std::map<int, int>::const_iterator i;
	 };



private:
	// get most central face
	int GetCentralFace(IplImage *image, CvSeq* faces, int scale);

	//get the category of face image 
	int Expression(IplImage *faceimage);

	//load the face-detector model
	CvHaarClassifierCascade* LoadDetector( const char* cascade_path );

	// Initilise
	void Init();
	
};


#endif // CLASSIFIER_H