#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>
#include <stdint.h>
#include "hwtimer.h"

#define MAX_NUMP 16
#define BILLION 1000000000L

int   xydim;
int   nThreads;   /*The number of threads to be used for ocean program*/
int   timestamp;
pthread_mutex_t   ThreadLock; /* mutex */

/* -- Debug prints -- */
void printGrid(int** grid, int xysize, int threadId) {
  int i, j;
	printf("ThreadID = %d\n",threadId);
  for(i=0;i<xysize;i++) {
    for(j=0;j<xysize;j++) {
      printf("%i ",grid[j][i]);  
    }
    printf("\n");
  }
  printf("\n\n");
}

void* pthread_ocean (void *tmp)
{
		int threadId = *((int*) tmp);
		int i, j, k;
		int** grid[2];
		int** nxgrid;
		int xysize = (xydim-2) * (threadId%4+1) + 2;

		grid[0] = (int**) malloc(xysize*sizeof(int*));
    grid[1] = (int**) malloc(xysize*sizeof(int*));
    int *temp = (int*) malloc(xysize*xysize*sizeof(int));
    int *other_temp = (int*) malloc(xysize*xysize*sizeof(int));
    // Force xdim to be a multiple of 64 bytes.
    for (i=0; i<xysize; i++) {
        grid[0][i] = &temp[i*xysize];
        grid[1][i] = &other_temp[i*xysize];
    }
    for (i=0; i<xysize; i++) {
        for (j=0; j<xysize; j++) {
            if (i == 0 || j == 0 || i == xysize - 1 || j == xysize - 1)
            {
                grid[0][j][i] = 1000;
                grid[1][j][i] = 1000;
            } else {
                grid[0][j][i] = 500;
                grid[1][j][i] = 500;
            }
            //grid[0][j][i] = grid[1][j][i] = rand();
        }
    }
		/*### Parallel execution start from here. ###*/
		for (k=0; k<timestamp; k++) {
        for (j=1; j<xysize-1; j++) {
					for (i=1; i<xysize-1; i++) {
								/*### 5 grids are sumed and then divided by 5.  ###*/
								/*### remnants lesser than 5 will be discarded. ###*/
								grid[1][j][i] = ((long int)grid[0][j][i-1] + grid[0][j][i+1] + grid[0][j-1][i] + grid[0][j+1][i] + grid[0][j][i])/5;
						}
        }
				/*### The calculated value will be stored back to grid. ###*/
				nxgrid  = grid[1];
				grid[1] = grid[0];
				grid[0] = nxgrid;
    }
		//pthread_mutex_lock(&ThreadLock);
			//printGrid(grid[0], xysize, threadId);
		//pthread_mutex_unlock(&ThreadLock);
    free(temp);
    free(other_temp);
    free(grid[0]);
    free(grid[1]);
}

int main(int argc, char* argv[])
{
    int i,j,t;
  	int ret;
		int *tinfo;
		pthread_t*  threads;
  	pthread_attr_t attr;
		
		hwtimer_t timer;
    initTimer(&timer);
    
    /********************Get the arguments correctly (start) **************************/
    /* 
    Three input Arguments to the program
    1. X Dimension of the grid
    2. Y dimension of the grid
    3. number of timestamp the algorithm is to be performed
    */
    
    if (argc!=4) {
        printf("The Arguments you entered are wrong.\n");
        printf("./pthread_ocean <x-dim> <y-dim> <timestamp> <number of thread>\n");
        return EXIT_FAILURE;
    } else {
        xydim     = atoi(argv[1]);
        timestamp = atoi(argv[2]);
        nThreads  = atoi(argv[3]);
    }
    tinfo = (int*) malloc(nThreads*sizeof(int));

	  if(nThreads > MAX_NUMP || nThreads <= 0)
  	{
    	fprintf(stderr, "number of processors should be between 1 - 16.\n");
    	exit(-1);
  	}
	
  	/* Initialize array of thread structures */
  	threads = (pthread_t *) malloc(sizeof(pthread_t) * nThreads);
  	assert(threads != NULL);
	
  	/* Initialize thread attribute */
  	pthread_attr_init(&attr);
  	pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM); // sys manages contention
		ret = pthread_mutex_init(&ThreadLock, NULL);
  	assert(ret == 0);
	
    /*********************create the grid as required (start) ************************/
    /*
    The grid needs to be allocated as per the input arguments and randomly initialized.
    Remember during allocation that we want to gaurentee a contiguous block, hence the
    nasty pointer math.

    To test your code for correctness please comment this section of random initialization.
    */
		// ################### Start measurement ##################### //
		startTimer(&timer); // Start the time measurment here before the algorithm starts
		for(i=0; i < nThreads; i++) {
			tinfo[i] = i;
    	ret = pthread_create(&threads[i], &attr, pthread_ocean, (void*) &tinfo[i]);
    	assert(ret == 0);
  	}

	 	for(i=0; i < nThreads; i++) {
    	ret = pthread_join(threads[i], NULL);
    	assert(ret == 0);
	  }
		stopTimer(&timer); // End the time measuremnt here since the algorithm ended
		// ################### End measurement ##################### //
    printf("Total Execution time: %lld ns\n", (long long)getTimerNs(&timer)); //Do the time calcuclation
		
		pthread_mutex_destroy(&ThreadLock);
    return EXIT_SUCCESS;
}
