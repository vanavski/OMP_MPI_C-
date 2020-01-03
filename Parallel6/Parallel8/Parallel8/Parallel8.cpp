#include <stdio.h>
#include <omp.h>
#include <cstdlib>
#include <ctime>
#include <stdio.h>
#include <stdlib.h>
#include "pch.h"

int main() {
	int vector[20];
	int matr[20][20];
	int resultVector[20];

	double parallelTime = 0;
	double consistentTime = 0;

	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 20; j++) {
			vector[i] = i;
			matr[i][j] = i + 2;
		}
	}

	for (int i = 0; i < 20; i++) {
		resultVector[i] = 0;
	}

	omp_set_dynamic(0);
	omp_set_num_threads(3);


	parallelTime = omp_get_wtime();

#pragma omp parallel for
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 20; j++) {
			resultVector[i] += matr[i][j] * vector[j];
		}
	}

	parallelTime = omp_get_wtime() - parallelTime;

	for (int i = 0; i < 20; i++) {
		resultVector[i] = 0;
	}


	consistentTime = clock();

	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 20; j++) {
			resultVector[i] = matr[i][j] * vector[j];
		}
	}

	consistentTime = (clock() - consistentTime) / CLOCKS_PER_SEC;

	printf("parallel: %d,\nconsistent: %d\n", parallelTime, consistentTime);

	return 0;
}