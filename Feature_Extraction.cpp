/*
 * Feature_Extraction.cpp
 *
 *  Created on: Jun 17, 2011
 *      Author: Chionidis Ioannis
 */

#include "Feature_Extraction.h"

Feature_Extraction::Feature_Extraction(IplImage *src) {
	  	IROI = src;  theta_max=6;//maximumum value of theta

}
double Feature_Extraction::G_Real(double x,double y,double theta){
	  bw=1;//bandwidth
	  gamma = 0.85;//aspect ratio
	  psi = 108*Pi/180;//phase shift
	  lambda = 16;//wavelength >=2
	  sigma = lambda/Pi*sqrt(2*log(2))*((pow(2,bw)+1)/(pow(2,bw)+1));
	  sigma_x = sigma;
	  sigma_y = sigma/gamma;
	  sz = 10;
	  theta_max=6;
	  double x_theta = (x*cos(theta*Pi/theta_max))+(y*sin(theta*Pi/theta_max));
	  double y_theta = -(x*sin(theta*Pi/theta_max))+(y*cos(theta*Pi/theta_max));
	  double z = (exp(-0.5*((pow(x_theta,2)/pow(sigma_x,2))+(pow(y_theta,2)/pow(sigma_y,2))))*cos(2*Pi/lambda*x_theta+psi));
	  return z;
}
IplImage * Feature_Extraction::step_4_1(IplImage *src){

	  IplImage *dst_img[(int)theta_max];
	  IplImage *res_img=cvCreateImage( cvSize( IROI->width, IROI->height ), IROI->depth, IROI->nChannels );
	  uchar *res_data_result = ( uchar* )res_img->imageData;
	  for(int i = 0;i<theta_max;i++ ){
	    dst_img[i]=cvCreateImage( cvSize( IROI->width, IROI->height ), IROI->depth, IROI->nChannels );
	  }
	  CvMat stub;
	  CvMat *filter_mat[(int)theta_max];
	  for (int i = 0; i<theta_max;i++)
	    filter_mat[i] = cvCreateMat(IROI->height,IROI->width,CV_32FC1);
	  for(int k = 0;k < theta_max; k++)
	  {
	    for(int i = 0;i < IROI->height; i++)
	    {
	      for(int j = 0;j < IROI->width; j++)
	      {
	    	  CV_MAT_ELEM(*filter_mat[k], float, i, j)=(float)G_Real(i,j,k);//Generate Filter Kernel
	      }
	    }
	    cvFilter2D(IROI,dst_img[k],filter_mat[k],cvPoint(0,0));
	  }
	 uchar *data_result[(int)theta_max];
	  for(int k=0;k<theta_max;k++)
	  {
	    data_result[k]  = ( uchar* )dst_img[k]->imageData;
	  }

	    for(int i = 0;i < IROI->height; i++)
	    {
	      for(int j = 0;j < IROI->width; j++)
	      {
	    	  for(int k = 0;k < theta_max; k++)
	    	  {
	    		  if(k==0){

	    			  res_data_result[i*(res_img->widthStep)+j] = abs(data_result[k][i*(dst_img[k]->widthStep)+j]);

	    		  }else{
	    			  if(res_data_result[i*(res_img->widthStep)+j]<=abs(data_result[k][i*(dst_img[k]->widthStep)+j]))
	    			  {
	    				  res_data_result[i*(res_img->widthStep)+j]=abs(data_result[k][i*(dst_img[k]->widthStep)+j]);
	    			  }
	    		  }
	    	  }
	      }
	    }	  
return res_img;
}
Feature_Extraction::~Feature_Extraction() {
;
}
