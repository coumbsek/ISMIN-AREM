#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "variablesExt.h"
#include "recContours.h"

using namespace cv;
using namespace std;

void drawStuff(int, void*){ // fonction appelée au début et à chaque changement des trackbar
	
	img_rgb.copyTo(rgb_copy);
	
	Canny( img_rgb, canny_output, lowThreshold, lowThreshold+hystSize, apertureSize, L2Gradient); // filtre de Canny dans canny-output
	cv::dilate(canny_output, canny_output, cv::Mat(), cv::Point(-1,-1)); // dilate chaque pixel dans une matrice 3*3

	findContours( canny_output, contours, hierarchy, CV_RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0) ); // -> contours
	
	for(int i = 0; i<contours.size(); i++){ // pour chaque contour
		int j = hierarchy[i][2]; // on prend son premier fils
		if (j>=0){ // s'il existe
			do {
				double areaI = realContourArea(i);
				double areaJ = realContourArea(j);
				if(areaJ<=(areaI*lowNegligeable/1000)){ // si l'aire de j est bien plus petite que celle de i...
					
					//cout << "contour " << j << "(" << areaJ << ")" << " detruit dans " << i << "(" << areaI << ")" << endl;
					killContourAndChildren(j);
				}
				j = hierarchy[j][0]; // prochain voisin
			}while(j!=-1);
		}
	}
	
	//dessin des contours :
	if(selectedI != -1)
	{
		selectedI = selectedContour(clickPoint);
		if(selectedI != -1)
		{
			approxEpsilon = (double)approxEpsilonInt;
			cv::approxPolyDP(contours[selectedI], approxPoly, approxEpsilon, true);
		}
	}
	drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
	for(size_t i = 0; i < contours.size(); i++){
		int k = (i+1)*10%255;
		//int k = rand()%255;
		if(i != selectedI) drawContours(drawing, contours, i, Scalar(k,k,k), CV_FILLED);   // fill one gray per contour
		else
		{
			cv::drawContours(drawing, contours, i, Scalar(0,0,255), CV_FILLED);
			cv::drawContours(rgb_copy, contours, i, Scalar(0,0,255), 1);
			cv::polylines(rgb_copy, approxPoly, true, Scalar(255,0,0), 1, LINE_8, 0);
		}
	}
	
	displayPicture(0,NULL);
}

void killContourAndChildren(int contour){
	vector<int> toKill; // contient les indices de tous les contours qu'on va tuer
	toKill.clear();
	toKill.push_back(contour);
	int i = contour; // index de travail
	while(true)
	{
		int j = hierarchy[i][2];
		if(j >= 0) // si le contour a des enfants
		{
			i = j; // on prend le premier
		}
		else if(i != contour) // sinon, si on est dans un enfant, on est dans le plus petit
		{
			toKill.push_back(i); // on va tuer i;
			
			j = hierarchy[i][0];
			if(j >= 0) // s'il avait un voisin
			{
				i=j; // on le prend
			}
			else // sinon
			{
				hierarchy[hierarchy[i][3]][2] = -1; // son père n'a plus de fils
				i=hierarchy[i][3]; // on remonte au père
			}
		}
		
		if(i==contour) break;
	}
	
	// on est remonté à i, il faut modifier son prochain voisin :
	int ii = hierarchy[i][0];
	if(ii >= 0) hierarchy[ii][1] = hierarchy[i][1];
	
	// son précédent :
	ii = hierarchy[i][1];
	if(ii >= 0) hierarchy[ii][0] = hierarchy[i][0];
	
	// et son père :
	ii = hierarchy[i][3];
	if(ii >= 0) hierarchy[ii][2] = hierarchy[i][0];
	
	// enfin, on peut tuer tout le monde :
	
	for(int index = 0; index < toKill.size(); index++) contours[toKill[index]].clear();
	
}

double realContourArea(int contour)
{
	if(contours[contour].empty()) return 0;
	
	double childrenArea = 0;
	int i = hierarchy[contour][2];
	while(i >= 0)
	{
		if(!contours[i].empty()) // si le contour n'a pas été effacé
		{
			childrenArea += contourArea(contours[i],false);
		}
		
		i = hierarchy[i][0]; // next
	}
	
	return (contourArea(contours[contour],false) - childrenArea);
}

int selectedContour(Point clickPoint)
{
	int iMin = -1;
	double minDist = -1;
	
	for(size_t i = 0; i < contours.size(); i++)
	{
		if(!contours[i].empty())
		{
			if(pointPolygonTest(contours[i], clickPoint, false) > 0)
			{
				if(!minDist == -1)
				{
					double distI = pointPolygonTest(contours[i], clickPoint, true);
					if(minDist > distI)
					{
						minDist = distI;
						iMin = i;
					}
				}
				else
				{
					iMin = i;
					minDist = pointPolygonTest(contours[iMin], clickPoint, true);
				}
			}
		}
	}
	return iMin;
}

void displayPicture(int, void*){
	switch(displayedPicture)
	{
		case 0:
		showInputWindow();
		break;
		
		case 1:
		showCannyWindow();
		break;
		
		case 2:
		showContourWindow();
		break;
	}
}

void showInputWindow(){
	cv::imshow(window_name,rgb_copy);	
}

void showCannyWindow(){
	cv::imshow(window_name,canny_output);
}
void showContourWindow(){
	cv::imshow(window_name,drawing);
}
