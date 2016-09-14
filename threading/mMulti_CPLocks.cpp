//
//	Implements matrix multiply using a Single consumer, multiple producer queue model
//
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
#include <boost/lockfree/queue.hpp>
#include <boost/atomic.hpp>

#define MATRIX_DIMENSION 3000

struct MMREQUEST {
	int i;
	int j;
	int32_t value;
};

int n, m, p;

int32_t *A, *B, *C, *CC;
boost::lockfree::queue<MMREQUEST, boost::lockfree::fixed_sized<false> > MMRequestQueue(0);
int nextI, nextJ;
boost::mutex mtx_;

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
int32_t MMult(int32_t *A, int32_t *B, int n, int p, int i, int j)
{
	// A = input matrix (m x p)
	// B = input matrix (p x n)
	// m = number of rows in A
	// p = number of columns in A = number of rows in B
	// n = number of columns in B
	// C = output matrix = A*B (m x n)
	int k;
	MMREQUEST mm;
	int32_t value = 0;

		for (k=0;k<p;k++)
		{
			value = value +A[p*i+k]*B[n*k+j];
		};
	return value;
	
}
void Consumer()
{
	MMREQUEST mm;
	bool done;
	done = false;
	while (!done)
	{
		while (MMRequestQueue.pop(mm))
		{	
			if (mm.i == -1 || mm.j == -1)
			{
				done = true;
				break;
			};
			//("Storing %d %d\n", mm.i, mm.j);
			C[n*mm.i+mm.j] = mm.value;
			CC[n*mm.i + mm.j] += 1;	// count this store
		};
	};

}

void Producer()
{
	// figure out the next i and j to be computed
	int i, j;
	int32_t value;
	MMREQUEST mm;
	bool done = false;
	
	while (!done)
	{
		//mtx_.lock();
		i = nextI;
		j = nextJ;
		if (nextI != -1 && nextJ != -1)
		{
			nextI += 1;
			if (nextI >= m)
			{
				nextI = 0;
				nextJ += 1;
				if (nextJ >= n)
				{
					nextI = -1;
					nextJ = -1;
				};
			};
			//printf("Computing entry %d, %d\n", i, j);
		} else
			done = true;
		//mtx_.unlock();
		if (!done)
			value = MMult(A, B, n, p, i, j);
		else
			value = 0;
		//
		//	push even if i or j = -1 so consumer exits
		//
		mm.i = i;
		mm.j = j;
		mm.value = value;
		MMRequestQueue.push(mm);
	};
}

int main(int argc, const char *argv[])
{
	//
	// get the number of threads to run
	//
	int nThreads;
	boost::thread_group MM_threads;
	
	if (argc >=2) {
		nThreads = atoi(argv[1]);
	} else {
		nThreads = boost::thread::hardware_concurrency();
	};
	//
	//	Allocate the matrix storage
	//
	n = MATRIX_DIMENSION;
	p = MATRIX_DIMENSION;
	m = MATRIX_DIMENSION;
	A = Allocate(MATRIX_DIMENSION, MATRIX_DIMENSION);
	B = Allocate(MATRIX_DIMENSION, MATRIX_DIMENSION);
	C = Allocate(MATRIX_DIMENSION, MATRIX_DIMENSION);
	CC = Allocate(MATRIX_DIMENSION, MATRIX_DIMENSION);
	//
	//	zero the C matrix store counter
	//
	for (int i=0;i<m;i++)
		for(int j=0;j<n;j++)
			CC[n*i+j]=0;
	
	//Start the threads
	//printf("Performing CP Locked Matrix Multiply with %d threads.\n", nThreads);
	long long startTM = millis();
	//
	//	start the consumer thread
	//
	MM_threads.create_thread(Consumer);
	for (int i = 0; i < nThreads; i++) {
		MM_threads.create_thread(Producer);
	};
	//
	//	Wait for the threads to complete
	//
	MM_threads.join_all();
	//
	//	There still might be entries in the queue to be handled
	//
	MMREQUEST mm;
	while (MMRequestQueue.pop(mm))
	{	
		if (mm.i != -1 && mm.j != -1)
		{
			//("Storing %d %d\n", mm.i, mm.j);
			C[n*mm.i+mm.j] = mm.value;
			CC[n*mm.i + mm.j] += 1;	// count this store
		}
	};

	long long stopTM = millis();
	int zero, one, multi, mcount;
	zero = 0;
	one = 0;
	multi = 0;
	mcount = 0;
	
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n;j++)
		{
			if (CC[n*i+j] == 0)
				zero += 1;
			else if (CC[n*i+j] == 1)
				one += 1;
			else
			{
				multi += 1;
				mcount += CC[n*i + j];
			};
		};
	printf("%d,%lld,%lld,%lld", nThreads,startTM, stopTM, (stopTM - startTM));
	printf(",%d,%d,%d,%d\n", zero, one, multi, mcount);
	//printf("Done computing.\n");
	return 0;
}
