#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "input.h"
#include "output.h"
#include "gauss_reverse_MPI.h"

int main(int argc, char **argv)
{
	int rank = 0, size = 0, n = 0, r = 0, s = 0, task = 14, flag = 0;
	int *col = NULL;
	char *filename = NULL;
	double *local_a = NULL, *local_x = NULL, *buffer = NULL;
	double r1 = 0, r2 = 0, t1 = 0, t2 = 0;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if (argc < 4 || argc > 5) {
		if (rank == 0)
			printf("Usage: mpirun -np p %s n r s [filename]\n", argv[0]);
		MPI_Finalize();
		return -1;
	}

	n = strtol(argv[1], NULL, 10);
	r = strtol(argv[2], NULL, 10);
	s = strtol(argv[3], NULL, 10);

	if (n <= 0 || r < 0 || s < 0 || s > 4 || (s == 0 && argc != 5) || (s != 0 && argc != 4)) {
		if (rank == 0)
			printf("Invalid arguments.\n");
		MPI_Finalize();
		return -1;
	}
	if (s == 0)
		filename = argv[4];

	/* 2n^2 + O(n): две матрицы по строкам + буфер 2n + массив перестановки n */
	local_a = (double*)malloc((n/size+1)*n*sizeof(double));
	local_x = (double*)malloc((n/size+1)*n*sizeof(double));
	buffer = (double*)malloc(2*n*sizeof(double));
	col = (int*)malloc(n*sizeof(int));

	if (local_a == NULL || local_x == NULL || buffer == NULL || col == NULL) {
		if (rank == 0)
			printf("Not enough memory.\n");
		free(local_a); free(local_x); free(buffer); free(col);
		MPI_Finalize();
		return -1;
	}

	flag = input_mpi(s, filename, n, local_a, buffer, rank, size);
	if (flag) {
		if (rank == 0) {
			if (flag == -1)
				printf("Cannot open file %s\n", filename);
			else if (flag == -2)
				printf("Wrong data in file %s\n", filename);
			else
				printf("Wrong formula number: %d\n", s);
		}
		free(local_a); free(local_x); free(buffer); free(col);
		MPI_Finalize();
		return -1;
	}

	output_mpi(r, n, n, local_a, rank, size, buffer);

	MPI_Barrier(MPI_COMM_WORLD);
	t1 = MPI_Wtime();

	flag = gauss_reverse_mpi(n, local_a, local_x, rank, size, buffer, col);

	MPI_Barrier(MPI_COMM_WORLD);
	t1 = MPI_Wtime() - t1;

	if (flag) {
		r1 = -1;
		r2 = -1;
		t2 = 0;
	} else {
		output_mpi(r, n, n, local_x, rank, size, buffer);

		/* памяти на копию A нет, поэтому читаем её заново */
		input_mpi(s, filename, n, local_a, buffer, rank, size);

		MPI_Barrier(MPI_COMM_WORLD);
		t2 = MPI_Wtime();

		r1_r2_mpi(n, local_a, local_x, &r1, &r2, rank, size, buffer);

		MPI_Barrier(MPI_COMM_WORLD);
		t2 = MPI_Wtime() - t2;
	}

	if (rank == 0)
		printf("%s : Task = %d Res1 = %e Res2 = %e T1 = %.2f T2 = %.2f S = %d N = %d\n",
		       argv[0], task, r1, r2, t1, t2, s, n);

	free(local_a);
	free(local_x);
	free(buffer);
	free(col);

	MPI_Finalize();
	return 0;
}
