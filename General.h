#ifndef __GENERAL__
#define __GENERAL__
#include <mpi.h>
#include <stddef.h>

#define PI 3.141592654
#define outputFilePath "output.txt"

// A struct that represents the data in the first line of input file.
typedef struct {
	int numOfPoints; // N.
	int miniNumOfPCPoints; // minimal number of Proximity Criteria Points.
	double distance; // D.
	int tCount; // tCount.
}GivenData;

// A struct that represents a Point data.
typedef struct {
	int id;
	double x1;
	double x2;
	double a;
	double b;
	double x;
	double y;
} Point;

// A struct that represents the final results of Proximity Creteria.
typedef struct {
	double t;
	int point1ID;
	int point2ID;
	int point3ID;
}Result;


int readDataFromFile(const char* filePath, GivenData* data);
void readPointsFromFile(const char* filePath, int numPoints,Point* points);

void writeResultToOutputFile(Result* gathered_results, int gathered_results_length);
int writeMessageToOutputFile(const char* msg);

void createPoint_Datatype(MPI_Datatype *datatype);
void createGivenData_Datatype(MPI_Datatype *datatype);
void createResult_Datatype(MPI_Datatype *datatype);


#endif
