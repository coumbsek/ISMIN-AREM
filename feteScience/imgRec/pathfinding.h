#ifndef DEF_PATH
#define DEF_PATH

using namespace std;
using namespace cv;

void pathfinding(double offset);
Point vec2Point(Point depart, vector<double> vec, double lenght);
vector<double> points2Vec(Point A, Point B);
vector<double> offsetVector(Point current, Point next, Point previous);


#endif
