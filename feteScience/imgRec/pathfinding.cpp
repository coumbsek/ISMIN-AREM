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
	
	//Point o1 = vec2Point(A1,,offset)
	
	
}

Point vec2Point(Point depart, Vect vec, double lenght)
{
	double lx = (double)depart.x + vec.x*lenght;
	double ly = (double)depart.y + vec.y*lenght;
	
	return Point(ceil(lx),ceil(ly)); 
}

Vect points2Vec(Point A, Point B) // transforme AB en vecteur unitaire
{
	double moduleX = (B.x - A.x);
	double moduleY = (B.y - A.y);
	double module = sqrt(moduleX*moduleX + moduleY*moduleY);
	
	Vect V;
	V.x = moduleX/module;
	V.y = moduleY/module;
	
	return V;
}

Vect offsetVector(Point A, Point B) // A et B sont les points de l'arrête à couper, leur ordre spatial ne change pas, et le polygone est codé en sens horraire
{
	Vect edge = points2vec(A,B);
	
	Vect offset;
	
	offset.x = edge.y;
	offset.y = -1 * edge.x;
	
	return offset;
}


