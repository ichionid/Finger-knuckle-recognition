/*
 * ROI_extraction.h
 *
 *  Created on: Jun 16, 2011
 *      Author: puppy
 */

#ifndef ROI_EXTRACTION_H_
#define ROI_EXTRACTION_H_

#include <highgui.h>
#include <stdio.h>
#include <cv.h>
#include <iostream>
#include <string>
#include <fstream>
#include <cmath>
#define Pi 3.14159265
#define MIN_LINE_POINTS 3
#define CV_RECT_ARG1 140

/**
D will be used in step_5,in order to calculate the convexity magnitude
of every pixel in the image
**/
#define D 60

using namespace std;
class ROI_extraction{
  char* imageName;
  IplImage *Is ;
  IplImage *Ie ;
  IplImage *frame;
  IplImage *imageWhite;
  IplImage *edgesImage;
  IplImage *Negatives;//Debugging
  IplImage *Positives;//Debugging



  int **Icd ;
  int x0;//Will be our Y-Axis, initiated at step_5()
  int y0;//X-axis generated by get_lowest_x();

  public:
  IplImage *IROI;

  ROI_extraction(char* name);
  void apply_ROI();/** In order to Apply ROI for every Image**/
  void print_image_data(IplImage *src);
  int get_lowest_X(IplImage *src);
  IplImage* crop_image(int rowLow,int rowHigh,IplImage *src);
  int get_highest_X(IplImage *src);
  void step_3();
  void step_4();
  void step_5();
  void step_6();
//  void step_4_1(IplImage *src);//input *IROI;
  int  calc_conMag(int i,int j);
  void create_panel();
  void cvShowManyImages(char* title, int nArgs, IplImage *Is0,IplImage *Is1,IplImage *Is2,IplImage *Is3,IplImage *Is4,IplImage *Is5,IplImage *Is6);
  ~ROI_extraction();
};
#endif /* INC_HEADER_H */

