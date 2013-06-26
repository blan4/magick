#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
namespace Corners
{
	std::vector<cv::Point2f> find(cv::Mat src, unsigned thresh);	
}