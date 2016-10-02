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

int lowThreshold=43;
int lowNegligeable=10;
int const max_lowThreshold = 100;
const char* window_name = "Edge Map";

int thresh = 40;
int max_thresh = 120;
Mat img_rgb,img_gray,img_bw,canny_output,drawing;
Mat _img;

vector<vector<Point> > contours;
vector<Vec4i> hierarchy;
vector<Point> approxShape;
vector<double> areas;
vector<int> grays;

int main(int argc, char *argv[]){
	img_rgb  = imread(argv[1]);
	blur( img_rgb, img_rgb, Size(3,3) );
	namedWindow( window_name, WINDOW_AUTOSIZE );
	Scalar meanColor = mean(img_rgb);
	lowThreshold = (meanColor[0]+meanColor[1]+meanColor[1])/4;		
	cv::createTrackbar( "Min Threshold:", window_name, &lowThreshold, max_lowThreshold, drawStuff );
	cv::createTrackbar( "Negligeable: Area", window_name, &lowNegligeable, max_lowThreshold, drawStuff );
	drawStuff(0,0);
	cv::waitKey(0);
}

void suppressInnerShapes(int i){
	int j = hierarchy[i][2];
	if (j<0)
		return;
	else{
		contours[j];
		drawContours(drawing, contours, i, Scalar(255,0,0), CV_FILLED);   // fill one gray per contour
		cv::imshow("Fill",drawing);
		cv::waitKey(0);
		drawContours(drawing, contours, i, Scalar(grays[i],grays[i],grays[i]), CV_FILLED);   // fill one gray per contour
		suppressInnerShapes(j);
	}
}

void drawStuff(int, void*){
	Canny( img_rgb, canny_output, lowThreshold, lowThreshold*1.5, 3 );
	cv::dilate(canny_output, canny_output, cv::Mat(), cv::Point(-1,-1));
	showCannyWindow();

	findContours( canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
	drawing = Mat::zeros( canny_output.size(), CV_8UC3 );

	for(size_t i = 0; i < contours.size(); i++){
		approxPolyDP(contours[i], approxShape, arcLength(Mat(contours[i]), true)*0.04, true);
		grays.push_back((i+1)*10%255);
		areas.push_back(contourArea(contours[i],false));
		drawContours(drawing, contours, i, Scalar(grays[i],grays[i],grays[i]), CV_FILLED);   // fill one gray per contour
	//	printf("contour %d couleur %d aire %f\n",i,(i+i)*10%255,areas[i]);
	}
	for(size_t i = 0; i< contours.size(); i++){
		int j = hierarchy[i][2];
		if(areas[j]<=(areas[i]*lowNegligeable/100)){
			suppressInnerShapes(i);	
		}
	}
	showContourWindow();
}

void showInputWindow(){
	cv::namedWindow("InputImage");
	cv::imshow("InputImage",img_rgb);
}

void showCannyWindow(){
	cv::namedWindow("Canny");
	cv::imshow("Canny",canny_output);
}
void showContourWindow(){
	cv::namedWindow("Fill");
	cv::imshow("Fill",drawing);
}
