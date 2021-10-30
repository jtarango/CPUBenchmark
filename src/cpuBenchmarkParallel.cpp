#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#ifdef _WIN32
#include <sys/timeb.h>
#include <windows.h>
#else

#include <sys/time.h>
#include <unistd.h>

#endif

#include <cmath>
#include <cstdlib>
#include <pthread.h>
#include<errno.h>

#define MAX_THREADS 32
#define CHAR_BUFFER_SIZE 1024
#define PATH_MAX 4096
volatile size_t DATASET_SIZE = 4294967291; // 100000007;
const char filenameCPUData[] = "cpu_benchmark.csv"; // Random self generation file name.
FILE *writingFileContext = (FILE *) calloc(1, sizeof(FILE));

uint32_t getNumCores(void) {
  uint32_t coreCount;
#ifdef _WIN32
  SYSTEM_INFO sysinfo;
  GetSystemInfo(&sysinfo);
  coreCount = sysinfo.dwNumberOfProcessors;
#else
  coreCount = sysconf(_SC_NPROCESSORS_ONLN);
#endif
  return
}

int printFullPath(const char *partialPath) {
  int rc = 0;
  char *fullPath = (char *) calloc(PATH_MAX, sizeof(char));
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
  } else {
    printf("Path, %s, %s \n", partialPath, fullPath);
  }
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
  return (double) tv.tv_sec + (0.000001 * (double) tv.tv_usec);
# endif
}

template<typename Type>
void my_test(const char *name) {
  double t1;
  volatile Type v_add, v_sub, v_subadd, v_mul, v_div, v_muldiv, v_sqsqrtmul = 0;
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
    v0 = (Type) (rand() % modSize) / divSize + 1;
  }
  while (v1 == 0) {
    v1 = (Type) (rand() % modSize) / divSize + 1;
  }
  while (v2 == 0) {
    v2 = (Type) (rand() % modSize) / divSize + 1;
  }
  while (v3 == 0) {
    v3 = (Type) (rand() % modSize) / divSize + 1;
  }
  while (v4 == 0) {
    v4 = (Type) (rand() % modSize) / divSize + 1;
  }
  while (v5 == 0) {
    v5 = (Type) (rand() % modSize) / divSize + 1;
  }
  while (v6 == 0) {
    v6 = (Type) (rand() % modSize) / divSize + 1;
  }
  while (v7 == 0) {
    v7 = (Type) (rand() % modSize) / divSize + 1;
  }
  while (v8 == 0) {
    v8 = (Type) (rand() % modSize) / divSize + 1;
  }
  while (v9 == 0) {
    v9 = (Type) (rand() % modSize) / divSize + 1;
  }

  // Addition
  t1 = mygettime();
  for (volatile size_t i = 0; i < DATASET_SIZE; ++i) {
    v_add += v9;
    v_add += v0;
    v_add += v1;
    v_add += v2;
    v_add += v3;
    v_add += v4;
    v_add += v5;
    v_add += v6;
    v_add += v7;
    v_add += v8;
    v_add += v9;
  }
  // Pretend we make use of v so compiler doesn't optimize out
  //  the loop completely
  printf("%s, add, %f, [%d]\n", name, mygettime() - t1, (int) v_add & 1);
  fprintf(writingFileContext, "%s, add, %f, %llu, [%d]\n", name, mygettime() - t1,
          (unsigned long long int) (DATASET_SIZE * 10), (int) v_add & 1);

  // Subtraction
  t1 = mygettime();
  for (volatile size_t i = 0; i < DATASET_SIZE; ++i) {
    v_sub -= v9;
    v_sub -= v0;
    v_sub -= v1;
    v_sub -= v2;
    v_sub -= v3;
    v_sub -= v4;
    v_sub -= v5;
    v_sub -= v6;
    v_sub -= v7;
    v_sub -= v8;
    v_sub -= v9;
  }
  // Pretend we make use of v so compiler doesn't optimize out
  //  the loop completely
  printf("%s, sub, %f, [%d]\n", name, mygettime() - t1, (int) v_sub & 1);
  fprintf(writingFileContext, "%s, sub, %f, %llu, [%d]\n", name, mygettime() - t1,
          (unsigned long long int) (DATASET_SIZE * 10), (int) v_sub & 1);

  // Addition/Subtraction
  t1 = mygettime();
  for (volatile size_t i = 0; i < DATASET_SIZE; ++i) {
    v_addsub += v9;
    v_addsub += v0;
    v_addsub -= v1;
    v_addsub += v2;
    v_addsub -= v3;
    v_addsub += v4;
    v_addsub -= v5;
    v_addsub += v6;
    v_addsub -= v7;
    v_addsub += v8;
    v_addsub -= v9;
  }
  // Pretend we make use of v so compiler doesn't optimize out
  //  the loop completely
  printf("%s, add/sub, %f, [%d]\n", name, mygettime() - t1, (int) v_addsub & 1);
  fprintf(writingFileContext, "%s, add/sub, %f, %llu, [%d]\n", name, mygettime() - t1,
          (unsigned long long int) (DATASET_SIZE * 10), (int) v_addsub & 1);

  // Multiply
  t1 = mygettime();
  for (volatile size_t i = 0; i < DATASET_SIZE; ++i) {
    v_mul *= v9;
    v_mul *= v0;
    v_mul *= v1;
    v_mul *= v2;
    v_mul *= v3;
    v_mul *= v4;
    v_mul *= v5;
    v_mul *= v6;
    v_mul *= v7;
    v_mul *= v8;
    v_mul *= v9;
  }
  // Pretend we make use of v so compiler doesn't optimize out
  //  the loop completely
  printf("%s, mul, %f, [%d]\n", name, mygettime() - t1, (int) v_mul & 1);
  fprintf(writingFileContext, "%s, mul, %f, %llu, [%d]\n", name, mygettime() - t1,
          (unsigned long long int) (DATASET_SIZE * 10), (int) v_mul & 1);

  // Divide
  t1 = mygettime();
  for (volatile size_t i = 0; i < DATASET_SIZE; ++i) {
    v_div /= v9;
    v_div /= v0;
    v_div /= v1;
    v_div /= v2;
    v_div /= v3;
    v_div /= v4;
    v_div /= v5;
    v_div /= v6;
    v_div /= v7;
    v_div /= v8;
    v_div /= v9;
  }
  // Pretend we make use of v so compiler doesn't optimize out
  //  the loop completely
  printf("%s, div, %f, [%d]\n", name, mygettime() - t1, (int) v_div & 1);
  fprintf(writingFileContext, "%s, div, %f, %llu, [%d]\n", name, mygettime() - t1,
          (unsigned long long int) (DATASET_SIZE * 10), (int) v_div & 1);

  // Multiply/Divide
  t1 = mygettime();
  for (volatile size_t i = 0; i < DATASET_SIZE; ++i) {
    v_muldiv *= v9;
    v_muldiv *= v0;
    v_muldiv /= v1;
    v_muldiv *= v2;
    v_muldiv /= v3;
    v_muldiv *= v4;
    v_muldiv /= v5;
    v_muldiv *= v6;
    v_muldiv /= v7;
    v_muldiv *= v8;
    v_muldiv /= v9;
  }
  // Pretend we make use of v so compiler doesn't optimize out
  //  the loop completely
  printf("%s, mul/div, %f, [%d]\n", name, mygettime() - t1, (int) v_muldiv & 1);
  fprintf(writingFileContext, "%s, mul/div, %f, %llu, [%d]\n", name, mygettime() - t1,
          (unsigned long long int) (DATASET_SIZE * 10), (int) v_muldiv & 1);

  // Square/SquareRoot/Multiply
  t1 = mygettime();
  for (volatile size_t i = 0; i < DATASET_SIZE; ++i) {
    v_sqsqrtmul *= sqrt(v9 * v9);
    v_sqsqrtmul *= sqrt(v0 * v0);
    v_sqsqrtmul *= sqrt(v1 * v1);
    v_sqsqrtmul *= sqrt(v2 * v2);
    v_sqsqrtmul *= sqrt(v3 * v3);
    v_sqsqrtmul *= sqrt(v4 * v4);
    v_sqsqrtmul *= sqrt(v5 * v5);
    v_sqsqrtmul *= sqrt(v6 * v6);
    v_sqsqrtmul *= sqrt(v7 * v7);
    v_sqsqrtmul *= sqrt(v8 * v8);
    v_sqsqrtmul *= sqrt(v9 * v9);
  }
  // Pretend we make use of v so compiler doesn't optimize out
  //  the loop completely
  printf("%s, mul/sqrt/sq, %f, [%d]\n", name, mygettime() - t1, (int) v_sqsqrtmul & 1);
  fprintf(writingFileContext, "%s, sq/sqrt/mul, %f, %llu, [%d]\n", name, mygettime() - t1,
          (unsigned long long int) (DATASET_SIZE * 10 * 3), (int) v_sqsqrtmul & 1);
}

// Function pointer alias
typedef void *(*func_ptr)(void *);

// Function pointer list
func_ptr myTestFuncs[] = {
  my_test<volatile signed char>("signed char"),
  my_test<volatile unsigned char>("unsigned char"),
  my_test<volatile signed short>("signed short"),
  my_test<volatile unsigned short>("unsigned short"),
  my_test<volatile signed int>("signed int"),
  my_test<volatile unsigned int>("unsigned int"),
  my_test<volatile signed long>("signed long"),
  my_test<volatile unsigned long>("unsigned long"),
  my_test<volatile signed long long>("signed long long"),
  my_test<volatile unsigned long long>("unsigned long long"),
  my_test<volatile float>("float"),
  my_test<volatile double>("double"),
  my_test<volatile long double>("long double")};

int main() {
  uint32_t functionPointerListSize = sizeof(myTestFuncs) / sizeof(myTestFuncs[0]);
  pthread_t threadContext = new pthread_t[functionPointerListSize];
  size_t threadID = new size_t[functionPointerListSize];
  vector <size_t> notStartedQueue, inProgressQueue, completedQueue, errorQueue;
  uint32_t coreCount = getNumCores();
  uint32_t activeThreadCount = 0;
  uint32_t threadStatus;

  // Simple delete of old file.
  writingFileContext = fopen(filenameCPUData, "w");
  fprintf(writingFileContext, "\n");
  fclose(writingFileContext);
  // Create new file to append data.
  writingFileContext = fopen(filenameCPUData, "a+");
  fprintf(writingFileContext,
          "Type, Operation Set, Time for Operations, Count of Operations Performed, Random Last Bit of Computation Chain\n");

  // Prepare queue with function pointer index
  for (size_t i = 0; i < functionPointerListSize; i++) {
    threadID[i] = i;
    notStartedQueue.push_back(i);
  }

  // Loop to continue processing while threads are moved from "not started" to "in progress" then "completed".
  while (false == notStartedQueue.empty() || false == inProgressQueue.empty()) {
    // Prepare only if "not started queue" is not empty.
    if (false == notStartedQueue.empty()) {
      // Try to execute each queue item once then move on to checking inProgress.
      notStartedSize = notStartedQueue.size();
      // Prepare thread to be moved from notStarted to inProgress.
      while ((notStartedSize > 0) && (activeThreadCount < coreCount)) {
        threadIndex = notStartedQueue.pop_front();
        threadStatus = pthread_create(&threadContext[threadIndex],
                                      NULL,
                                      myTestFuncs[threadIndex],
                                      &threadID[threadIndex]);
        if (0 == threadStatus) {
          inProgressQueue.push_back(threadIndex)
          threadCount++;
        } else if (EINVAL == threadStatus || EPERM == threadStatus) {
          perror("Cannot create thread %d: invalid setting or permission.", threadIndex);
          errorQueue.push_front(threadIndex);
        } else { // (threadStatus == EAGAIN
          printf("Cannot create thread %d: resource limited.", threadIndex);
          notStartedQueue.push_back(threadIndex);
        }
      }
      notStartedSize--;
    }

    // Wait for some time so threads cann to do work...
    sleep(32);

    // Verify the status of an "in progress" thread and if done move to complete.
    if (false == inProgressQueue.empty()) {
      inProgressSize = inProgressQueue.size();
      while (inProgressSize > 0) {
        threadIndex = inProgressQueue.pop_front();
        threadStatus == pthread_tryjoin_np(inProgress[j], NULL)
        if (EBUSY == threadStatus) {
          inProgressQueue.push_back(threadIndex);
        } else if (EINVAL == threadStatus || ETIMEDOUT == threadStatus) {
          perror("Thread %d check status timeout or invalid.", threadIndex);
          errorQueue.push_front(threadIndex);
        } else {
          printf("Thread %d complete.", threadIndex);
          completedQueue.push_back(threadIndex)
        }
        inProgressSize--;
      }
    }
  }

  // Waits for threads to complete
  for (size_t i = 0; i < functionPointerListSize; i++) {
    pthread_join(threadContext[i], NULL);
  }
  printFullPath(filenameCPUData);
  fclose(writingFileContext);

  delete[] threadContext;
  delete[] threadID;
  return 0;
}
