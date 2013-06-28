#include "corners.h"

namespace Corners
{
	int find(cv::Mat src, unsigned thresh,std::vector<cv::Point2f>& corners){
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
		cv::Scalar color(0,0,255);
		
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

			cv::Mat drawing = cv::Mat::zeros( src.size(), CV_8UC3 );
			if (approx.size()==4){
				//cv::Rect r = cv::boundingRect(approx); // описанный квадрат
				//rectangle(drawing,r.tl(),r.br(),color,2,8,0); 
				cv::drawContours(drawing,contours,i,cv::Scalar(255,255,255));
				//FIX MEE!!!!!!!!!!!!!
				return find_corners(drawing, thresh, corners);
			}
		}
  		return -1;
	}	




	int find_corners(cv::Mat& contour, unsigned thresh,std::vector<cv::Point2f>& _corners){
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
	  	  
	  std::cout << lines.size()<<" : "<<corners.size() << std::endl;
	  for (std::vector<cv::Point2f>::const_iterator i = corners.begin();i != corners.end(); ++i){
	    std::cout << *i << std::endl;
	  }
	  
	  std::cout << "Counting" << std::endl;
	  aproxCorners(corners);
	  std::cout << "New" << std::endl;
	  
	  for (std::vector<cv::Point2f>::const_iterator i = corners.begin();i != corners.end(); ++i){
	    std::cout << *i << std::endl;
	  }


	  cv::Point2f center(0,0);
	  for (int i = 0; i < corners.size(); i++)
	    center += corners[i];
	  center *= (1. / corners.size());

	  _corners.clear();
	  if ( sortCorners(corners, center) > -1){    
	  	_corners = corners;	
	  }else{
	  	return -1;
	  }
	  return 0;
	}



	cv::Point2f computeIntersect(cv::Vec4i a, cv::Vec4i b){  
	  int x1 = a[0], y1 = a[1], x2 = a[2], y2 = a[3];  
	  int x3 = b[0], y3 = b[1], x4 = b[2], y4 = b[3];  

	  if (float d = ((float)(x1-x2) * (y3-y4)) - ((y1-y2) * (x3-x4))){  
	    cv::Point2f pt;  
	    pt.x = ((x1*y2 - y1*x2) * (x3-x4) - (x1-x2) * (x3*y4 - y3*x4)) / d;  
	    pt.y = ((x1*y2 - y1*x2) * (y3-y4) - (y1-y2) * (x3*y4 - y3*x4)) / d; 

	    float a1 = y1 - y2;
	    float a2 = y3 - y4;
	    float b1 = x1 - x2;
	    float b2 = x3 - x4;
	    
	    float tg = (a1*b2 - a2*b1) / (a1*a2 + b1*b2);
	    if (fabs(tg) < 1.0)
	      return cv::Point2f(-1, -1);

	    return pt;  
	  }  
	  return cv::Point2f(-1, -1);  
	}  


	int aproxCorners(std::vector<cv::Point2f>& corners){
	  std::vector<cv::Point2f> apr;
	  cv::Point2f average,current;
	  unsigned unchecked = corners.size();
	  std::vector<bool> flags(unchecked,false);
	  cv::Rect r(-16,-16,32,32);//окрестность
	  int i,j,
	    len=flags.size();
	  while (unchecked){
	    j = 0;
	    i = -1;
	    while (j < len){ //ищем непроверенную точку
	      if (flags[j] == false){
	        i = j;
	        break;
	      }
	      j++;
	    }
	    if (i == -1)
	      break;

	    flags[i] = true;
	    unchecked--;
	    current = average = corners[i];
	    for(int iter = i; iter<len; ++iter){
	      if ( flags[iter] == false ){//т.е не проверяли точку
	        std::cout << current << " - "<< corners[iter] << " = " <<(current - corners[iter]);
	        if ((current - corners[iter]).inside(r)){
	          average = (average + corners[iter]) * 0.5;
	          flags[iter] = true;
	          unchecked--;
	          std::cout << " inside";
	        }
	        std::cout<<std::endl;
	      }
	    }
	    apr.push_back(average);
	  }
	  corners.clear();
	  corners = apr;
	  return 0;
	}


	int sortCorners(std::vector<cv::Point2f>& corners, cv::Point2f center){
	    if (corners.size()<4){
	      std::cout << "Unrectangled" << std::endl;
	      return -1;
	    }

	    std::vector<cv::Point2f> top, bot;

	    for (int i = 0; i < corners.size(); i++)
	    {
	        if (corners[i].y < center.y)
	            top.push_back(corners[i]);
	        else
	            bot.push_back(corners[i]);
	    }

	    cv::Point2f tl = top[0].x > top[1].x ? top[1] : top[0];
	    cv::Point2f tr = top[0].x > top[1].x ? top[0] : top[1];
	    cv::Point2f bl = bot[0].x > bot[1].x ? bot[1] : bot[0];
	    cv::Point2f br = bot[0].x > bot[1].x ? bot[0] : bot[1];

	    corners.clear();
	    corners.push_back(tl);
	    corners.push_back(tr);
	    corners.push_back(br);
	    corners.push_back(bl);

	    return 0;
	}
}