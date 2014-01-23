/*
 * comparison.cpp
 *
 *  Created on: Jun 19, 2011
 *      Author: puppy
 */

#include "comparison.h"

comparison::comparison(IplImage *src1,IplImage *src2){
	this->src1=src1;//template
	this->src2=src2;
}
double comparison::run_template_matching_comparison(int type){
	int height = src2->height - src1->height +1;
	int width = src2->width - src1->width +1;
	 /* create a new image, to store phase correlation result */
	IplImage *ftmp = cvCreateImage( cvSize(width,height ), IPL_DEPTH_32F, 1);
	/* get phase correlation of input images */
	cvMatchTemplate(src2,src1,ftmp,type);
	/* find the maximum value and its location */
	CvPoint minloc, maxloc;
	double  minval, maxval;
	cvMinMaxLoc(ftmp, &minval, &maxval, &minloc, &maxloc, 0 );		
return maxval;
}
comparison::~comparison() {
}
