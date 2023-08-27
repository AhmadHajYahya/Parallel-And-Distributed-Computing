#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>
#include"HelperFuncs.h"
#include"General.h"
#include<omp.h>

#define inputFilePath  "/home/linuxu/mydir/workspace/largeInput.txt"

int main() 
{
	int rank, size;
	MPI_Init(NULL,NULL);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	
	GivenData data; // Struct that stores the N,k,D,tCount.
	Point* allPoints = NULL; // Array of points.
	
	
	double start_time = MPI_Wtime();
	
	// Create New Data types.
	MPI_Datatype MPI_Point;
	createPoint_Datatype(&MPI_Point);
	MPI_Datatype MPI_GivenData;
	createGivenData_Datatype(&MPI_GivenData);
	MPI_Datatype MPI_Result;
	createResult_Datatype(&MPI_Result);
	
	// Process rank 0
	if(rank == 0)
	{
		if(readDataFromFile(inputFilePath, &data) == 0)
		{
			allPoints= (Point*)malloc(data.numOfPoints * sizeof(Point));
			if (!allPoints)
			{	
				printf("Failed to allocate memory for allPoints, rank = %d\n",rank);
				return 1;
			}
			readPointsFromFile(inputFilePath, data.numOfPoints,allPoints);
		}
		else
		{
			return 1;
		}
		
	}
	// Broadcast given data to all processes.
	MPI_Bcast(&data, 1, MPI_GivenData, 0, MPI_COMM_WORLD);
	
	// Allocate memory for points in every process
	if(rank != 0)
	{
		allPoints = (Point*)malloc(data.numOfPoints * sizeof(Point));
		if(!allPoints)
		{
			printf("Failed to allocate allPoints array, process: %d\n",rank);
		}
	}
	
	// Broadcast points to all processes.
	MPI_Bcast(allPoints,data.numOfPoints, MPI_Point,0,MPI_COMM_WORLD);
	
	
	// Divide the tCount between all processes.
	int chunkSize = (data.tCount + 1)/ size;
	int offset = rank * chunkSize;
	
	
	// If the division between the processes not equal, add the remaining t's to the last process.
	if(rank == size - 1)
	{
		chunkSize += (data.tCount+1) % size;
	}
	
	int physSize = 2; // Initializing physical size of the final result array.
	Result tempResult;
	int mfr_length = 0; // myFinalResult array length.
	Result* myFinalResult = (Result*)malloc(physSize*sizeof(Result));
	if(!myFinalResult)
	{
		printf("Failed to allocate myFinalResult array, process: %d\n",rank);
		return 1;
	}
	
	
	
	// Check Proximity Criteria for every t starting from offset to offset+chunkSize.
	for (int i =  offset; i < (offset + chunkSize); i++)
	{
		   double t = 2 * i / (double)data.tCount - 1; // Calculate value of t.
		   //if(rank == 0)
		   	//printf("chunck = %d , i = %d, t= %.2lf\n",(offset + chunkSize),i,t);
		   // For every Point calculate X and Y according to t.
		   calcXY(data.numOfPoints, allPoints, t);
		   
		   // Find Proximity Criteria points for this t and store them in tempResult.
		   int r = findPoints(&data, allPoints, t,&tempResult);
		   if(r == 1)
		   {	
				// Increase myFinalResult size if the array is full and there is more result to store.
				if(mfr_length == physSize)
				{	
					  physSize *= 2;
					  myFinalResult = (Result*)realloc(myFinalResult,physSize*sizeof(Result));
				}
				myFinalResult[mfr_length]= tempResult;
				mfr_length++;	
		 }  
	}
	
	// Reallocate myFinalResult to free the unused memory cells.
	myFinalResult = (Result*)realloc(myFinalResult,mfr_length*sizeof(Result));

	// Wait for other processes to finish there search for points.
	MPI_Barrier(MPI_COMM_WORLD);
	

	// Gather the mfr_length from each process to the root process and store them in array recvcounts.
	int *recvcounts = NULL;
	if (rank == 0) 
	{
	    recvcounts = (int*)malloc(size * sizeof(int));
	    if(!recvcounts)
	    {
	    	printf("Failed to allocate recvcounts array, process: %d\n",rank);
	    }
	}
	MPI_Gather(&mfr_length, 1, MPI_INT, recvcounts, 1, MPI_INT, 0, MPI_COMM_WORLD);
	

	// Calculate the displacements for the gathered_results array on the root process.
	// The displacements represent the starting indices of each process's data in the gathered array.
	int *displs = NULL;
	int gathered_results_length = 0;
	if (rank == 0) {
	    displs = (int*)malloc(size * sizeof(int));
	    displs[0] = 0;
	    for (int i = 1; i < size; i++) {
		displs[i] = displs[i - 1] + recvcounts[i - 1];
	    }
	    gathered_results_length = displs[size - 1] + recvcounts[size - 1];
	}

	// Allocate memory for the gathered_results on the root process
	Result* gathered_results = NULL;
	if (rank == 0) {
	    gathered_results = (Result*)malloc(gathered_results_length * sizeof(Result));
	    if(!gathered_results)
	    {
	    	printf("Failed to allocate gathered_results array, process: %d\n",rank);
	    }
	}

	// Gather the myFinalResult arrays from all processes to the root process
	MPI_Gatherv(myFinalResult, mfr_length, MPI_Result, gathered_results, recvcounts, displs, MPI_Result, 0, MPI_COMM_WORLD);

	
	if (rank == 0)
	{
		if(gathered_results_length > 0)
		{
			writeResultToOutputFile(gathered_results, gathered_results_length);
		}
		else
		{
			writeMessageToOutputFile("There were no 3 points found for any t.");
		}
	}
	

	if(rank == 0)
	{
		double endTime = MPI_Wtime();
		printf("Time = %f\n",endTime-start_time);
	}
			
	
	free(allPoints);
	free(myFinalResult);
	free(recvcounts);
	free(displs);
	if (rank == 0) {
	    free(gathered_results);
	}
	
	// Free the MPI custom data types
	MPI_Type_free(&MPI_Point);
	MPI_Type_free(&MPI_GivenData);
	MPI_Type_free(&MPI_Result);

	MPI_Finalize();

	return 0;
}
