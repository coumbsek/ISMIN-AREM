#ifndef VAREXT_H
#define VAREXT_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using namespace cv;
using namespace std;

// Paramètres de Canny :
extern int lowThreshold; // gradient eliminatoire
extern int hystSize; // bande passante avant le gradient qualifiant
extern int apertureSize; // taille de la matrice de floutage
extern bool L2Gradient; // utilisation ou non d'un calcul amélioré du gradient

extern int lowNegligeable;
extern double approxEpsilon;
extern int approxEpsilonInt;

extern int const max_lowThreshold;
extern int const max_hystSize;
extern int const max_Negligeable;
extern const char* window_name;

extern double offsetLenght;

extern Point clickPoint; // point où l'utilisateur a cliqué
extern int selectedI; // indice du contour selectionné par l'utilisateur

extern Mat img_rgb,canny_output,drawing,rgb_copy;

extern vector<vector<Point> > contours;
extern vector<Vec4i> hierarchy;
extern vector<Point> approxPoly;

extern vector<Point> approxConvex;
extern vector<Point> path;

extern int displayedPicture; // image actuellement affichée
#endif
