#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <pthread.h>
#include <sys/time.h>

int NUM_OF_THREADS, M, N;

float* A;
float* x;
float* y;
float* y_serial;

int _check_results(float x[], float y[])
{
	int i;
	for(int i = 0; i < M; i++)
		if(abs(x[i] - y[i]) > 0.01) return 1;
	return 0;
}

long _calc_time_secs(timeval start, timeval end)
{
	long seconds, useconds;

	seconds  = end.tv_sec  - start.tv_sec;

	useconds = end.tv_usec - start.tv_usec;

	return ((seconds) * 1000 + useconds/1000.0) + 0.5;
}

void Fill_Matrix(float A[], int rows, int cols);
void Fill_Vector(float x[], int rows);
void Print_Matrix(float A[], int rows, int cols);
void Print_Vector(float x[], int rows);

void *_math_vec_prod(void* rank);

void _math_vec_prod(void);

int main(int argc, char* argv[]){

	timeval start;
    timeval end;

    long final_par, final_serial;

	if(argc != 4)
	{
		fputs("Debe colocar M, N y el número de hilos\n", stdout);
		return EXIT_FAILURE;
	}
	else
	{
		M = atoi(argv[1]);
		N = atoi(argv[2]);
		NUM_OF_THREADS = atoi(argv[3]);
	}

	srand(time(NULL));

	int i, _error;

	pthread_t* threads = (pthread_t *)malloc(NUM_OF_THREADS * sizeof(pthread_t));

	A = (float *) malloc(M * N * sizeof(float));
	x = (float *) malloc(N * sizeof(float));
	y = (float *) malloc(M * sizeof(float));
	y_serial = (float *) malloc(M * sizeof(float));


	Fill_Matrix(A, M, N);

	Fill_Vector(x, N);

	gettimeofday(&start, 0);

	for(i = 0; i < NUM_OF_THREADS; i++)
	{
		_error = pthread_create(&threads[i], NULL,
						_math_vec_prod, &i);

		if (_error){
		  fprintf(stderr, "error: pthread_create, rc: %d\n", _error);
		  return EXIT_FAILURE;
		}
	}



	for (i = 0; i < NUM_OF_THREADS; i++)
		pthread_join(threads[i], NULL);


	gettimeofday(&end, 0);


	final_par = _calc_time_secs(start, end);


	gettimeofday(&start, 0);

	_math_vec_prod();

	gettimeofday(&end, 0);

	final_serial = _calc_time_secs(start, end);

	if(!_check_results(y, y_serial))
		fputs("Prueba exitosa\n", stdout);
	else
		fputs("Prueba fallida\n", stdout);


	fprintf(stdout, "TAMAÑO: %d x %d E %d THREADS\n", M, N, NUM_OF_THREADS);
	fprintf(stdout, "TIEMPO CONC.:%d mseg\n", (int)final_par);
	fprintf(stdout, "TIEMPO SERIAL:%d mseg\n", (int)final_serial);



	if(M < 6 && N < 6)
	{
		puts("RES PARALELO:\n");
		Print_Vector(y, M);
		puts("RES SERIAL:\n");
		Print_Vector(y_serial, M);
	}


	free(A);
	free(x);
	free(y);
	free(y_serial);
	free(threads);
}

void Fill_Matrix(float A[], int rows, int cols)
{
	int i, j;
	for (i = 0; i < rows; i++) 
    	for (j = 0; j < cols; j++)
    		A[i * cols + j] = rand() % 1;
}

void Fill_Vector(float x[], int rows)
{
	int i;
	for (i = 0; i < rows; i++) 
		x[i] = rand() % 1;
}

void Print_Matrix(float A[], int rows, int cols) {
   int	i, j;

   for (i = 0; i < rows; i++) {
      for (j = 0; j < cols; j++)
         printf("%.2f ", A[i*cols + j]);
      printf("\n");
   }
}

void Print_Vector(float x[], int rows) {
   int   i;

   for (i = 0; i < rows; i++)
      printf("%.5f ", x[i]*1000);
   printf("\n");
} 

void *_math_vec_prod(void* rank) {
   	int whoami = *(int *) rank;
    int from = whoami * M / NUM_OF_THREADS;
    int to = from + (M / NUM_OF_THREADS) - 1;
    int i, j;
	for (i = from; i <= to; i++) {
	  y[i] = 0.0f;
	  for (j = 0; j < M; j++)
	      y[i] += A[i*M+j]*x[j];
	}
   return NULL;
} 

void _math_vec_prod(void)
{
	int i,j;
	for(i = 0; i < M; i++)
	{
		y_serial[i] = 0.0f;
		for(j = 0; j < N; j++)
			y_serial[i] += A[i * N + j] * x[j];
	}
}