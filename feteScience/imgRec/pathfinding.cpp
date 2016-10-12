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
	cout << "--- Nouveau path ---" << endl;
	convexHull(approxPoly, approxConvex, false, false); // on approxime par un poly convexe
	if(approxConvex.size() < 3) return; // si ce n'est pas au moins un triangle on sort
	
	Point A1,A2,B1,B2,L1,L2;
	
	A1 = approxConvex[0]; 
	A2 = approxConvex[approxConvex.size()-1];
	B1 = approxConvex[1];
	B2 = approxConvex[2];
	size_t iB2 = 2;
	size_t iA2 = approxConvex.size()-1;
	
	Vector2d vOffset = offsetVector(A1,B1,offset);
	Vector2d vAB = points2Vec(A1,B1);
	
	path.clear();
	
	// on commence par aller de A1 à B1
	
	path.push_back(A1);
	path.push_back(B1);
	
	bool continuer = true;
	bool altern = true;
	
	Point B = B1;
	int cmpt = 1;
	
	while(continuer)
	{	
		// on recherche les points d'intersection de la parallèle à A1B1 décalée de vOffset
	
		Point O = vec2Point(B, vOffset*cmpt); // à partir du point 0, on cherche ces points
		Vector2d vB12 = points2Vec(B1,B2); // on a besoin de croiser vB12 avec vAB, on vérifie qu'ils ne sont pas colinéaires
		Vector2d vA12 = points2Vec(A1,A2);
	
		if(vAB == vB12 || vAB == vA12 || vAB == vB12 * -1 || vAB == vA12 * -1)
		{
			cout << "vecteurs colineaires" << endl;
			continuer = false; // c'est possible mais ça signifie qu'on est arrivé au bout du champ
		}
		else
		{	
			Matrix4d MB;
			MB << 	1,-1*vAB[0],0,0,
				1,0,0,-1*vB12[0],
				0,-1*vAB[1],1,0,
				0,0,1,-1*vB12[1];
			//cout << "MB : " << endl << MB << endl;
	
			Vector4d CB;
			CB << O.x,B1.x,O.y,B1.y;
			//cout << "CB : " << endl << CB << endl;
	
			FullPivLU<Matrix4d> decompositionB(MB);

		    	Vector4d XB = decompositionB.solve(CB); // X : (x, t, y, t')
		    	cout << "XB : " << endl << XB << endl;
		    	
		    	Matrix4d MA;
			MA << 	1,-1*vAB[0],0,0,
				1,0,0,-1*vA12[0],
				0,-1*vAB[1],1,0,
				0,0,1,-1*vA12[1];
			//cout << "MA : " << endl << MA << endl;
	
			Vector4d CA;
			CA << O.x,A1.x,O.y,A1.y;
			//cout << "CA : " << endl << CA << endl;
	
			FullPivLU<Matrix4d> decompositionA(MA);

		    	Vector4d XA = decompositionA.solve(CA); // X : (x, t, y, t')
		    	cout << "XA : " << endl << XA << endl;
		    	
		    	Point BB = Point(XB[0],XB[2]);
		    	Point AA = Point(XA[0],XA[2]);	    	
		    	
		    	// il faut regarder si on est sorti de l'une des deux arêtes
		    	
		    	Vector2d vNBB = points2VecN(B1,BB);
		    	Vector2d vNAA = points2VecN(A1,AA);
		    	Vector2d vNB12 = points2VecN(B1,B2);
		    	Vector2d vNA12 = points2VecN(A1,A2);
		    	
		    	if(vNBB.squaredNorm() > vNB12.squaredNorm())
		    	{
		    		iB2++;
		    		B1 = B2;
		    		B2 = approxConvex[iB2];
		    		if(iA2 >= iB2) continue;
		    	}
		    	else
		    	{
		    		B1 = BB;
		    	}
		    	
		    	if(vNAA.squaredNorm() > vNA12.squaredNorm())
		    	{
		    		iA2--;
		    		A1 = A2;
		    		A2 = approxConvex[iA2];
		    		if(iA2 >= iB2) continue;
		    	}
		    	else
		    	{
		    		A1 = AA;
		    	}
		    	
		    	if(iA2 < iB2) // on est arrivé au bout du champ
		    	{
		    		continuer = false;
		    		cout << "iA2:"<<iA2<<"<iB2:"<<iB2<<endl;
		    		break;
		    	}
		    	
		    	// ici, on vérifie que nos points sont dans l'approxPoly, s'ils n'y sont pas, on les force à y rentrer.
		    	double distAA = pointPolygonTest(approxPoly, AA, true);
		    	if(distAA < 0)
		    	{
		    		// on cherche le point d'intersection entre AA-BB et l'approxPoly (à la bourrin)
		    		int t = 0;
		    		while(true)
		    		{
		    			t++;
			    		//Point AAA = vec2Point(AA, t * vAB);
			    		Point AAA = vec2Point(AA, t * points2Vec(AA,BB));
			    		double distAAA = pointPolygonTest(approxPoly, AAA, true);
			    		
			    		AA = AAA;
			    		
			    		if(distAAA >= 0)
			    		{
			    			cout << "On est rentré dans le polygone pour AA" << endl;
			    			break;
			    		}
			    		else if(distAAA < distAA - 1000)
			    		{
			    			cout << "ERREUR : on s'eloigne !" << endl;
			    			break;
			    		}
		    		}
		    	}
		    	double distBB = pointPolygonTest(approxPoly, BB, true);
		    	if(distBB < 0)
		    	{
		    		// on cherche le point d'intersection entre AA-BB et l'approxPoly (à la bourrin)
		    		int t = 0;
		    		while(true)
		    		{
		    			t++;
			    		//Point BBB = vec2Point(BB, (-1*t) * vAB);
			    		Point BBB = vec2Point(BB, t * points2Vec(BB,AA));
			    		double distBBB = pointPolygonTest(approxPoly, BBB, true);
			    		
			    		BB = BBB;
			    		
			    		if(distBBB >= 0)
			    		{
			    			cout << "On est rentré dans le polygone pour BB" << endl;
			    			break;
			    		}
			    		else if(distBBB < distBB - 1000)
			    		{
			    			cout << "ERREUR : on s'eloigne !" << endl;
			    			break;
			    		}
		    		}
		    	}
		    	
		    	if(altern)
		    	{
			    	path.push_back(BB);
			    	path.push_back(AA);
			    	cout << "nouveau trait : " << BB<< "-"<< AA<<endl;
		    	}
		    	else
		    	{
		    		path.push_back(AA);
			    	path.push_back(BB);
			    	cout << "nouveau trait : " << AA<< "-"<< BB<<endl;
		    	}
	    	}
    	altern = !altern;
    	cmpt++;
    	
    	cv::polylines(rgb_copy, path, false, Scalar(0,255,0), 1, LINE_8, 0);
	displayPicture(0,NULL);
    	
    	//cv::waitKey(0);
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

Vector2d points2VecN(Point A, Point B) // transforme AB en vecteur normé
{
	double moduleX = (B.x - A.x);
	double moduleY = (B.y - A.y);
	
	Vector2d V;
	V[0] = moduleX;
	V[1] = moduleY;
	
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


