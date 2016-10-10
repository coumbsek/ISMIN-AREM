#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <cmath>

#include <Eigen/Dense>

#include "variablesExt.h"
#include "recContours.h"
#include "pathfinding.h"

using namespace cv;
using namespace std;
using namespace Eigen;

void pathfinding(double offset)
{
	convexHull(approxPoly, approxConvex, false, false);
	if(approxConvex.size() < 3) return;
	
	Point A1,A2,B1,B2,L1,L2;
	
	A1 = approxConvex[0];
	A2 = approxConvex[approxConvex.size()-1];
	B1 = approxConvex[1];
	B2 = approxConvex[2];
	
	Vector2d vOffset = offsetVector(A1,B1,offset);
	Vector2d vAB = points2Vec(A1,B1);
	
	path.clear();
	
	// on commence par aller de A1 à B1
	
	path.push_back(A1);
	path.push_back(B1);
	
	// puis on recherche les points d'intersection de la parallèle à A1B1 décalée de vOffset
	
	Point O = vec2Point(B1, vOffset); // à partir du point 0, on cherche ces points
	Vector2d vB12 = points2Vec(B1,B2); // on a besoin de croiser vB12 avec vAB, on vérifie qu'ils ne sont pas colinéaires
	Vector2d vA12 = points2Vec(A1,A2);
	
	if(vAB == vB12 || vAB == vA12 || vAB == vB12 * -1 || vAB == vA12 * -1) cout << "Vecteurs colineaires" << endl; // c'est possible
	else
	{	
		Matrix4d MB;
		MB << 1,-1*vAB[0],0,0,1,0,0,-1*vB12[0],0,-1*vAB[1],1,0,0,0,1,-1*vB12[1];
		cout << "MB : " << endl << MB << endl;
	
		Vector4d CB;
		CB << O.x,B1.x,O.y,B1.y;
		cout << "CB : " << endl << CB << endl;
	
		FullPivLU<Matrix4d> decompositionB(MB);

	    	Vector4d XB = decompositionB.solve(CB); // X : (x, t, y, t')
	    	cout << "XB : " << endl << XB << endl;
	    	
	    	Matrix4d MA;
		MA << 1,-1*vAB[0],0,0,1,0,0,-1*vA12[0],0,-1*vAB[1],1,0,0,0,1,-1*vA12[1];
		cout << "MA : " << endl << MA << endl;
	
		Vector4d CA;
		CA << O.x,A1.x,O.y,A1.y;
		cout << "CA : " << endl << CA << endl;
	
		FullPivLU<Matrix4d> decompositionA(MA);

	    	Vector4d XA = decompositionA.solve(CA); // X : (x, t, y, t')
	    	cout << "XA : " << endl << XA << endl;
	    	
	    	path.push_back(Point(XB[0],XB[2]));
	    	path.push_back(Point(XA[0],XA[2]));
	    	
	    	cv::polylines(rgb_copy, path, false, Scalar(0,255,0), 1, LINE_8, 0);
	    	displayPicture(0,NULL);
    	}
}

Point vec2Point(Point depart, Vector2d vec)
{
	double lx = (double)depart.x + vec[0];
	double ly = (double)depart.y + vec[1];
	
	return Point(ceil(lx),ceil(ly)); 
}

Vector2d points2Vec(Point A, Point B) // transforme AB en vecteur unitaire
{
	double moduleX = (B.x - A.x);
	double moduleY = (B.y - A.y);
	double module = sqrt(moduleX*moduleX + moduleY*moduleY);
	
	Vector2d V;
	V[0] = moduleX/module;
	V[1] = moduleY/module;
	
	return V;
}

Vector2d offsetVector(Point A, Point B, double offset) // (unitaire) A et B sont les points de l'arrête à couper, leur ordre spatial ne change pas, et le polygone est codé en sens horraire
{
	Vector2d edge = points2Vec(A,B);
	
	Vector2d voffset;
	
	voffset[0] = (-1 * edge[1]) * offset;
	voffset[1] = (edge[0]) * offset;
	
	return voffset;
}


