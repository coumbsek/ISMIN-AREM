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
Point vec2Point(Point depart, Vect vec);
Vect points2Vec(Point A, Point B);
Vect offsetVector(Point A, Point B, double offset);


#endif
