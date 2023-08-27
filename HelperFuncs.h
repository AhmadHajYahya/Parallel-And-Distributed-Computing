#ifndef __POINT__
#define __POINT__
#include"General.h"

// Calculates the value of X and Y for a point.
void calcXY(int numOfPoints, Point* allPoints, double t);

// Calculates the distance between two points.
double calcDistance(Point* point1, Point* point2);

int findPoints(GivenData* data,Point* allPoints, double t, Result* result);
#endif
