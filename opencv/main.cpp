#include "corners.h"
//g++ main.cpp corners.cpp `pkg-config opencv --cflags --libs`
int main(int argc, char const *argv[])
{
	if (argv[1] == NULL){
		return -1;
	}
	cv::Mat src;
	src = cv::imread( argv[1], 1 );
	std::vector<cv::Point2f> corners;
	Corners::find(src,60,corners);
	
	for (int i=0;i<corners.size(); ++i){
	    cv::circle( 
	    	src,
	        corners[i],
	        1,
	        cv::Scalar(0,0,255),
	        2,
	        0);
  	}
  	
  	cv::imshow( "source_window", src );
  	cv::waitKey(0);
	return 0;
}