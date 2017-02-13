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

 File: global.h                 
 Authors: Yao Wei  
 Date Created    : 2007-8-11
                                          
****************************************************************************/

#ifndef GLOBAL_H
#define GLOBAL_H

//****************************************************************************//
// Includes                                                                   //
//****************************************************************************//

// standard includes
#include <stdio.h>
#include <string.h>
#include <direct.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <ctype.h>

// OpenCV includes
#include <cv.h>
#include <highgui.h>
//#include <windows.h>

#ifndef min
template <class T> CORE_API inline T min(T x,T y) { return (x<y)?x:y; }
#endif


#ifdef __cplusplus
extern "C"{
#endif


#define M_PI		3.14159265359
#define W			48
#define H			48
#define space_or_null(c) (isspace(c)||c == '\n') 
#define number(c) (isdigit(c)||c=='.'||c=='-'||c=='e')
#define INFINITER 1e32

#ifndef CORE_API
#ifdef CORE_EXPORTS
#define CORE_API __declspec(dllexport)
#else
#define CORE_API
#endif //CORE_EXPORTS
#endif


CORE_API enum{ NEUTRAL=0, ANGER = 1, DISGUST, FEAR, HAPPY, SADNESS,	SURPRISE };


CORE_API int	getint(char *expression); //translate char to int 
CORE_API char*	getstr(double c);		  //translate int to char
CORE_API void ** mymalloc2d(size_t size,int m, int n);

	
#ifdef __cplusplus
}
#endif



#endif //GLOBAL_H