#include <stdio.h>
#include <stdlib.h>
#include<math.h>
#include"HelperFuncs.h"
#include "General.h"
#include <omp.h>

// Calculates the value of X and Y for a point.
void calcXY(int numOfPoints, Point* allPoints, double t)
{
	
	for (int p = 0; p < numOfPoints; p++)
	{
		allPoints[p].x = ((allPoints[p].x2 - allPoints[p].x1) / 2) * sin(t * PI /2) + ((allPoints[p].x2 + allPoints[p].x1)/2);
		allPoints[p].y = (allPoints[p].a) * (allPoints[p].x) + allPoints[p].b;
	}
	
}

// Calculates the distance between two points.
double calcDistance(Point* point1, Point* point2)
{
	return sqrt(pow(point2->x - point1->x, 2) + pow(point2->y - point1->y, 2));
}


// findPoints with OMP.
int findPoints(GivenData* data, Point* allPoints, double t, Result* result)
{

    int a ; // pointsID index counter.

    int pointsID[3]; // Array that stores the Proximity Criteria points id.

    int countForSpecificPoint ; // To check whether we found K – minimal number of points.

    int countForSpecificT; // To Check whether we found 3 points for t.

    int isPointsFound = 0; // Shared flag variable to indicate whether the result is found.
    
    int i,j;
    
    // For each point find K – minimal number of points that their distance < D.
    #pragma omp parallel shared(pointsID) private(i,j,a,countForSpecificPoint,countForSpecificT) num_threads(1)
    {
	    countForSpecificPoint = 0;
	    countForSpecificT = 0;
	    a = 0;

	    #pragma omp for
	    for (i = 0; i < data->numOfPoints; i++)
	    {
	   	   if(isPointsFound == 1)
		   {
		  	 continue;
		   }
	    	  
	    	   countForSpecificPoint = 0;
	    	   
		   for (j = 0; j < data->numOfPoints; j++)
		   {
		       if(isPointsFound == 1)
		       {
		    	   continue;
		       }
		    
		       if (i != j)
		       {
			      double distance = calcDistance(&allPoints[i], &allPoints[j]);
			      if (distance < data->distance)
			      {
				  	countForSpecificPoint++;  
			      }

			      if (countForSpecificPoint == data->miniNumOfPCPoints)
			      {
				      if (a < 3 && allPoints[i].id != pointsID[0] && allPoints[i].id != pointsID[1] && allPoints[i].id != pointsID[2])
				      {
				           pointsID[a] = allPoints[i].id;
				           a++;
				           countForSpecificT++;
				      }
			      }
			      
			      if (countForSpecificT == 3 && !isPointsFound)
			      {
				      result->t = t;
				      result->point1ID = pointsID[0];
				      result->point2ID = pointsID[1];
				      result->point3ID = pointsID[2];
				      isPointsFound = 1;
			      }
			      
			      if(j + 1 == data->numOfPoints)
			      {
			       	      countForSpecificPoint=0;
			      }
 		      }
		   }
	    }
    }
    
    return isPointsFound;

}
