/*
 * Feature_Extraction.h
 *
 *  Created on: Jun 17, 2011
 *      Author: puppy
 */

#ifndef FEATURE_EXTRACTION_H_
#define FEATURE_EXTRACTION_H_

#include <highgui.h>
#include <stdio.h>
#include <cv.h>
#include <iostream>
#include <string>
#include <fstream>
#include <cmath>
#define Pi 3.14159265
/**
D will be used in step_5,in order to calculate the convexity magnitude
of every pixel in the image
**/
#define D 60

using namespace std;
class Feature_Extraction {
	  double ***G;
	  double bw;//bandwidth
	  double gamma ; //aspect ratio
	  double psi;//phase shift
	  double lambda;//wavelength >=2
	  double sigma;
	  double sigma_x;
	  double sigma_y;
	  double sz;
	  double theta_max;
public:
    IplImage *IROI;
	Feature_Extraction(IplImage *src);
	double  G_Real(double x,double y,double theta);
	IplImage* step_4_1(IplImage *src);
	virtual ~Feature_Extraction();
};

#endif /* FEATURE_EXTRACTION_H_ */
