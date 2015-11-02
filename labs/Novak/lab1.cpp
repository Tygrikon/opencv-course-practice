#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "opencv2/imgproc/imgproc.hpp"
#include "math.h"
#include "stdio.h"
#include "stdlib.h"

using namespace cv;
using namespace std;


int main( int argc, char** argv )
{
  Mat src ;
  Mat tmp_canny;
  Mat inverse_canny;
  Mat output;
  Mat dist;
  Mat integ;
  Mat tmp1;
  double r;
  char* source_window = "input image";
  char* dist_window = "output image";
	  /// Load image
  src = imread( argv[1], 1 );

  if( !src.data )
    { cout<<"Usage: ./Histogram_Demo <path_to_image>"<<endl;
      return -1;}

  tmp1=src.clone();
   // cvtColor( src, src, CV_BGR2GRAY );
  Canny(src,tmp_canny,20,150,3);
  inverse_canny=~tmp_canny ;

  distanceTransform(inverse_canny, dist, CV_DIST_L2, 3);
  //normalize(dist,dist,0.0,1.0,NORM_MINMAX);
  Mat bgr[3];
  Mat bgr_out[3];
  split(tmp1,bgr);

  for (int i=0; i<3; i++)
  {
	bgr_out[i]=bgr[i].clone();
  }


  int a,b,c,d;
  double n_okr;
  double gamma=0.5;
  int minx, miny, maxx, maxy;
  for (int k =0;k<3;k++)
  {
	  integral(bgr[k],integ);
	  for(int i=0;i<dist.rows;i++)
	  {
		  for(int j=0; j<dist.cols;j++)
		  {
			  float diam;
			  diam=dist.at<float>(i,j);
			  diam=(diam*gamma);

			  int rad=(diam/2) ;

			  if(rad%2==0)
			  {
				  rad=rad+1;
			  }


			  if(rad>0)
			  {
				  int max_x,max_y,min_x,min_y;
				  max_x=max(0,i-rad);
				  max_y=max(0,j-rad);
				  min_x=min(dist.rows-1,i+rad);
				  min_y=min(dist.cols-1,j+rad);

				  a=integ.at<int>(max_x,max_y);
				  b=integ.at<int>(max_x,min_y+1);
				  c=integ.at<int>(min_x+1,max_y);
				  d=integ.at<int>(min_x+1,min_y+1);

				  n_okr=(min_x-max_x+1)*(min_y-max_y+1);
				  double tmp5=((a+d-b-c)/n_okr );
				  bgr_out[k].at<unsigned char>(i,j)=(unsigned char)(tmp5);

			  }
			  else
			  {
				  bgr_out[k].at<unsigned char>(i,j)=bgr_out[k].at<unsigned char>(i,j);
			  }

		  }
	  }
  }

  merge(bgr_out,3,output);



   namedWindow( source_window, CV_WINDOW_AUTOSIZE );
   namedWindow( dist_window, CV_WINDOW_AUTOSIZE );
   imshow( source_window, src);
   imshow( dist_window, output);

	  /// Wait until user exits the program
  waitKey(0);

  return 0;

}







/*
int main( int argc, char** argv )
{
  Mat src, dst_canny,dst_blur,output;

  char* source_window = "Source image";
  char* equalized_window = "Equalized Image";
  char* canny_window = "canny image";
  char* blur_window = "blur Image";
  char* output_window = "output Image";
  /// Load image
  src = imread( argv[1], 1 );

  if( !src.data )
    { cout<<"Usage: ./Histogram_Demo <path_to_image>"<<endl;
      return -1;}
  output=src.clone();

  Canny(src,dst_canny,20,150,3);

  //cvtColor( src, src, CV_BGR2GRAY );

 

  /// Display results
  namedWindow( source_window, CV_WINDOW_AUTOSIZE );
  namedWindow( equalized_window, CV_WINDOW_AUTOSIZE );
  namedWindow( canny_window, CV_WINDOW_AUTOSIZE );
  namedWindow( blur_window, CV_WINDOW_AUTOSIZE );
  namedWindow( output_window, CV_WINDOW_AUTOSIZE );

  imshow( source_window, output );

  imshow( canny_window, dst_canny );


  /// Wait until user exits the program
  waitKey(0);

  return 0;
}

*/
