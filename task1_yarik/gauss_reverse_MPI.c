#include <stdio.h>
#include "gauss_reverse_MPI.h"

/* Меняет местами глобальные строки i и j распределённой матрицы.
   Если строки живут на разных процессах, они просто обмениваются ими. */
static void swap_rows_mpi(double *local_m, int n, int i, int j, int rank, int size)
{
	int owner_i = i%size, owner_j = j%size, t = 0;
	double tmp = 0;

	if (owner_i == owner_j) {
		if (rank == owner_i) {
			for (t = 0; t < n; ++t) {
				tmp = local_m[(i/size)*n+t];
				local_m[(i/size)*n+t] = local_m[(j/size)*n+t];
				local_m[(j/size)*n+t] = tmp;
			}
		}
	} else if (rank == owner_i) {
		MPI_Sendrecv_replace(local_m+(i/size)*n, n, MPI_DOUBLE, owner_j, 0,
		                     owner_j, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	} else if (rank == owner_j) {
		MPI_Sendrecv_replace(local_m+(j/size)*n, n, MPI_DOUBLE, owner_i, 0,
		                     owner_i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}
}

int gauss_reverse_mpi(int n, double *local_a, double *local_x, int rank, int size,
                      double *buffer, int *col)
{
	int local_n = 0, k = 0, i = 0, j = 0, gi = 0, lk = 0, pi = 0, pj = 0, t = 0;
	double eps = 0, norm = 0, c = 0, inv = 0, tmp = 0;
	double *row_a = buffer, *row_x = buffer + n;
	MAX local_max, global_max;

	local_n = n/size;
	if (rank < n%size)
		++local_n;

	/* Норма матрицы: максимум по столбцам суммы модулей.
	   Нужна только для того, чтобы понимать, какое число уже считать нулём. */
	for (j = 0; j < n; ++j)
		row_a[j] = 0;
	for (i = 0; i < local_n; ++i)
		for (j = 0; j < n; ++j)
			row_a[j] += fabs(local_a[i*n+j]);
	MPI_Allreduce(row_a, row_x, n, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
	for (j = 0; j < n; ++j)
		if (row_x[j] > norm)
			norm = row_x[j];
	eps = 1e-14*norm;

	/* X = E, col = (0, 1, ..., n-1) */
	for (i = 0; i < local_n; ++i)
		for (j = 0; j < n; ++j)
			local_x[i*n+j] = (i*size+rank == j ? 1. : 0.);
	for (i = 0; i < n; ++i)
		col[i] = i;

	/* ------------------------- ПРЯМОЙ ХОД ------------------------- */
	for (k = 0; k < n; ++k) {
		/* 1. Главный элемент - самый большой по модулю во всей оставшейся
		      подматрице (строки и столбцы с номерами от k до n-1). */
		local_max.value = -1;
		local_max.index = 0;
		for (i = 0; i < local_n; ++i) {
			gi = i*size + rank;
			if (gi < k)
				continue;
			for (j = k; j < n; ++j) {
				tmp = fabs(local_a[i*n+j]);
				if (tmp > local_max.value) {
					local_max.value = tmp;
					local_max.index = gi*n + j;
				}
			}
		}
		MPI_Allreduce(&local_max, &global_max, 1, MPI_DOUBLE_INT, MPI_MAXLOC, MPI_COMM_WORLD);

		if (global_max.value <= eps)
			return -1;			/* матрица вырождена */

		pi = global_max.index/n;
		pj = global_max.index - pi*n;

		/* 2. Ставим главный элемент в угол (k, k).
		      Строки переставляем и в A, и в X, столбцы - только в A. */
		if (pi != k) {
			swap_rows_mpi(local_a, n, k, pi, rank, size);
			swap_rows_mpi(local_x, n, k, pi, rank, size);
		}
		if (pj != k) {
			t = col[k];
			col[k] = col[pj];
			col[pj] = t;
			for (i = 0; i < local_n; ++i) {
				tmp = local_a[i*n+k];
				local_a[i*n+k] = local_a[i*n+pj];
				local_a[i*n+pj] = tmp;
			}
		}

		/* 3. Делим строку k на главный элемент и рассылаем её всем процессам:
		      в row_a - строка матрицы A, в row_x - строка матрицы X. */
		if (rank == k%size) {
			lk = k/size;
			inv = 1./local_a[lk*n+k];
			for (j = k; j < n; ++j)
				local_a[lk*n+j] *= inv;
			for (j = 0; j < n; ++j)
				local_x[lk*n+j] *= inv;
			for (j = 0; j < n; ++j) {
				row_a[j] = local_a[lk*n+j];
				row_x[j] = local_x[lk*n+j];
			}
		}
		MPI_Bcast(buffer, 2*n, MPI_DOUBLE, k%size, MPI_COMM_WORLD);

		/* 4. Вычитаем строку k только из строк НИЖЕ неё - это и есть Гаусс
		      (у Жордана здесь зануляются и строки выше). */
		for (i = 0; i < local_n; ++i) {
			gi = i*size + rank;
			if (gi <= k)
				continue;
			c = local_a[i*n+k];
			if (c == 0.)
				continue;
			for (j = k; j < n; ++j)
				local_a[i*n+j] -= c*row_a[j];
			for (j = 0; j < n; ++j)
				local_x[i*n+j] -= c*row_x[j];
		}
	}

	/* ------------------------ ОБРАТНЫЙ ХОД ------------------------ */
	/* Сейчас A - верхняя треугольная с единицами на диагонали.
	   Идём снизу вверх и убираем то, что осталось над диагональю.
	   Саму матрицу A при этом не трогаем: она дальше не нужна. */
	for (k = n-1; k > 0; --k) {
		if (rank == k%size) {
			lk = k/size;
			for (j = 0; j < n; ++j)
				row_x[j] = local_x[lk*n+j];
		}
		MPI_Bcast(row_x, n, MPI_DOUBLE, k%size, MPI_COMM_WORLD);

		for (i = 0; i < local_n; ++i) {
			gi = i*size + rank;
			if (gi >= k)
				continue;
			c = local_a[i*n+k];
			if (c == 0.)
				continue;
			for (j = 0; j < n; ++j)
				local_x[i*n+j] -= c*row_x[j];
		}
	}

	/* --------------- РАССТАНОВКА СТРОК НА МЕСТА --------------- */
	/* Из-за перестановок столбцов строки ответа перепутаны:
	   строка k того, что получилось, - это строка col[k] обратной матрицы.
	   Массив col одинаковый на всех процессах, поэтому все делают одни и те же обмены. */
	for (i = 0; i < n; ++i) {
		while (col[i] != i) {
			j = col[i];
			swap_rows_mpi(local_x, n, i, j, rank, size);
			col[i] = col[j];
			col[j] = j;
		}
	}

	return 0;
}

/* ||A*B - E||_1 = максимум по столбцам суммы модулей.
   Чтобы не заводить лишнюю память, куски столбца матрицы B гоняются по кольцу
   процессов: за size пересылок каждый процесс увидит весь столбец целиком
   и вернёт свой кусок обратно. */
void r_mpi(int n, double *local_a, double *local_b, double *r, int rank, int size, double *buffer)
{
	int i = 0, j = 0, k = 0, t = 0, local_n = 0, tmp_n = 0, new_rank = 0;
	double sum = 0, col_sum = 0, max = -1;
	MPI_Datatype col_t;

	local_n = n/size;
	if (rank < n%size)
		++local_n;

	/* столбец локальной матрицы: n/size+1 чисел с шагом n */
	MPI_Type_vector(n/size+1, 1, n, MPI_DOUBLE, &col_t);
	MPI_Type_commit(&col_t);

	for (t = 0; t < n; ++t) {
		/* buffer[i] = (A*B)[global_row(i)][t] - E[global_row(i)][t] */
		for (i = 0; i < local_n; ++i)
			buffer[i] = (i*size+rank == t ? -1. : 0.);

		for (k = 0; k < size; ++k) {
			new_rank = (rank+k)%size;
			tmp_n = n/size;
			if (new_rank < n%size)
				++tmp_n;

			/* сейчас у нас куски столбца t, которые принадлежат процессу new_rank;
			   складываем их подряд в buffer+n, чтобы читать память по порядку */
			for (j = 0; j < tmp_n; ++j)
				buffer[n+j] = local_b[j*n+t];

			for (i = 0; i < local_n; ++i) {
				sum = 0;
				for (j = 0; j < tmp_n; ++j)
					sum += local_a[i*n+(j*size+new_rank)]*buffer[n+j];
				buffer[i] += sum;
			}

			MPI_Sendrecv_replace(local_b+t, 1, col_t, (rank-1+size)%size, 0,
			                     (rank+1)%size, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		}

		sum = 0;
		for (i = 0; i < local_n; ++i)
			sum += fabs(buffer[i]);

		MPI_Reduce(&sum, &col_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
		if (rank == 0 && col_sum > max)
			max = col_sum;
	}

	MPI_Type_free(&col_t);
	if (rank == 0)
		*r = max;
}

void r1_r2_mpi(int n, double *local_a, double *local_x, double *r1, double *r2,
               int rank, int size, double *buffer)
{
	if (n > 11000) {
		*r1 = 0;
		*r2 = 0;
		return;
	}

	r_mpi(n, local_a, local_x, r1, rank, size, buffer);	/* ||A*A^(-1) - E|| */
	r_mpi(n, local_x, local_a, r2, rank, size, buffer);	/* ||A^(-1)*A - E|| */
}
