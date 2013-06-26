#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;

Mat src;
Mat src_gray;
int thresh = 50;
int blr = 2;
int ratio = 3;
int max_thresh = 100;
RNG rng(12345);
const char* source_window = "Source";



void aproxCorners(std::vector<Point2f>&);//находим классы близких точек и объединяем их.
void find_corners(Mat&);
void thresh_callback_fix(int,void*);
Point2f computeIntersect(Vec4i, Vec4i);//поиск пересечений линий
int sortCorners(vector<Point2f>&, Point2f);// определение порядка углов четырехугольника


int main(int argc, char const *argv[])
{
  /// Load source image and convert it to gray
  src = imread( argv[1], 1 );


  /// Convert image to gray and blur it
  cvtColor( src, src_gray, CV_RGB2GRAY );
  GaussianBlur(src_gray,src_gray,Size(3,3),0);

  namedWindow( source_window, WINDOW_AUTOSIZE );
  imshow( source_window, src );

  createTrackbar( " Canny thresh:", "Source", &thresh, max_thresh, thresh_callback_fix );
  thresh_callback_fix( 0, 0 );

  waitKey(0);
  return(0);
}

void thresh_callback_fix(int,void*){
  vector<vector<Point> > contours;
  vector<Vec4i> lines;
  Mat canny_output;
  Mat drawing = Mat::zeros( src.size(), CV_8UC3 );

  Canny( src_gray, canny_output, thresh, thresh*ratio, 3 );
  imshow( "canny", canny_output);

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
        true
    );
    // Skip small or non-convex objects 
    if (std::fabs(cv::contourArea(contours[i])) < 200 || !cv::isContourConvex(approx))
      continue;

    if (approx.size()==4){
      //cv::Rect r = cv::boundingRect(approx); // описанный квадрат
      //rectangle(drawing,r.tl(),r.br(),color,2,8,0); 
      cout << "RECT"<<endl;
      drawContours(drawing,contours,i,Scalar(255,255,255));
      find_corners(drawing);
    }
  }
  imshow("dr",drawing);
}
void find_corners(Mat& contour){
  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;

  vector<Vec4i> lines;
  /// Detect edges using canny
  Mat gray;
  cvtColor( contour, gray, CV_RGB2GRAY );
  HoughLinesP(gray, lines, 1, CV_PI/180, thresh, 30, 10);   // Преобразованием Hough находим только прямые линии. получаем вектор lines (x1,y1,x2,y2)
  
  vector<Point2f> corners;
  for (int i = 0; i < lines.size(); i++){
    for (int j = i+1; j < lines.size(); j++)    {
        Point2f pt = computeIntersect(lines[i], lines[j]);
        if (pt.x >= 0 && pt.y >= 0)
            corners.push_back(pt);
    }
  }
  
  //для красоты
//  for (int i = 0; i < lines.size(); i++){
//    Vec4i v = lines[i];
//    lines[i][0] = 0;
//    lines[i][1] = ((float)v[1] - v[3]) / (v[0] - v[2]) * -v[0] + v[1]; 
//    lines[i][2] = src.cols; 
//    lines[i][3] = ((float)v[1] - v[3]) / (v[0] - v[2]) * (src.cols - v[2]) + v[3];
//  }
//  Scalar color;
//  for (size_t i=0; i< lines.size(); ++i){
//    color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
//    line( contour, 
//          Point(lines[i][0], lines[i][1]),
//          Point(lines[i][2], lines[i][3]), 
//         color, 5, 1 );
//  }
  
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

  for (size_t i=0;i<corners.size(); ++i){
    circle( contour,
            corners[i],
            1,
            Scalar(0,0,255),
            2,
            0);
  }

  cv::Point2f center(0,0);
  for (size_t i = 0; i < corners.size(); i++)
    center += corners[i];
  center *= (1. / corners.size());
  
  circle( contour,
            center,
            1,
            Scalar(255,0,0),
            2,
            0);

  cv::Mat quad = cv::Mat::zeros(300, 300, CV_8UC3);

  if ( sortCorners(corners, center) > -1){
    cout << "Wrap" <<endl;
    for (size_t i = 0; i < corners.size(); i++){
      line( contour, 
            corners[i % corners.size()],
            corners[(i+1) % corners.size()], 
            Scalar(255,0,0), 3, 8 );
    }
    
    
      // Corners of the destination image
    vector<cv::Point2f> quad_pts;
    quad_pts.push_back(cv::Point2f(0, 0));
    quad_pts.push_back(cv::Point2f(quad.cols, 0));
    quad_pts.push_back(cv::Point2f(quad.cols, quad.rows));
    quad_pts.push_back(cv::Point2f(0, quad.rows));

      // Get transformation matrix
    cv::Mat transmtx = cv::getPerspectiveTransform(corners, quad_pts);

      // Apply perspective transformation
    cv::warpPerspective(src, quad, transmtx, quad.size());

  }
  imshow("quadrilateral", quad);
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


void aproxCorners(vector<Point2f>& corners){
  vector<Point2f> apr;
  Point2f average,current;
  unsigned unchecked = corners.size();
  vector<bool> flags(unchecked,false);
  Rect r(-16,-16,32,32);//окрестность
  size_t i,j,
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
    ///////!!!!!!!!!!!!!!!! FIX MEEEEEEEEEEEEEEE
    for(size_t iter = i; iter<len; ++iter){
      if ( flags[iter] == false ){//т.е не проверяли точку
        cout << current << " - "<< corners[iter] << " = " <<(current - corners[iter]);
        if ((current - corners[iter]).inside(r)){
          average = (average + corners[iter]) * 0.5;
          flags[iter] = true;
          unchecked--;
          cout << " inside";
        }
        cout<<endl;
      }
    }
    apr.push_back(average);
  }
  corners.clear();
  corners = apr;
}


int sortCorners(vector<Point2f>& corners, Point2f center){
    if (corners.size()<4){
      cout << "Unrectangled" <<endl;
      return -1;
    }


    vector<Point2f> top, bot;

    for (int i = 0; i < corners.size(); i++)
    {
        if (corners[i].y < center.y)
            top.push_back(corners[i]);
        else
            bot.push_back(corners[i]);
    }

    Point2f tl = top[0].x > top[1].x ? top[1] : top[0];
    Point2f tr = top[0].x > top[1].x ? top[0] : top[1];
    Point2f bl = bot[0].x > bot[1].x ? bot[1] : bot[0];
    Point2f br = bot[0].x > bot[1].x ? bot[0] : bot[1];

    corners.clear();
    corners.push_back(tl);
    corners.push_back(tr);
    corners.push_back(br);
    corners.push_back(bl);

    return 0;
}
