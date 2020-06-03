#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
	int i, j;
	srand(time(NULL));
	int n = atoi(argv[1]);
	int vector__1[n][n], vector__2[n][n], vector__res[n][n];
	for(i = 0; i < n; i++)
	{
		for(j = 0; j < n; j++)
		{
			vector__1[i][j] = (rand()%4)+1;
			vector__2[i][j] = (rand()%4)+1;
		}
	}
	/* vector 1 */
	for(i = 0; i < n; i++)
	{
		for(j = 0; j < n; j++)
		{
			printf("%d ", vector__1[i][j]);
		}
		printf("\n");
	}
	printf("\n");
	/* vector 2 */
	for(i = 0; i < n; i++)
	{
		for(j = 0; j < n; j++)
		{
			printf("%d ", vector__2[i][j]);
		}
		printf("\n");
	}

	printf("\n");
	/* realizar la multiplicación en paralelo */
	#pragma omp parallel
	{
		int i, j, k, suma = 0;
		#pragma omp for 
		for(i = 0; i < n; i++)
		{
			for(j = 0; j < n; j++)
			{
				for(k = 0; k < n; k++)
				{
					suma += (vector__1[i][k] * vector__2[j][k]);
				}
				vector__res[i][j] = suma;
				suma = 0;
			}
		}
	}

	for(i = 0; i < n; i++){
		for(j = 0; j < n; j++){
			printf("%d ", vector__res[i][j]);
		}
		printf("\n");
	}
	return 0;
}
