#ifndef DEF_PATH
#define DEF_PATH

using namespace std;
using namespace cv;

struct Vect
{
	double x;
	double y;
};

typedef struct Vect Vect;

void pathfinding(double offset);
Point vec2Point(Point depart, vector<double> vec, double lenght);
Vect points2Vec(Point A, Point B);
Vect offsetVector(Point current, Point next, Point previous);


#endif
