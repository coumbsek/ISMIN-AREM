#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;

void drawStuff(int, void*);
void displayPicture(int, void*);
void showInputWindow();
void showCannyWindow();
void showContourWindow();
void killContourAndChildren(int contour);

int displayedPicture=0; // image affichée
int lowThreshold=43;
int lowNegligeable=10;
int const max_lowThreshold = 100;
int const max_Negligeable = 100;
const char* window_name = "Parametres du traitement";

int thresh = 40;
int max_thresh = 120;
Mat img_rgb,img_gray,img_bw,canny_output,drawing;
Mat _img;

vector<vector<Point> > contours;
vector<Vec4i> hierarchy;
vector<Point> approxShape;
vector<vector<int> > sons;

int main(int argc, char *argv[]){
	img_rgb  = imread(argv[1]); // charge l'image passée en paramètre (img_rgb)
	
	blur( img_rgb, img_rgb, Size(3,3) ); // floute l'image
	namedWindow( window_name, WINDOW_AUTOSIZE ); // nouvelle fenêtre
	showInputWindow();
	
	Scalar meanColor = mean(img_rgb);
	
	lowThreshold = (meanColor[0]+meanColor[1]+meanColor[1])/4;
		
	cv::createTrackbar( "Pas de Canny :", window_name, &lowThreshold, max_lowThreshold, drawStuff ); // barre du pas pour Canny
	cv::createTrackbar( "Aire negligeable :", window_name, &lowNegligeable, max_Negligeable, drawStuff ); // barre du pourcentage d'aire négligeable pour nettoyage
	cv::createTrackbar( "Filtre - 0=Image de depart, 1=Canny, 2=Contours :", window_name, &displayedPicture, 2, displayPicture); // image affichée
	
	drawStuff(0,0);
	
	cv::moveWindow	(window_name,0,0);
	cv::moveWindow	("Canny",0,0);
	cv::moveWindow	("Fill",0,0);
	
	cv::waitKey(0);
}

void drawStuff(int, void*){ // fonction appelée au début et à chaque changement des trackbar
	
	cout << "--------" << endl;
	
	Canny( img_rgb, canny_output, lowThreshold, lowThreshold*1.5, 3 ); // filtre de Canny dans canny-output
	cv::dilate(canny_output, canny_output, cv::Mat(), cv::Point(-1,-1)); // dilate chaque pixel dans une matrice 3*3

	findContours( canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) ); // -> contours
	
	for(int i = 0; i<contours.size(); i++){
		int j = hierarchy[i][2]; // premier fils
		if (j>=0){ // si i a un fils
			do {
				double areaI = contourArea(contours[i],false);
				double areaJ = contourArea(contours[j],false);
				if(areaJ<=(areaI*lowNegligeable/1000)){ // si l'aire de j est bien plus petite que celle de i...
				
					//drawContours(drawing, contours, i, Scalar(grays[i],grays[i],grays[i]), CV_FILLED);   // fill one gray per contour
					//printf("contour père %d : fils %d couleur %d aire %f\n",i, j,(i+i)*10%255,areas[i]);
					
					killContourAndChildren(j);
				}
				j = hierarchy[j][0]; // prochain voisin
			}while(j!=-1);
		}
	}
	
	//dessin des contours :
	drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
	for(size_t i = 0; i < contours.size(); i++){
		int k = (i+1)*10%255;
		drawContours(drawing, contours, i, Scalar(k,k,k), CV_FILLED);   // fill one gray per contour
	}
	
	displayPicture(0,NULL);
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
	if(ii >= 0) hierarchy[ii][1] = -1;
	
	// son précédent :
	ii = hierarchy[i][1];
	if(ii >= 0) hierarchy[ii][0] = -1;
	
	// et son père :
	ii = hierarchy[i][3];
	if(ii >= 0) hierarchy[ii][2] = -1;
	
	// enfin, on peut tuer tout le monde :
	if(toKill.size() > 1)
	{
	cout << "Pile a tuer pour " << contour << " :" << endl;
	for(int index = 0; index < toKill.size(); index++) cout << toKill[index] << ",";
	cout << endl;
	}
}

void showInputWindow(){
	cv::imshow(window_name,img_rgb);	
}

void showCannyWindow(){
	cv::imshow(window_name,canny_output);
}
void showContourWindow(){
	cv::imshow(window_name,drawing);
}
