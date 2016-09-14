#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
//
//	Multithreading
//
#include <boost/thread/thread.hpp>
#include <boost/atomic.hpp>

#define MATRIX_DIMENSION 3000

int32_t *A, *B, *C;

long long millis()
{
    struct timeval te; 
    gettimeofday(&te, NULL); // get current time
    long long milliseconds = te.tv_sec*1000LL + te.tv_usec/1000; // calculate milliseconds
    return milliseconds;
}

int32_t *Allocate(int m, int n) {
	int32_t *ms;

	
	ms = (int32_t *)calloc(m*n,sizeof(int32_t));
	return ms;
}
//
//	Multiply A * B starting at Row iStart and continue for iNum Rows
//
void MMult(int32_t *A, int32_t *B, int32_t *C, int m, int p, int n, int iStart, int iNum)
{
	// A = input matrix (m x p)
	// B = input matrix (p x n)
	// m = number of rows in A
	// p = number of columns in A = number of rows in B
	// n = number of columns in B
	// C = output matrix = A*B (m x n)
	int i, j, k;
	for (i=iStart;i<iStart + iNum;i++)
		for (k=0;k<p;k++)
		{
			for(j=0;j<n;j++)
			{
				if (k == 0)
					C[n*i+j]=0;
				C[n*i+j]= C[n*i+j]+A[p*i+k]*B[n*k+j];
			}
		}

}
int main(int argc, const char *argv[])
{
	//
	// get the number of threads to run
	//
	int nThreads;
	int nRows;
	int nAdd;
	
	if (argc >=2) {
		nThreads = atoi(argv[1]);
	} else {
		nThreads = boost::thread::hardware_concurrency();
	};
	//
	//	Allocate the matrix storage
	//
	A = Allocate(MATRIX_DIMENSION, MATRIX_DIMENSION);
	B = Allocate(MATRIX_DIMENSION, MATRIX_DIMENSION);
	C = Allocate(MATRIX_DIMENSION, MATRIX_DIMENSION);
	//
	//	Figure out the number of rows each thread should compute
	//
	nRows = MATRIX_DIMENSION / nThreads;
	//
	//	nRows times nThreads might not cover all the dimensions.
	//
	nAdd = MATRIX_DIMENSION - (nRows * nThreads);
	
	boost::thread_group MM_threads;
	
	int iStart = 0;
	//Start the threads
	//printf("Performing Matrix Multiply with %d threads.\n", nThreads);
	long long startTM = millis();
	for (int i = 0; i < nThreads; i++) {
		int iRows = nRows;
		if (nAdd > 0) {
			iRows += 1;
			nAdd -= 1;
		};
		//printf ("Starting thread at row %d for %d rows\n", iStart, iRows);
		MM_threads.add_thread(new boost::thread(MMult,A, B, C, MATRIX_DIMENSION, MATRIX_DIMENSION, MATRIX_DIMENSION, iStart, iRows));
		iStart += iRows;
	};
	//
	//	Wait for the threads to complete
	//
	MM_threads.join_all();
	long long stopTM = millis();
	printf("%d,%lld,%lld,%lld\n", nThreads,startTM, stopTM, (stopTM - startTM));
	//printf("Done computing.\n");
	return 0;
}
