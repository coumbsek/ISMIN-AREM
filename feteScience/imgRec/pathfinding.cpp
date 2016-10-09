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

void pathfinding(double offset, double delta){
	Point p1,pc;
	int x,y; //x = xc-x1, y = yc-y1
	
	convexHull(approxPoly,approxConvex,false,false);
	
	p1.x = approxConvex[0].x;
	p1.y = approxConvex[0].y;
	pc.x = approxConvex[1].x;
	pc.y = approxConvex[1].y;
	x = pc.x-p1.x;
	y = pc.y-p1.y;
	
	while (pointPolygonTest(approxConvex,pc,true)
}
