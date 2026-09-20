#include "output.h"

void output_mpi(int r, int l, int n, double *local_a, int rank, int size, double *buffer)
{
	int i = 0, j = 0, rows = 0, cols = 0, owner = 0;
	double *row = NULL;

	rows = (l < r ? l : r);
	cols = (n < r ? n : r);

	for (i = 0; i < rows; ++i) {
		owner = i%size;
		row = local_a + (i/size)*n;

		/* строка живёт не на нулевом процессе - пересылаем её туда */
		if (owner != 0) {
			if (rank == owner) {
				MPI_Send(row, n, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
			} else if (rank == 0) {
				MPI_Recv(buffer, n, MPI_DOUBLE, owner, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				row = buffer;
			}
		}

		if (rank == 0) {
			for (j = 0; j < cols; ++j)
				printf(" %10.3e", row[j]);
			printf("\n");
		}
	}

	if (rank == 0)
		fflush(stdout);
}
