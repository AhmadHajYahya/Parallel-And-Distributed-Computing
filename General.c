#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <stddef.h>
#include"General.h"
#include"HelperFuncs.h"
#include <omp.h>

FILE* inputFile;

// Reads the given data (N, k, D, tCount) from text file.
int readDataFromFile(const char* filePath ,GivenData* data) 
{
	inputFile = fopen(filePath, "r");

	if (inputFile == NULL)
	{
		printf("Failed to open the file. (In readDataFromFile())\n");
		return 1;
	}

	if (fscanf(inputFile, "%d %d %lf %d", &(data->numOfPoints), &(data->miniNumOfPCPoints), &(data->distance), &(data->tCount)) != 4)
	{
		printf("Error reading the file. (In readDataFromFile())\n");
		fclose(inputFile);
		return 1;
	}

	fclose(inputFile);

	return 0;
}
// Reads points from text file.
void readPointsFromFile(const char* filePath, int numPoints,Point* points)
{
	inputFile = fopen(filePath, "r");

	if (inputFile == NULL)
	{
		printf("Failed to open the file. (In readPointsFromFile())\n");
		return ;
	}

	// Skip the first line, start reading point from the second line of the file.
	char buffer[100];
	fgets(buffer, sizeof(buffer), inputFile);
	
			
	// Read the remaining lines and store the points in the array.
	for (int i = 0; i < numPoints; i++)
	{
		if (fgets(buffer, sizeof(buffer), inputFile) == NULL)
		{
			printf("Error reading line: %d (In readPointsFromFile())\n", i + 2);
			fclose(inputFile);
			return ;
		}

		if (sscanf(buffer, "%d %lf %lf %lf %lf", &(points[i].id), &(points[i].x1), &(points[i].x2), &(points[i].a), &(points[i].b)) != 5)
		{
			printf("Error reading line: %d (In readPointsFromFile())\n", i + 2);
			fclose(inputFile);
			return;
		}
		points[i].x = 0.0;
		points[i].y = 0.0;	

	}
	
	fclose(inputFile);
}

void writeResultToOutputFile(Result* gathered_results, int gathered_results_length) 
{
    FILE* outputFile = fopen(outputFilePath, "w");
    if (outputFile == NULL) {
        printf("Error opening the output file.\n");
        return;
    }
 
    for (int i = 0; i < gathered_results_length; i++) {
        fprintf(outputFile, "Points PointID%d, PointID%d, PointID%d satisfy Proximity Criteria at t = %lf\n",
                gathered_results[i].point1ID, gathered_results[i].point2ID, gathered_results[i].point3ID,
                gathered_results[i].t);
    }

    fclose(outputFile);
}

int writeMessageToOutputFile(const char* msg)
{
	FILE* outputFile = fopen(outputFilePath, "w");
	if (outputFile == NULL)
	{
		printf("Failed to open the file. (In writeMessageToOutputFile())\n");
		return 1;
	}
	fprintf(outputFile, "%s\n", msg);
	fclose(outputFile);
	return 0;
}

void createPoint_Datatype(MPI_Datatype *datatype) 
{
	int block_lengths[7] = { 1, 1, 1, 1, 1, 1, 1 };
	MPI_Datatype types[7] = { MPI_INT, MPI_DOUBLE, MPI_DOUBLE, MPI_DOUBLE, MPI_DOUBLE, MPI_DOUBLE, MPI_DOUBLE };
	MPI_Aint offsets[7] = { offsetof(Point, id), offsetof(Point, x1), offsetof(Point, x2), offsetof(Point, a), offsetof(Point, b), offsetof(Point, x), offsetof(Point, y) };
	MPI_Type_create_struct(7, block_lengths, offsets, types, datatype);
	MPI_Type_commit(datatype);
}

void createGivenData_Datatype(MPI_Datatype *datatype)
{
	int block_lengths[4] = { 1, 1, 1, 1};
	MPI_Datatype types[4] = { MPI_INT, MPI_INT, MPI_DOUBLE, MPI_INT};
	MPI_Aint offsets[4] = { offsetof(GivenData, numOfPoints), offsetof(GivenData, miniNumOfPCPoints), offsetof(GivenData, distance), offsetof(GivenData, tCount) };
	MPI_Type_create_struct(4, block_lengths, offsets, types, datatype);
	MPI_Type_commit(datatype);
}

void createResult_Datatype(MPI_Datatype *datatype)
{
	int block_lengths[4] = { 1, 1, 1, 1};
	MPI_Datatype types[4] = { MPI_DOUBLE, MPI_INT, MPI_INT, MPI_INT};
	MPI_Aint offsets[4] = { offsetof(Result, t), offsetof(Result, point1ID), offsetof(Result, point2ID), offsetof(Result,point3ID) };
	MPI_Type_create_struct(4, block_lengths, offsets, types, datatype);
	MPI_Type_commit(datatype);
}


