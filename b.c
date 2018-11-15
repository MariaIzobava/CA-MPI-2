#include <stdio.h>
#include "mpi.h"
#include <sys/timeb.h>
#include <stdlib.h>

int main(int argc, char ** argv)
{
	 int myrank, ranksize;
	 int i;
	 MPI_Status status;

   	 
	 MPI_Init (&argc, &argv);       /* initialize MPI system */
	 MPI_Comm_rank (MPI_COMM_WORLD, &myrank);    /* my place in MPI system */
	 MPI_Comm_size (MPI_COMM_WORLD, &ranksize);  /* size of MPI system */
	 int n, m;
	 int* matr;
	 int* v;
	 int send_count;
	 int send_count_zero;

	 if (myrank == 0) {
		 freopen("input.txt", "r", stdin);
		 scanf("%d%d", &n, &m);
		 matr = malloc(n * m * sizeof(int));
		 v = malloc(m * sizeof(int));
		 for (int i = 0; i < n; i++){
	 		for (int j = 0; j < m; j++) {
	 			int x;
	 			scanf("%d", &x);
	 			matr[i * m + j] = x;
	 		}
		 }
		 for (int i = 0; i < m; i++) {
		 	int x;
		 	scanf("%d", &x);
		 	v[i] = x;
		 }
		 send_count = n / ranksize;
		 send_count_zero = n / ranksize;
		 if (n % ranksize > 0) {
		 	send_count += 1;
		 	send_count_zero = n % ranksize;
		 }
		 for (int i = 1; i < ranksize; i++) {
			 MPI_Send(&send_count_zero, 1, MPI_INT, i, 98, MPI_COMM_WORLD);
			 MPI_Send(&send_count, 1, MPI_INT, i, 98, MPI_COMM_WORLD);
			 MPI_Send(&n, 1, MPI_INT, i, 98, MPI_COMM_WORLD);
			 MPI_Send(&m, 1, MPI_INT, i, 98, MPI_COMM_WORLD);
			 MPI_Send(v, m, MPI_INT, i, 98 ,MPI_COMM_WORLD);
		}
	 }
	 else {
	 	MPI_Recv(&send_count_zero, 1, MPI_INT, 0, 98, MPI_COMM_WORLD, &status);
	 	MPI_Recv(&send_count, 1, MPI_INT, 0, 98, MPI_COMM_WORLD, &status);
	 	MPI_Recv(&n, 1, MPI_INT, 0, 98, MPI_COMM_WORLD, &status);
	 	MPI_Recv(&m, 1, MPI_INT, 0, 98, MPI_COMM_WORLD, &status);
	 	v = malloc(m * sizeof(int));
	 	MPI_Recv(v, m, MPI_INT, 0, 98 ,MPI_COMM_WORLD, &status);

	 }

	 MPI_Barrier (MPI_COMM_WORLD);  
	 if (myrank == 0)              
	 {
		MPI_Send(matr, send_count_zero * m, MPI_INT, 0, 98, MPI_COMM_WORLD);
		for (i = 1; i < ranksize; i++)
			MPI_Send (matr + (send_count_zero + send_count * (i - 1)) * m, m * send_count, MPI_INT, i, 98, MPI_COMM_WORLD);
	 } 
	 MPI_Barrier (MPI_COMM_WORLD);
	 int* buff;
	 int k;
	 if (myrank == 0) 
	 	k = send_count_zero;
	 else
	 	k = send_count;
	 buff = malloc(k * m * sizeof(int));
	 MPI_Recv(buff, k * m, MPI_INT, 0, 98, MPI_COMM_WORLD, &status);
	 int* out;
	 int* out_k;
	 int* out_pos;
	 if (myrank == 0) {
	 	out = malloc(n * sizeof(int));
	 	out_k = malloc(n * sizeof(int));
	 	out_pos = malloc(n * sizeof(int));
	 	for (int i = 0; i < n; i++){
	 		out_k[i] = 1;
	 		out_pos[i] = i;
	 	}

	 }
	 for (int i = 0; i < k; i++) {
	 	int s = 0;
	 	for (int j = 0; j < m; j++)
	 		s += buff[i * m + j] * v[j];
	 	MPI_Gatherv(&s, 1, MPI_INT, out, out_k, out_pos, MPI_INT, 0, MPI_COMM_WORLD);
	 }

	 if (myrank == 0) {
	 	for (int i = 0; i < n; i++)
	 		printf("%d ", out[i]);
	 	printf("\n");
	 }
	 MPI_Finalize ();
	 return 0;
}

