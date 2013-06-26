#include "corners.h"

namespace Corners
{
	std::vector<cv::Point2f> find(cv::Mat src, unsigned thresh){
  		cv::Mat src_gray;
  		cv::cvtColor( src, src_gray, CV_RGB2GRAY );
  		cv::GaussianBlur(src_gray,src_gray,cv::Size(3,3),0);


		std::vector<std::vector<cv::Point> > contours;
		std::vector<cv::Vec4i> lines;
		cv::Mat canny_output;

		cv::Canny( src_gray, canny_output, thresh, thresh*3, 3 );
		cv::findContours(
		    canny_output, 
		    contours, 
		    CV_RETR_EXTERNAL, 
		    CV_CHAIN_APPROX_SIMPLE);
		  
		std::vector<cv::Point> approx;
		Scalar color(0,0,255);
		
		for (int i = 0; i < contours.size(); i++){
		// Approximate contour with accuracy proportional
		// to the contour perimeter
		cv::approxPolyDP(
		    cv::Mat(contours[i]), 
		    approx, 
		    cv::arcLength(cv::Mat(contours[i]), true) * 0.02, 
		    true);
		    
		// Skip small or non-convex objects 
		if (std::fabs(cv::contourArea(contours[i])) < 200 || !cv::isContourConvex(approx))
		    continue;

		cv::Mat drawing = Mat::zeros( src.size(), CV_8UC3 );
		if (approx.size()==4){
			//cv::Rect r = cv::boundingRect(approx); // описанный квадрат
			//rectangle(drawing,r.tl(),r.br(),color,2,8,0); 
			drawContours(drawing,contours,i,Scalar(255,255,255));
			return find_corners(drawing,thresh);
		}
  		return NULL;
	}	




	std::vector<cv::Point2f> find_corners(Mat& contour, unsigned thresh){
	  std::vector<std::vector<cv::Point> > contours;
	  std::vector<cv::Vec4i> hierarchy;
	  std::vector<cv::Vec4i> lines;

	  cv::Mat gray;
	  cv::cvtColor( contour, gray, CV_RGB2GRAY );
	  cv::HoughLinesP(gray, lines, 1, CV_PI/180, thresh, 30, 10);   // Преобразованием Hough находим только прямые линии. получаем вектор lines (x1,y1,x2,y2)
	  
	  std::vector<cv::Point2f> corners;
	  for (int i = 0; i < lines.size(); i++){
	    for (int j = i+1; j < lines.size(); j++)    {
	        cv::Point2f pt = computeIntersect(lines[i], lines[j]);
	        if (pt.x >= 0 && pt.y >= 0)
	            corners.push_back(pt);
	    }
	  }
	  	  
	  cout << lines.size()<<" : "<<corners.size() << endl;
	  for (vector<Point2f>::const_iterator i = corners.begin();i != corners.end(); ++i){
	    cout << *i << endl;
	  }
	  
	  cout << "Counting" << endl;
	  aproxCorners(corners);
	  cout << "New" << endl;
	  
	  for (vector<Point2f>::const_iterator i = corners.begin();i != corners.end(); ++i){
	    cout << *i << endl;
	  }


	  cv::Point2f center(0,0);
	  for (int i = 0; i < corners.size(); i++)
	    center += corners[i];
	  center *= (1. / corners.size());


	  std::vector<cv::Point2f> quad_pts;
	  if ( sortCorners(corners, center) > -1){    
	    // Corners of the destination image
	    quad_pts.push_back(cv::Point2f(0, 0));
	    quad_pts.push_back(cv::Point2f(quad.cols, 0));
	    quad_pts.push_back(cv::Point2f(quad.cols, quad.rows));
	    quad_pts.push_back(cv::Point2f(0, quad.rows));
	  }
	  return quad_pts;
	}
}