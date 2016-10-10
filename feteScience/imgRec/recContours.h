#ifndef REC_H
#define REC_H
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

void drawStuff(int, void*);
void displayPicture(int, void*);
void showInputWindow();
void showCannyWindow();
void showContourWindow();
void killContourAndChildren(int contour);
double realContourArea(int contour);
int selectedContour(Point clickPoint);
void onMouse(int event, int x, int y, int, void*);
void pathfinding();

#endif
