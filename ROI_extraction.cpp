#include "ROI_extraction.h"
/**
1.apply_ROI()
2.step_3()
3.step_4()
4.step_5()
5.step_6()
**/

/**
Step_1 and step_2 implemented in apply_ROI()
Functonallity: determine X-axis: y0 AND Is which is the
	      partially empirically cropped initial image.
**/
void ROI_extraction::apply_ROI(){
	frame= cvLoadImage( imageName, 0 );
	imageWhite = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);				  
	edgesImage = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);
	cvThreshold(frame, imageWhite, 100, 255, CV_THRESH_BINARY);
	cvCanny(imageWhite, edgesImage, 120, 120*3, 3);
	CvMemStorage* storage = cvCreateMemStorage(0);
	CvSeq* overheadLines = cvHoughLines2(edgesImage, storage, CV_HOUGH_PROBABILISTIC, 1,  CV_PI/180, 20, 20, 30);
	IplImage* imageLines // after line detection
	    = cvCreateImage(cvGetSize(edgesImage), IPL_DEPTH_8U, 1);
	cvSetZero(imageLines);
	/**cropY_x will eventually have the upper and lower limit to crop**/
	int cropY_high=frame->height;
	y0=0;
	for (int i = 0; i < overheadLines->total; i++) {
	  CvPoint* line = (CvPoint*)cvGetSeqElem(overheadLines, i);
	  cvLine(imageLines, line[0], line[1], CV_RGB(255, 255, 255));
	  if((line[0].y<cropY_high)&&(line[0].y>50))cropY_high=line[0].y;
	  if((line[1].y<cropY_high)&&(line[1].y>50))cropY_high=line[1].y;
	  if(line[0].y>y0)y0=line[0].y;
	  if(line[1].y>y0)y0=line[1].y;
	}
	Is = crop_image(y0,cropY_high,frame);
}

/**
Step_3 :
  Functionality creates Ie, which is an edge representation of Is
**/
void ROI_extraction::step_3(){
	Ie = cvCreateImage(cvGetSize(Is),
				  Is->depth,
				  Is->nChannels);
	IplImage *temp_imageWhite = cvCreateImage(cvGetSize(Is), IPL_DEPTH_8U, 1);
	cvThreshold(Is, temp_imageWhite, 220, 255, CV_THRESH_BINARY);
	cvCanny(temp_imageWhite, Ie, 240, 240*3, 3);
}
/**
Step_4 :
  Convex Direction Coding
**/

void ROI_extraction::step_4(){

  Icd = new int*[Ie->height];
  for(int i=0; i<Ie->height; ++i)
    Icd[i] = new int[Ie->width];

  /**
  * Rows: i
  * Columns : j
  **/
  uchar *data_Ie  = ( uchar* )Ie->imageData;
  int   ymid = Ie->height/2;
  bool noteEntered = true;
  for( int i = 0 ; i < Ie->height ; i++ ) {
    for( int j = 0 ; j < Ie->width ; j++ ) {
    noteEntered = true;

      if((int)data_Ie[i*Ie->widthStep+j]==0){
	  Icd[i][j]=0;noteEntered = false;

      }else if(((int)data_Ie[(i+1)*Ie->widthStep+(j-1)]>0)
		  &&((int)data_Ie[(i+1)*Ie->widthStep+(j+1)]>0)){
	  Icd[i][j]=0;noteEntered = false;

      }else if((((int)data_Ie[(i+1)*Ie->widthStep+(j-1)]>0)
		      && i<=ymid)
	      ||(((int)data_Ie[(i+1)*Ie->widthStep+(j+1)]>0)
		      && i>ymid)){
    	  Icd[i][j]=1;noteEntered = false;

     }else if((((int)data_Ie[(i+1)*Ie->widthStep+(j+1)]>0)
		      && i<=ymid)
	      ||(((int)data_Ie[(i+1)*Ie->widthStep+(j-1)]>0)
		      && i>ymid)){
    	  Icd[i][j]=-1;noteEntered = false;

     }else{
	  Icd[i][j]=0;
     }
    }
  }
   Negatives = cvCreateImage(cvGetSize(Is), IPL_DEPTH_8U, 1);
   Positives = cvCreateImage(cvGetSize(Is), IPL_DEPTH_8U, 1);
   uchar *data_P  = ( uchar* )Positives->imageData;
   uchar *data_N  = ( uchar* )Negatives->imageData;
}
int ROI_extraction::calc_conMag(int y,int x){
int res=0;
/**
We will not search in Ie->width-D and further. Since
Y-axis will be located near that center, no problem derives
by this assumption
**/
  if(!((Ie->width - y )<= D)){
    for( int i = 0 ; i < Ie->height ; i++ ) {
      for( int j = x ; j < x+D; j++ ) {
	res+=Icd[i][j];
      }
    }
  }
  return res;
}
void ROI_extraction::step_5(){

  uchar *data  = ( uchar* )Ie->imageData;
 /**Array allocation**/
 int **conMag;
 conMag = new int*[Ie->height];
  for(int i=0; i<Ie->height; ++i)
    conMag[i] = new int[Ie->width];
  /**DEBUG**/
 for( int i = 0 ; i < Ie->height ; i++ ) {
    for( int j = 0 ; j < Ie->width ; j++ ) {
       conMag[i][j]=calc_conMag(i,j);
    }
  }
  x0=0;			//initialize y axis at point 0
  int min = conMag[0][0];
 for( int i = 0 ; i < Ie->height ; i++ ) {
    for( int j = 0 ; j < (Ie->width-(D-1)); j++ ) {
       if(conMag[i][j]<min){x0=j;min=conMag[i][j];}
    }
  }
}
void ROI_extraction::step_6(){
/**
Produce fixed size image according to the coordination system*
x0+CV_RECT_ARG1 is the x0 of the starting image
*/

    cvSetImageROI(frame, cvRect(x0+CV_RECT_ARG1-110, y0-110, 220,110));//From Paper
    IROI = cvCreateImage(cvGetSize(frame),
				  frame->depth,
				  frame->nChannels);
    cvCopy(frame, IROI, NULL);
    cvResetImageROI(frame);
}


void ROI_extraction::print_image_data(IplImage *src){
  /**
  * Rows: i
  * Columns : j
  **/
  uchar *data = ( uchar* )src->imageData;
  for( int i = 0 ; i < src->height ; i++ ) {
    for( int j = 0 ; j < src->width ; j++ ) {
    }
  }
}


int ROI_extraction::get_lowest_X(IplImage *src){
  int h=0;//ptr that will eventually return the last Horizontal line;
  int count=0;	  //Rule/Assumption:there should be at least 3 points in a row to be considered as line
  uchar *data = ( uchar* )src->imageData;
  for( int i = 0 ; i < src->height ; i++ ) {
    count=0;/**Reset for every Row**/
    for( int j = 0 ; j < src->width ; j++ ) {
	if((int)data[i*src->widthStep+j]>0){
	  count++;
	  if(count>MIN_LINE_POINTS)
	  h=i;
	}
      }
   }
  return h;
}
int ROI_extraction::get_highest_X(IplImage *src){
  int count=0;	  //Rule/Assumption:there should be at least 3 points in a row to be considered as line
  uchar *data = ( uchar* )src->imageData;
  for( int i = 55 ; i < src->height ; i++ ) {
    for( int j = 0 ; j < src->width ; j++ ) {
	if((int)data[i*src->widthStep+j]>0){
	  count++;
	  if(count>MIN_LINE_POINTS)
	  {
	    return i;//Return the first Horizontal line
	  }
	}
    }
   }
   return -1; /**Image consists of no lines at all**/
}
/**Crop Image**/
IplImage* ROI_extraction::crop_image(int rowLow,int rowHigh,IplImage *src){
    /** sets the Region of Interest
      Note that the rectangle area has to be __INSIDE__ the image
      cvRect(x,y,width,height). x and y refer to top left point of rectangle
    **/
    cvSetImageROI(src, cvRect(CV_RECT_ARG1, rowHigh+10, src->width-100,rowLow-rowHigh-10));//From Paper
    IplImage *img2 = cvCreateImage(cvGetSize(src),
				  src->depth,
				  src->nChannels);
    cvCopy(src, img2, NULL);
    cvResetImageROI(src);
    return img2;
}
ROI_extraction::~ROI_extraction(){
  ////cout<<"Cleaning up our --Garbage--"<<endl;
  for(int i = 0; i < Ie->height; ++i)
    delete [] Icd[i];
  delete [] Icd;
  cvReleaseImage(&Is);
  cvReleaseImage(&Ie);
  cvReleaseImage(&frame);
  cvReleaseImage(&imageWhite);
  cvReleaseImage(&edgesImage);
  cvReleaseImage(&Negatives);
  cvReleaseImage(&IROI);
}

void ROI_extraction::create_panel(){
      cvShowImage("Frame", frame);
      cvShowImage("Is", Is);
      cvShowImage("Y-Axis", Ie);
 //     cvShowImage("Negatives", Negatives);
//      cvShowImage("Positives", Positives);
      cvShowImage("IROI", IROI);
      cvWaitKey(0);//waits forever
}
ROI_extraction::ROI_extraction(char* name){
  imageName=name;
}
