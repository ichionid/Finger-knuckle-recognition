/*
 * comparison.h
 *
 *  Created on: Jun 19, 2011
 *      Author: Chionidis Ioannis
 */

#ifndef COMPARISON_H_
#define COMPARISON_H_
#include <iostream>
#include "cv.h"
#include "highgui.h"
using namespace std;
class comparison {
	IplImage *src1,*src2,*poc;
public:
	comparison(IplImage *src1,IplImage *src2);
	double run_template_matching_comparison(int type);
	virtual ~comparison();
};

#endif /* COMPARISON_H_ */
