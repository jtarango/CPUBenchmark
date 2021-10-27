#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#ifdef _WIN32
#include <sys/timeb.h>
#else
#include <sys/time.h>
#endif
#include <cmath>
#include <time.h>
#include <cstdlib>

#define CHAR_BUFFER_SIZE 1024
#define PATH_MAX 4096
volatile size_t DATASET_SIZE = 4294967291; // 100000007;
const char filenameCPUData[] = "cpu_benchmark.csv"; // Random self generation file name.
FILE *writingFileContext = (FILE *)calloc(1, sizeof(FILE));

int printFullPath(const char * partialPath)
{
	int rc = 0;
	char *fullPath = (char *)calloc(PATH_MAX, sizeof(char));
#ifdef _WIN32
	if (_fullpath(fullPath, partialPath, PATH_MAX) != NULL) {
		printf("Path, %s, %s\n", partialPath, fullPath);
	}
	else {
		rc = -1;
		printf("Path Error, %s\n", partialPath);

	}
#else
	if (realpath(partialPath, fullPath) == 0) {
		rc = -1;
		printf("Path Error, %s\n", partialPath);
	}
	else {
		printf("Path, %s, %s \n", partialPath, fullPath);
	}
	return(rc);
#endif
	return rc;
}


double mygettime(void) {
# ifdef _WIN32
	struct _timeb tb;
	_ftime(&tb);
	return (double)tb.time + (0.001 * (double)tb.millitm);
# else
	struct timeval tv;
	if (gettimeofday(&tv, 0) < 0) {
		perror("oops");
	}
	return (double)tv.tv_sec + (0.000001 * (double)tv.tv_usec);
# endif
}

template< typename Type >
void my_test(const char* name) {
	double t1;
	volatile Type v = 0;
	// Do not use constants or repeating values
	//  to avoid loop unroll optimizations.
	// All values >0 to avoid division by 0
	// Perform ten ops/iteration to reduce
	//  impact of ++i below on measurements
	volatile long int modSize = 256;
	volatile long int divSize = 16;
	volatile Type v0 = 0;
	volatile Type v1 = 0;
	volatile Type v2 = 0;
	volatile Type v3 = 0;
	volatile Type v4 = 0;
	volatile Type v5 = 0;
	volatile Type v6 = 0;
	volatile Type v7 = 0;
	volatile Type v8 = 0;
	volatile Type v9 = 0;

	while (v0 == 0) {
		v0 = (Type)(rand() % modSize) / divSize + 1;
	}
	while (v1 == 0) {
		v1 = (Type)(rand() % modSize) / divSize + 1;
	}
	while (v2 == 0) {
		v2 = (Type)(rand() % modSize) / divSize + 1;
	}
	while (v3 == 0) {
		v3 = (Type)(rand() % modSize) / divSize + 1;
	}
	while (v4 == 0) {
		v4 = (Type)(rand() % modSize) / divSize + 1;
	}
	while (v5 == 0) {
		v5 = (Type)(rand() % modSize) / divSize + 1;
	}
	while (v6 == 0) {
		v6 = (Type)(rand() % modSize) / divSize + 1;
	}
	while (v7 == 0) {
		v7 = (Type)(rand() % modSize) / divSize + 1;
	}
	while (v8 == 0) {
		v8 = (Type)(rand() % modSize) / divSize + 1;
	}
	while (v9 == 0) {
		v9 = (Type)(rand() % modSize) / divSize + 1;
	}

	// Addition
	t1 = mygettime();
	for (volatile size_t i = 0; i < DATASET_SIZE; ++i) {
		v += v0;
		v += v1;
		v += v2;
		v += v3;
		v += v4;
		v += v5;
		v += v6;
		v += v7;
		v += v8;
		v += v9;
	}
	// Pretend we make use of v so compiler doesn't optimize out
	//  the loop completely
	printf("%s, add, %f, [%d]\n", name, mygettime() - t1, (int)v & 1);
	fprintf(writingFileContext, "%s, add, %f, %llu, [%d]\n", name, mygettime() - t1, (unsigned long long int) (DATASET_SIZE * 10), (int)v & 1);

	// Subtraction
	t1 = mygettime();
	for (volatile size_t i = 0; i < DATASET_SIZE; ++i) {
		v -= v0;
		v -= v1;
		v -= v2;
		v -= v3;
		v -= v4;
		v -= v5;
		v -= v6;
		v -= v7;
		v -= v8;
		v -= v9;
	}
	// Pretend we make use of v so compiler doesn't optimize out
	//  the loop completely
	printf("%s, sub, %f, [%d]\n", name, mygettime() - t1, (int)v & 1);
	fprintf(writingFileContext, "%s, sub, %f, %llu, [%d]\n", name, mygettime() - t1, (unsigned long long int) (DATASET_SIZE * 10), (int)v & 1);

	// Addition/Subtraction
	t1 = mygettime();
	for (volatile size_t i = 0; i < DATASET_SIZE; ++i) {
		v += v0;
		v -= v1;
		v += v2;
		v -= v3;
		v += v4;
		v -= v5;
		v += v6;
		v -= v7;
		v += v8;
		v -= v9;
	}
	// Pretend we make use of v so compiler doesn't optimize out
	//  the loop completely
	printf("%s, add/sub, %f, [%d]\n", name, mygettime() - t1, (int)v & 1);
	fprintf(writingFileContext, "%s, add/sub, %f, %llu, [%d]\n", name, mygettime() - t1, (unsigned long long int) (DATASET_SIZE * 10), (int)v & 1);

	// Multiply
	t1 = mygettime();
	for (volatile size_t i = 0; i < DATASET_SIZE; ++i) {
		v *= v0;
		v *= v1;
		v *= v2;
		v *= v3;
		v *= v4;
		v *= v5;
		v *= v6;
		v *= v7;
		v *= v8;
		v *= v9;
	}
	// Pretend we make use of v so compiler doesn't optimize out
    //  the loop completely
	printf("%s, mul, %f, [%d]\n", name, mygettime() - t1, (int)v & 1);
	fprintf(writingFileContext, "%s, mul, %f, %llu, [%d]\n", name, mygettime() - t1, (unsigned long long int) (DATASET_SIZE * 10), (int)v & 1);

	// Divide
	t1 = mygettime();
	for (volatile size_t i = 0; i < DATASET_SIZE; ++i) {
		v /= v0;
		v /= v1;
		v /= v2;
		v /= v3;
		v /= v4;
		v /= v5;
		v /= v6;
		v /= v7;
		v /= v8;
		v /= v9;
	}
	// Pretend we make use of v so compiler doesn't optimize out
	//  the loop completely
	printf("%s, div, %f, [%d]\n", name, mygettime() - t1, (int)v & 1);
	fprintf(writingFileContext, "%s, div, %f, %llu, [%d]\n", name, mygettime() - t1, (unsigned long long int) (DATASET_SIZE * 10), (int)v & 1);

	// Multiply/Divide
	t1 = mygettime();
	for (volatile size_t i = 0; i < DATASET_SIZE; ++i) {
		v *= v0;
		v /= v1;
		v *= v2;
		v /= v3;
		v *= v4;
		v /= v5;
		v *= v6;
		v /= v7;
		v *= v8;
		v /= v9;
	}
	// Pretend we make use of v so compiler doesn't optimize out
	//  the loop completely
	printf("%s, mul/div, %f, [%d]\n", name, mygettime() - t1, (int)v & 1);
	fprintf(writingFileContext, "%s, mul/div, %f, %llu, [%d]\n", name, mygettime() - t1, (unsigned long long int) (DATASET_SIZE * 10), (int)v & 1);

	// Square/SquareRoot/Multiply
	t1 = mygettime();
	for (volatile size_t i = 0; i < DATASET_SIZE; ++i) {
		v *= sqrt(v0*v0);
		v *= sqrt(v1*v1);
		v *= sqrt(v2*v2);
		v *= sqrt(v3*v3);
		v *= sqrt(v4*v4);
		v *= sqrt(v5*v5);
		v *= sqrt(v6*v6);
		v *= sqrt(v7*v7);
		v *= sqrt(v8*v8);
		v *= sqrt(v9*v9);
	}
	// Pretend we make use of v so compiler doesn't optimize out
	//  the loop completely
	printf("%s, mul/sqrt/sq, %f, [%d]\n", name, mygettime() - t1, (int)v & 1);
	fprintf(writingFileContext, "%s, sq/sqrt/mul, %f, %llu, [%d]\n", name, mygettime() - t1, (unsigned long long int) (DATASET_SIZE * 10 * 3), (int)v & 1);
}

int main() {
	// Simple delete of old file.
	writingFileContext = fopen(filenameCPUData, "w");
	fprintf(writingFileContext, "\n");
	fclose(writingFileContext);
	// Create new file to append data.
	writingFileContext = fopen(filenameCPUData, "a+");
	fprintf(writingFileContext, "Type, Operation Set, Time for Operations, Count of Operations Performed, Random Last Bit of Computation Chain\n");
	for (volatile size_t i = 0; i < 64; i++) {
		my_test< volatile signed char >("signed char");
		my_test< volatile unsigned char >("unsigned char");
		my_test< volatile signed short >("signed short");
		my_test< volatile unsigned short >("unsigned short");
		my_test< volatile signed int >("signed int");
		my_test< volatile unsigned int >("unsigned int");
		my_test< volatile signed long >("signed long");
		my_test< volatile unsigned long >("unsigned long");
		my_test< volatile signed long long >("signed long long");
		my_test< volatile unsigned long long >("unsigned long long");
		my_test< volatile float >("float");
		my_test< volatile double >("double");
		my_test< volatile long double >("long double");
	}
	printFullPath(filenameCPUData);
	fclose(writingFileContext);
	return 0;
}
