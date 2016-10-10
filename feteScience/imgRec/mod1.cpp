#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "variables.h"
#include "recContours.h"
#include "pathfinding.h"

using namespace cv;
using namespace std;

void onMouse(int event, int x, int y, int, void*);

int main(int argc, char *argv[]){
	srand (time(NULL));
	
	img_rgb  = imread(argv[1]); // charge l'image passée en paramètre (img_rgb)
	
	//blur( img_rgb, img_rgb, Size(3,3) ); // floute l'image
	namedWindow(window_name, WINDOW_NORMAL); // nouvelle fenêtre
	cv::setWindowProperty(window_name, WND_PROP_FULLSCREEN, WINDOW_FULLSCREEN);	
		
	cv::createTrackbar("Minimum de Canny :", window_name, &lowThreshold, max_lowThreshold, drawStuff ); // barre du pas pour Canny
	cv::createTrackbar("Taille de la bande de Canny :", window_name, &hystSize, max_hystSize, drawStuff );
	cv::createTrackbar("Reduction du bruit :", window_name, &lowNegligeable, max_Negligeable, drawStuff ); // barre du pourcentage d'aire négligeable pour nettoyage
	cv::createTrackbar("Approximation :", window_name, &approxEpsilonInt, 50, drawStuff );
	cv::createTrackbar("Filtre - 0=Image de depart, 1=Canny, 2=Contours :", window_name, &displayedPicture, 2, displayPicture); // image affichée
	
	drawStuff(0,0);
	
	cv::moveWindow(window_name,0,0);
	
	cv::setMouseCallback(window_name, onMouse, 0);	
	
	cv::waitKey(0);
}

void onMouse(int event, int x, int y, int, void*)
{
    if(event == EVENT_LBUTTONDOWN)
    {
    	clickPoint = Point(x,y);
    	selectedI = selectedContour(clickPoint);
    	drawStuff(0,0);
    }
    else if(event == EVENT_RBUTTONDOWN)
    {
    	selectedI = -1;
    	drawStuff(0,0);
    }
    else if(event == EVENT_LBUTTONDBLCLK)
    {
    	pathfinding(10);
    }
    else return;
}

