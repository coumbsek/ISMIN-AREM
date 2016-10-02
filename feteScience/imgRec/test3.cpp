#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;

void drawStuff(int, void*);
void showInputWindow();
void showCannyWindow();
void showContourWindow();

int lowThreshold;
int const max_lowThreshold = 100;
const char* window_name = "Edge Map";

int thresh = 40;
int max_thresh = 120;
Mat img_rgb,img_gray,img_bw,canny_output,drawing;

int main(int argc, char *argv[]){
    img_rgb  = imread(argv[1]);
    blur( img_rgb, img_rgb, Size(3,3) );
    cvtColor(img_rgb,img_gray,CV_RGB2GRAY);
    namedWindow( window_name, WINDOW_AUTOSIZE );
    cv::createTrackbar( "Min Threshold:", window_name, &lowThreshold, max_lowThreshold, drawStuff );
    showInputWindow();

    drawStuff(0,0);
    cv::waitKey(0);
}

void drawStuff(int, void*){
	Mat thr;
	threshold(img_gray, thr, 200,255,THRESH_BINARY_INV);

	vector< vector <Point> > contours; // Vector for storing contour
	vector< Vec4i > hierarchy;
	findContours( thr, contours, hierarchy,CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );

	for( int i = 0; i< contours.size(); i=hierarchy[i][0] ) // iterate through each contour.
	{
		Rect r= boundingRect(contours[i]);
		if(hierarchy[i][2]<0) //Check if there is a child contour
		  rectangle(img_gray,Point(r.x-10,r.y-10), Point(r.x+r.width+10,r.y+r.height+10), Scalar(0,0,255),2,8,0); //Opened contour
		else
		  rectangle(img_gray,Point(r.x-10,r.y-10), Point(r.x+r.width+10,r.y+r.height+10), Scalar(0,255,0),2,8,0); //closed contour
	}
	showContourWindow();
}
void showInputWindow(){
    cv::namedWindow("InputImage");
    cv::imshow("InputImage",img_rgb);
}
void showContourWindow(){
	cv::namedWindow("Fill");
	cv::imshow("Fill",img_gray);
}
