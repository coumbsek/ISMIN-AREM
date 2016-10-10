#ifndef DEF_PATH
#define DEF_PATH

#include <Eigen/Dense>

using namespace std;
using namespace cv;
using namespace Eigen;

void pathfinding(double offset);
Point vec2Point(Point depart, Vector2d vec);
Vector2d points2Vec(Point A, Point B);
Vector2d offsetVector(Point A, Point B, double offset);


#endif
