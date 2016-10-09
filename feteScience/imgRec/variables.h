#ifndef VAR_H
#define VAR_H

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
int lowThreshold=68; // gradient eliminatoire
int hystSize=170; // bande passante avant le gradient qualifiant
int apertureSize=3; // taille de la matrice de floutage
bool L2Gradient=true; // utilisation ou non d'un calcul amélioré du gradient

int lowNegligeable=10;
double approxEpsilon=10;
int approxEpsilonInt=10;

int const max_lowThreshold = 500;
int const max_hystSize = 300;
int const max_Negligeable = 100;
const char* window_name = "Parametres du traitement";

Point clickPoint; // point où l'utilisateur a cliqué
int selectedI = -1; // indice du contour selectionné par l'utilisateur

Mat img_rgb,canny_output,drawing,rgb_copy;

vector<vector<Point> > contours;
vector<Vec4i> hierarchy;
vector<Point> approxPoly;

int displayedPicture=0; // image actuellement affichée
#endif
