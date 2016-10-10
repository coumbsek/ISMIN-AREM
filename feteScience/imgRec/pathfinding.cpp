#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math>

#include "variablesExt.h"
#include "pathfinding.h"

using namespace cv;
using namespace std;

void pathfinding(double offset)
{
	Point A1,A2,B1,B2,L1,L2;
	
	A1 = approxConvex[0];
	A2 = approxConvex[approxConvex.size()-1]
	B1 = approxConvex[1];
	B2 = approxConvex[2];
	
	// recherche de L1 (point de départ) :
	
	Point o1 = vec2Point(A1,,offset)
	
	
}

Point vec2Point(Point depart, vector<double> vec, double lenght)
{
	double lx = (double)depart.x + vec[0]*lenght;
	double ly = (double)depart.y + vec[1]*lenght;
	
	return Point(ceil(lx),ceil(ly)); 
}

vector<double> points2Vec(Point A, Point B)
{
	double moduleX = (B.x - A.x);
	double moduleY = (B.y - A.y);
	double module = sqrt(moduleX*moduleX + moduleY*moduleY);
	
	return Point(moduleX/module, moduleY/module);
}

vector<double> offsetVector(Point current, Point next, Point previous) // il doit être perpendiculaire et dans le bon sens
{
	vector<double> test;
	return test;
}


