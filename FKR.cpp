#include "ROI_extraction.h"
#include "Feature_Extraction.h"
#include "comparison.h"
#include <time.h>
#include <stdlib.h>
/**
RUN this:
./a.out 01ROI.jpg 02ROI.jpg 
**/
/**
* Src images size is :: 384x288
* crop to 220x110: taken from ROI Images from databases should be justyfied in a way TODO
**/

int main(int argc, char *argv[]) {		
	time_t start,end;
	time(&start);
   	ROI_extraction a(argv[1]);
 	a.apply_ROI();
 	a.step_3();
 	a.step_4();
 	a.step_5();
 	a.step_6();
 	Feature_Extraction f(a.IROI);
	IplImage *src1 = f.step_4_1(a.IROI);
	IplImage *img_ch3 = cvLoadImage(argv[2]);
	IplImage *img_ch1 = cvCreateImage(cvSize(img_ch3->width,img_ch3->height),IPL_DEPTH_8U, 1 );
	cvCvtColor(img_ch3, img_ch1, CV_RGB2GRAY);
	Feature_Extraction f2(img_ch1);
	IplImage *src2 = f2.step_4_1(img_ch1);//Gabor image
	comparison c(src1,src2);//src1 acts as a template
	time(&end);
	cout<<"Similarity ration:"<<c.run_template_matching_comparison(5)<<endl;
	cout<<"Computation Time:"<<difftime (end,start)<< " seconds"<<endl; 
 	return 0;
}