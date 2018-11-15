#include <stdio.h>
#include "mpi.h"
#include <sys/timeb.h>
#include <stdlib.h>


/****************************************************************
*                                                               *
*  compute_interval - numerical integration over                *
*                     part of interval                          *
*                                                               *
****************************************************************/

double realtime(void);

double compute_interval (int myrank, int ntasks, long intervals)
{
	 double width, x, localsum;
	 long j;

	 width = 1.0 / intervals;	/* width of single stripe */
	 localsum = 0.0;
	 for (j = myrank; j < intervals; j += ntasks)
	 {
		  x = (j + 0.5) * width;
		  localsum += 4 / (1 + x * x);
	 }
	 return (localsum * width);	/* size of area */
}


int main(int argc, char ** argv)
{
	 long intervals;
	 int myrank, ranksize;
	 double pi, di , send[2],recv[2];
	 int i;
	 MPI_Status status;
	 double t1,t2;
	 double t3, t4, t5, t6,t7;

   
	 t1=realtime();		 
	 MPI_Init (&argc, &argv);       /* initialize MPI system */
	 t2=realtime();
	 MPI_Comm_rank (MPI_COMM_WORLD, &myrank);    /* my place in MPI system */
	 MPI_Comm_size (MPI_COMM_WORLD, &ranksize);  /* size of MPI system */
	 int n, m;
	 int * matr;
	 int send_count;
	 int send_count_zero;

	 if (myrank == 0) {
		 freopen("input.txt", "r", stdin);
		 scanf("%d%d", &n, &m);
		 matr = malloc(n * m * sizeof(int *));
		 for (int i=0; i<n; i++){
		 		for (int j=0; j<m; j++) {
		 			int x;
		 			scanf("%d", &x);
		 			matr[i * m + j] = x;
		 		}
		 }
		 send_count = n / (ranksize + 1);
		 send_count_zero = n % (ranksize + 1);
		 printf("%d %d\n", send_count_zero, send_count);
	 }

	 MPI_Barrier (MPI_COMM_WORLD);  /* make sure all MPI tasks are running */
	 if (myrank == 0)               /* I am the master */
	 {

		/* distribute parameter */
		printf ("Master: Sending # of intervals to MPI-Processes \n");
		// t3 = MPI_Wtime();
		MPI_Send(matr, send_count_zero * m, MPI_LONG, 0, 98, MPI_COMM_WORLD);
		for (i = 1; i < ranksize; i++)
		{
				MPI_Send (matr + (send_count_zero + send_count * (i-1)) * m * sizeof(int), m*send_count, MPI_LONG, i, 98, MPI_COMM_WORLD);
			}
	 }
	 	/* I am a slave */
	 /* receive parameters */
	 int* buff;
	 int k;
	 MPI_Recv (buff, k, MPI_LONG, 0, 98, MPI_COMM_WORLD, &status);

	 printf("%d\n", myrank);
	 for (int i=0; i<k; i++)
	 	printf("%d ", buff[i]);
	 printf("%d\n");
	 

	 /* compute my portion of interval */
	 t4 = MPI_Wtime();
	 pi = compute_interval (myrank, ranksize, intervals);
	 t5 = MPI_Wtime();
	 MPI_Barrier (MPI_COMM_WORLD);
	 t6 = MPI_Wtime();

	 if (myrank == 0)	/* I am the master */
	 /* collect results, add up, and print results */
	 {
		for (i = 1; i < ranksize; i++)
		{
			MPI_Recv (&di, 1, MPI_DOUBLE, i, 99, MPI_COMM_WORLD, &status);
			pi += di;
		}
		t7 = MPI_Wtime();
		printf ("Master: Has collected sum from MPI-Processes \n");


		printf ("\nPi estimation: %.20lf\n", pi);
		printf ("%d tasks used - Execution time: %.3lf sec\n",ranksize, t7 -t3);

		printf("\nStatistics:\n");
		printf("Master: startup: %.0lf msec\n",t2-t1);
		printf("Master: time to send # of intervals:%.3lf sec\n",t4-t3);
		printf("Master: waiting time for sincro after calculation:%.2lf sec\n",t6-t5);
		printf("Master: time to collect: %.3lf sec\n",t7-t6);
		printf("Master: calculation time:%.3lf sec\n",t5-t4);

		MPI_Barrier (MPI_COMM_WORLD);
		/* collect there calculation time */
		for (i = 1; i < ranksize; i++){
			MPI_Recv (recv, 2, MPI_DOUBLE, i, 100, MPI_COMM_WORLD, &status);
			printf("process %d: calculation time: %.3lf sec\twaiting time for sincro.: %.3lf sec\n",i,recv[0],recv[1]);
		}
	 }
	 else{	/* I am a slave */
	 /* send my result back to master */
		  MPI_Send (&pi, 1, MPI_DOUBLE, 0, 99, MPI_COMM_WORLD);

		  MPI_Barrier (MPI_COMM_WORLD);
		  send[0]=t5-t4;
		  send[1]=t6-t5;
		  MPI_Send (send, 2, MPI_DOUBLE, 0, 100, MPI_COMM_WORLD);

	 }

	 MPI_Finalize ();
	 return 0;
}

double realtime()			     /* returns time in seconds */
{
	struct timeb tp;
	ftime(&tp);
	return((double)(tp.time)*1000+(double)(tp.millitm));
}
