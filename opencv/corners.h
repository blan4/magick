#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
namespace Corners
{
	int find(
		cv::Mat 					src, 
		unsigned 					thresh,
		std::vector<cv::Point2f>& 	corners);
	
	int find_corners(
		cv::Mat& 					contour, 
		unsigned 					thresh,
		std::vector<cv::Point2f>& 	_corners);
	
	cv::Point2f computeIntersect(
		cv::Vec4i 	a, 
		cv::Vec4i 	b);
	
	int aproxCorners(
		std::vector<cv::Point2f>& corners);

	int sortCorners(
		std::vector<cv::Point2f>& 	corners, 
		cv::Point2f 				center);	
}