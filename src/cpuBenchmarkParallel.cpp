/*
 * Written by Joseph Tarango. The original work was to develop a dynamic data
 * type for precision related code in embedded processors. Joseph
 * Tarango webpages can be found at http://www.josephtarango.com
 *
 *THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 *AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
 *THE CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 *EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 *OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 *WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 *OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 *ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 =============================================================================*/
// #pragma once // Only used in header files.

#ifndef __cplusplus
extern "C++" {
#endif // __cplusplus

#ifndef _CPUBENCHMARKPARALLEL_CPP_
#define _CPUBENCHMARKPARALLEL_CPP_

// Ignore data flow analysis is to complex in IDE
#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCDFAInspection"

// @note uncomment to disable assert()
// #define NDEBUG

#include <cassert>
#include <cmath>
#include <cstdlib>
#include <cstdint>
#include <cinttypes>
#include <errno.h>
#include <iostream>
#include <limits.h>
#include <pthread.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <typeinfo>
#include <type_traits>
#include <vector>

#define __STDC_LIMIT_MACROS

#if defined(_WIN32) | defined(_WIN64)
#include <sys/timeb.h>
#include <windows.h>
#else //  !(defined(_WIN32) | defined(_WIN64))

#include <sys/time.h>
#include <unistd.h>

#endif // defined(_WIN32) | defined(_WIN64)

#define ENABLE_DEBUG 0
#define CHAR_BUFFER_SIZE 1024
#define PATH_MAX 4096

// Use (void) to silent unused warnings.
#define assertm(exp, msg) assert(((void)msg, exp))

#define asserterror() fprintf(stderr, "Error in %s at line %d with %s.\n" \
"PRETTY_FUNCTION=%s\n" \
"CONTEXT=%s.\n", \
__FILE__, \
__LINE__, \
__func__, \
__PRETTY_FUNCTION__, \
strerror(errno))

#define asserterrorthread(tid) fprintf(stderr, "Error in %s at line %d with %s.\n" \
"PRETTY_FUNCTION=%s\n" \
"CONTEXT=%s.\n" \
"Cannot create thread %ld: invalid setting or permission.", \
__FILE__, \
__LINE__, \
__func__, \
__PRETTY_FUNCTION__, \
strerror(errno), \
  (uint64_t)tid) \

#define OPERANDS_2_IN 2
#define RESULTANTS_1_OUT 1
#define ENABLE_BASIC_C_ALLOC 0

/* Random Method Selection
 * 1=RAND based random number generator.
 * 2=Exploit the Central Limit Theorem (law of large numbers) and add up several uniformly-distributed random numbers.
 * 3=Use a method described by Abramowitz and Stegun.
 * 4=Use a method discussed in Knuth and due originally to Marsaglia.
*/
#define RANDOM_METHOD 4 // Random Method to select

/*======================================================================================================================
 * Data structures
 * ===================================================================================================================*/
typedef enum fileState_e {
  fs_Unknown_vet = 0,
  fs_Found_vet = (1 << 1),
  fs_NotFound_vet = (1 << 2),
  fs_CanBeRead_vet = (1 << 3),
  fs_CannotBeRead_vet = (1 << 4),
  fs_CanBeEdited_vet = (1 << 5),
  fs_CannotBeEdited_vet = (1 << 6),
  fs_IsExecutable_vet = (1 << 7),
  fs_IsNotExecutable_vet = (1 << 8)
} fileState_et;

typedef enum TypeSystemEnumeration_e {
  tse_int8_e = 1,
  tse_uint8_e = 2,
  tse_int16_e = 3,
  tse_uint16_e = 4,
  tse_int32_e = 5,
  tse_uint32_e = 6,
  tse_int64_e = 7,
  tse_uint64_e = 8,
  tse_float_e = 9,
  tse_double_e = 10,
  tse_long_double_e = 11,
  tse_unknown_e = 12
} TypeSystemEnumeration_t;

typedef union dynamicCompact {
  int8_t int8_data;
  uint8_t uint8_data;
  int16_t int16_data;
  uint16_t uint16_data;
  int32_t int32_data;
  uint32_t uint32_data;
  int64_t int64_data;
  uint64_t uint64_data;
  float float_data;
  double double_data;
  long double longdouble_data;
} dynamicCompact_t;

typedef struct threadContextMeta {
  size_t loopSetSize; // Iterations of arithmetic
  uint16_t threadTag; // Thread identification
  uint8_t isExecuting; // Debug flag
  char *saveFilename; // File name to save
  FILE *saveFileContext; // File object
  char *datatypeIDName; // Saved type name
  TypeSystemEnumeration_t typeSystemName; // Type system enum selection
  dynamicCompact_t operandsMeta[OPERANDS_2_IN]; // Operands
  dynamicCompact_t resultantsMeta; // Resultants.
  char *messages; // Extra messages (optional)

  threadContextMeta() {
    this->loopSetSize = 0;
    this->threadTag = 0;
    this->isExecuting = 0;
    this->saveFilename = NULL;
    this->saveFileContext = NULL;
    this->datatypeIDName = NULL;
    this-> typeSystemName = tse_unknown_e;
    memset(this->operandsMeta, 0, sizeof(dynamicCompact_t) * OPERANDS_2_IN);
    memset(&this->resultantsMeta, 0, sizeof(dynamicCompact_t));
    this->messages = NULL;
  }
} threadContextMeta_t;

typedef struct threadContextArray {
  std::vector<threadContextMeta_t> threadContextVectorMeta;
  size_t size;
} threadContextArray_t;

// function pointers for pthreads_create
// Code reads inside out such that *func_ptr is the function declaration.
// func_ptr is a function pointer such that the first void* is the return
// data and takes in the second void*.
typedef void *(*func_ptr)(void *);

/*======================================================================================================================
 * Functions prototypes
 * ===================================================================================================================*/
// Main execution in library or standalone execution mode.
#ifndef LIBRARY_MODE

int main(int argc, char *argv[]);

#elif (LIBRARY_MODE >= 0)
#pragma message("LIBRARY_MODE")
int testharness_CPUBenchmarkParallel_main(int argc, char *argv[]);
#else // LIBRARY_MODE is defined
#pragma message("LIBRARY_MODE DEFAULT")
int testharness_CPUBenchmarkParallel_main(int argc, char *argv[]);
#endif // LIBRARY_MODE

// Helper functions.
template<typename Type>
bool safeAlloc(Type *&address, size_t size);

void showUsage(void);

void printArgs(int argc, char *argv[]);

int32_t printFullPath(const char *partialPath);

uint32_t getNumCores(void);

double getTime(void);

void setCharArray(char content[CHAR_BUFFER_SIZE]);

bool fileDelete(char fileName[CHAR_BUFFER_SIZE]);

bool fileCreate(char fileName[CHAR_BUFFER_SIZE], char headerString[CHAR_BUFFER_SIZE]);

bool fileOverwriteNil(char fileName[CHAR_BUFFER_SIZE]);

fileState_et fileIsFound(const char fileName[CHAR_BUFFER_SIZE]);

fileState_et fileIsReadable(const char fileName[CHAR_BUFFER_SIZE]);

fileState_et fileIsEditable(const char fileName[CHAR_BUFFER_SIZE]);

fileState_et fileIsExecutable(const char fileName[CHAR_BUFFER_SIZE]);

uint32_t fileCheckAll(const char fileName[CHAR_BUFFER_SIZE]);

bool fileUpCurrentDirectory(char absoluteFolderPath[CHAR_BUFFER_SIZE]);

bool fileGetCurrentWorkingDirectory(char absoluteFolderPath[CHAR_BUFFER_SIZE]);

bool fileMakeDirectory(const char absoluteFolderPath[CHAR_BUFFER_SIZE]);

void fileMakeDirectories(const char absoluteFolderPath[CHAR_BUFFER_SIZE]);

int fileDirectoryExists(const char selectPath[CHAR_BUFFER_SIZE]);

int fileGetDirectory(const char selectPath[CHAR_BUFFER_SIZE],
                     char foundDirectory[CHAR_BUFFER_SIZE]);

double getCPUInfoTokenDouble(const char *matchString, uint64_t matchStringSize);

long double getCPUFrequency(void);

// Template functions.
template<typename Type>
void push_front(std::vector<Type> &v, Type val);

template<typename Type>
void push_back(std::vector<Type> &v, Type val);

template<typename Type>
Type pop_front(std::vector<Type> &v);

template<typename Type>
Type pop_back(std::vector<Type> &v);

template<class classType>
classType gauss_rand(int select);

template<class classType>
classType typelessValid(int select, bool debug);

template<class classType>
TypeSystemEnumeration_t typelessClassify(classType inType);

// Pass pointer-to-template-function as function argument.
// Arithmetic Call template method on class template parameters.
// @note typedef template function pointer
template<template<typename> class tFunctor, class classType>
classType performOp(classType a, classType b);

// Arithmetic functions - ISA Arithmetic support
// https://web.archive.org/web/20130929035331/http://download-software.intel.com/sites/default/files/319433-015.pdf
// https://www.felixcloutier.com/x86/
// Supported: +, -, *, /
// Not Supported (yet): square root, reciprocal, exponential, Sine, Cosine, Multiply-Add, Multiply-Subtract
template<class classType>
classType typelessSubtraction(classType u, classType v);

template<class classType>
classType typelessAddition(classType u, classType v);

template<class classType>
classType typelessMultiplication(classType u, classType v);

template<class classType>
classType typelessDivision(classType u, classType v);

// Pass pointer-to-template-function as function argument
// Arithmetic Print Call template method on class template parameters
template<template<typename> class tPFunctor, class classType>
classType performPrint(classType inA, classType inB, classType outR, const char operationName[CHAR_BUFFER_SIZE],
                       FILE *writeFileContext, long double timeDelta, size_t loopIterations);

// Print function for Arithmetic
template<class classType>
void typelessStringName(classType inA, char printBuffer[CHAR_BUFFER_SIZE], bool printName);

template<class classType>
classType typelessPrint(classType inA, classType inB, classType outR, const char operationName[CHAR_BUFFER_SIZE],
                        FILE *writeFileContext, long double timeDelta, size_t loopIterations);

// Tests
void *testTypes_Template_Pthread(void *inArgs);

template<typename Type>
void testTypes_Template_typeless(Type inA, Type inB, FILE *fileContext, size_t dataSetsSize);

/*======================================================================================================================
 * Pthread generic struct definitions and prototypes for usage in arithmetic
 * ===================================================================================================================*/
bool threadContextMeta_init(threadContextMeta_t *&threadContextData);

template<typename Type>
bool threadContextMeta_set(threadContextMeta_t *&threadContextData,
                           size_t loopSetSize,
                           uint16_t threadTag,
                           uint8_t isExecuting,
                           char saveFilename[CHAR_BUFFER_SIZE],
                           FILE *saveFileContext,
                           Type operandsMeta[OPERANDS_2_IN],
                           size_t operandsMetaSize,
                           Type *resultantsMeta,
                           size_t resultantsMetaSize,
                           char messages[CHAR_BUFFER_SIZE],
                           const std::string fileHeader);

bool threadContextArray_init(threadContextArray_t *&threadContextArrayData,
                             size_t reserveSize);

template<typename Type>
bool testTypes_Template_Pthread_init(threadContextArray_t *&threadVector,
                                     size_t indexThread,
                                     size_t dataSetSize);

/*======================================================================================================================
 * Function definition and implementation
 * ===================================================================================================================*/
template<class classType>
classType typelessSubtraction(classType u, classType v) {
  return u - v;
}

template<class classType>
classType typelessAddition(classType u, classType v) {
  return u + v;
}

template<class classType>
classType typelessMultiplication(classType u, classType v) {
  return u * v;
}

template<class classType>
classType typelessDivision(classType u, classType v) {
  if (u == 0 || v == 0) {
    return u;
  }
  return u / v;
}

template<class classType>
struct tSubtract {
  classType operator()(classType a, classType b) {
    return typelessSubtraction<classType>(a, b);
  }
};

template<class classType>
struct tAddition {
  classType operator()(classType a, classType b) {
    return typelessAddition<classType>(a, b);
  }
};

template<class classType>
struct tMultiplication {
  classType operator()(classType a, classType b) {
    return typelessMultiplication<classType>(a, b);
  }
};

template<class classType>
struct tDivision {
  classType operator()(classType a, classType b) {
    return typelessDivision<classType>(a, b);
  }
};

template<class classType>
struct tPrint {
  classType operator()(classType inA, classType inB, classType outR,
                       const char operationName[CHAR_BUFFER_SIZE],
                       FILE *writeFileContext,
                       long double timeDelta,
                       size_t loopIterations) {
    return typelessPrint<classType>(inA, inB, outR, operationName, writeFileContext, timeDelta, loopIterations);
  }
};

/******************************************************************************
*
* @return
*****************************************************************************/
template<template<typename> class tFunctor, class classType>
classType performOp(classType a, classType b) {
  // Equivalent to this:
  // tFunctor<classType> functor;
  // return functor(a, b);
  return tFunctor<classType>()(a, b);
}

/******************************************************************************
*
* @return
*****************************************************************************/
template<template<typename> class tPFunctor, class classType>
classType performPrint(classType inA, classType inB, classType outR, const char operationName[CHAR_BUFFER_SIZE],
                       FILE *writeFileContext, long double timeDelta, size_t loopIterations) {
  // Equivalent to this:
  // tPFunctor<classType> functor;
  // return functor(inA, inB, outR, operationName);
  return tPFunctor<classType>()(inA, inB, outR, operationName, writeFileContext, timeDelta, loopIterations);
}

/*****************************************************************************
*
* @return
*****************************************************************************/
// Note: using std::enable_if with anonymous type parameters
// template <class classType, std::enable_if_t<!std::is_arithmetic<classType>::value>* = nullptr>
template<class classType>
void typelessStringName(classType inA, char printBuffer[CHAR_BUFFER_SIZE], bool printName) {
  TypeSystemEnumeration_t mt = typelessClassify<classType>(inA);
  /* IEEE-754 Precision of the representation for printing values.
   * A 32-bit, single-precision IEEE754 number has 24 mantissa bits, which gives about 23+1 * log10(2) = 7.22 ~ 8 digits of precision.
   * A 64-bit, double precision IEEE754 number has 53 mantissa bits, which gives about 52+1 * log10(2) = 15.95 ~ 16 digits of precision.
   * A 128-bit, long double precision IEEE754 number has 112 mantissa bits, which gives about 112+1 * log10(2) = 34.01 ~ 35 digits of precision.
   */
  // Header is: Type System, Operation Set Name, Time for Operations, Count of Operations Performed, LHS, RHS, R
  switch (mt) {
    case tse_int8_e:
      snprintf(printBuffer, CHAR_BUFFER_SIZE, "int8");
      break;
    case tse_uint8_e:
      snprintf(printBuffer, CHAR_BUFFER_SIZE, "uint8");
      break;
    case tse_int16_e:
      snprintf(printBuffer, CHAR_BUFFER_SIZE, "int16");
      break;
    case tse_uint16_e:
      snprintf(printBuffer, CHAR_BUFFER_SIZE, "uint16");
      break;
    case tse_int32_e:
      snprintf(printBuffer, CHAR_BUFFER_SIZE, "int32");
      break;
    case tse_uint32_e:
      snprintf(printBuffer, CHAR_BUFFER_SIZE, "uint32");
      break;
    case tse_int64_e:
      snprintf(printBuffer, CHAR_BUFFER_SIZE, "int64");
      break;
    case tse_uint64_e:
      snprintf(printBuffer, CHAR_BUFFER_SIZE, "uint64");
      break;
    case tse_float_e:
      snprintf(printBuffer, CHAR_BUFFER_SIZE, "float");
      break;
    case tse_double_e:
      snprintf(printBuffer, CHAR_BUFFER_SIZE, "double");
      break;
    case tse_long_double_e:
      snprintf(printBuffer, CHAR_BUFFER_SIZE, "longdouble");
      break;
    default:
      // Type System, Operation Set Name, Time for Operations, Count of Operations Performed, LHS, RHS, R
      snprintf(printBuffer, CHAR_BUFFER_SIZE,
               "Type_Unknown_AtLine_%d", __LINE__);
  }
  if (printName) {
    printf("%s\n", printBuffer);
  }
  return;
}

/******************************************************************************
*
* @return
*****************************************************************************/
// Note: using std::enable_if with anonymous type parameters
// template <class classType, std::enable_if_t<!std::is_arithmetic<classType>::value>* = nullptr>
template<class classType>
classType typelessPrint(classType inA, classType inB, classType outR, const char operationName[CHAR_BUFFER_SIZE],
                        FILE *fileContext, long double timeDelta, size_t loopIterations) {
  TypeSystemEnumeration_t mtA = typelessClassify<classType>(inA);
  TypeSystemEnumeration_t mtB = typelessClassify<classType>(inB);
  TypeSystemEnumeration_t mtR = typelessClassify<classType>(outR);
  char printBuffer[CHAR_BUFFER_SIZE];
  bool areAllSameType = ((mtA == mtB) && (mtB == mtR));
  setCharArray(printBuffer);

  if (areAllSameType) {
    /* IEEE-754 Precision of the representation for printing values.
     * A 32-bit, single-precision IEEE754 number has 24 mantissa bits, which gives about 23+1 * log10(2) = 7.22 ~ 8 digits of precision.
     * A 64-bit, double precision IEEE754 number has 53 mantissa bits, which gives about 52+1 * log10(2) = 15.95 ~ 16 digits of precision.
     * A 128-bit, long double precision IEEE754 number has 112 mantissa bits, which gives about 112+1 * log10(2) = 34.01 ~ 35 digits of precision.
     */
    // Header is: Type System, Operation Set Name, Time for Operations, Count of Operations Performed, LHS, RHS, R
    switch (mtR) {
      case tse_int8_e:
        snprintf(printBuffer, CHAR_BUFFER_SIZE, "int8_t, %s, %Lf, %lu, %d, %d, %d",
                 operationName, timeDelta, loopIterations,
                 (int8_t) inA, (int8_t) inB, (int8_t) outR);
        break;
      case tse_uint8_e:
        snprintf(printBuffer, CHAR_BUFFER_SIZE, "uint8_t, %s, %Lf, %lu, %d, %d, %d",
                 operationName, timeDelta, loopIterations,
                 (uint8_t) inA, (uint8_t) inB, (uint8_t) outR);
        break;
      case tse_int16_e:
        snprintf(printBuffer, CHAR_BUFFER_SIZE, "int16_t, %s, %Lf, %lu, %d, %d, %d",
                 operationName, timeDelta, loopIterations,
                 (int16_t) inA, (int16_t) inB, (int16_t) outR);
        break;
      case tse_uint16_e:
        snprintf(printBuffer, CHAR_BUFFER_SIZE, "uint16_t, %s, %Lf, %lu, %d, %d, %d",
                 operationName, timeDelta, loopIterations,
                 (uint16_t) inA, (uint16_t) inB, (uint16_t) outR);
        break;
      case tse_int32_e:
        snprintf(printBuffer, CHAR_BUFFER_SIZE, "int32_t, %s, %Lf, %lu, %d, %d, %d",
                 operationName, timeDelta, loopIterations,
                 (int32_t) inA, (int32_t) inB, (int32_t) outR);
        break;
      case tse_uint32_e:
        snprintf(printBuffer, CHAR_BUFFER_SIZE, "uint32_t, %s, %Lf, %lu, %d, %d, %d",
                 operationName, timeDelta, loopIterations,
                 (uint32_t) inA, (uint32_t) inB, (uint32_t) outR);
        break;
      case tse_int64_e:
        snprintf(printBuffer, CHAR_BUFFER_SIZE, "int64_t, %s, %Lf, %lu, %ld, %ld, %ld",
                 operationName, timeDelta, loopIterations,
                 (int64_t) inA, (int64_t) inB, (int64_t) outR);
        break;
      case tse_uint64_e:
        snprintf(printBuffer, CHAR_BUFFER_SIZE, "uint64_t, %s, %Lf, %lu, %ld, %ld, %ld",
                 operationName, timeDelta, loopIterations,
                 (uint64_t) inA, (uint64_t) inB, (uint64_t) outR);
        break;
      case tse_float_e:
        snprintf(printBuffer, CHAR_BUFFER_SIZE, "float, %s, %Lf, %lu, %.8f, %.8f, %.8f",
                 operationName, timeDelta, loopIterations,
                 (float) inA, (float) inB, (float) outR);
        break;
      case tse_double_e:
        snprintf(printBuffer, CHAR_BUFFER_SIZE, "double, %s, %Lf, %lu, %.16f, %.16f, %.16f",
                 operationName, timeDelta, loopIterations,
                 (double) inA, (double) inB, (double) outR);
        break;
      case tse_long_double_e:
        snprintf(printBuffer, CHAR_BUFFER_SIZE, "long double, %s, %Lf, %lu, %.35Lf, %.35Lf, %.35Lf",
                 operationName, timeDelta, loopIterations,
                 (long double) inA, (long double) inB, (long double) outR);
        break;
      default:
        // Type System, Operation Set Name, Time for Operations, Count of Operations Performed, LHS, RHS, R
        snprintf(printBuffer, CHAR_BUFFER_SIZE,
                 "TypeSystem=unknown_AtLine_%d, OperationSetName=%s, TimeForOperations=%Lf, CountOfOperationsPerformed=%ld, "
                 "LHS=unknown, RHS=unknown, R=unknown",
                 __LINE__, operationName, timeDelta, loopIterations);
        break;
    }
    if (ENABLE_DEBUG) {
      printf("%s\n", printBuffer);
    }
    fprintf(fileContext, "%s\n", printBuffer);
  }
  return outR;
}

/******************************************************************************
*
* @return
*****************************************************************************/
template<typename Type>
void testTypes_Template_typeless(Type inA, Type inB, FILE *fileContext, size_t datasetSize) {
  long double timeStart, timeStop, timeDelta;
  bool isOdd;
  size_t loopIterations = datasetSize;
  Type typelessResult_add, typelessResult_sub, typelessResult_mul, typelessResult_div;

  timeStart = getTime();
  typelessResult_add = performOp<tAddition>(inA, inB);
  for (size_t index = 0; index < loopIterations; index++) {
    isOdd = index & 1;
    if (0 == index) {
      typelessResult_add = performOp<tAddition>(inA, inB);
    } else if (isOdd) {
      typelessResult_add = performOp<tAddition>(typelessResult_add, inB);
    } else {
      typelessResult_add = performOp<tAddition>(inA, typelessResult_add);
    }
  }
  timeStop = getTime();
  timeDelta = timeStop - timeStart;
  performPrint<tPrint>(inA, inB, typelessResult_add, "addition", fileContext, timeDelta, loopIterations);

  timeStart = getTime();
  for (size_t index = 0; index < loopIterations; index++) {
    isOdd = index & 1;
    if (0 == index) {
      typelessResult_sub = performOp<tSubtract>(inA, inB);
    } else if (isOdd) {
      typelessResult_sub = performOp<tSubtract>(inA, typelessResult_sub);
    } else {
      typelessResult_sub = performOp<tSubtract>(typelessResult_sub, inB);
    }
  }
  timeStop = getTime();
  timeDelta = timeStop - timeStart;
  performPrint<tPrint>(inA, inB, typelessResult_sub, "subtraction", fileContext, timeDelta, loopIterations);

  timeStart = getTime();
  for (size_t index = 0; index < loopIterations; index++) {
    isOdd = index & 1;
    if (0 == index) {
      typelessResult_mul = performOp<tMultiplication>(inA, inB);
    } else if (isOdd) {
      typelessResult_mul = performOp<tMultiplication>(inA, typelessResult_mul);
    } else {
      typelessResult_mul = performOp<tMultiplication>(typelessResult_mul, inB);
    }
  }
  timeStop = getTime();
  timeDelta = timeStop - timeStart;
  performPrint<tPrint>(inA, inB, typelessResult_mul, "multiplication", fileContext, timeDelta, loopIterations);

  timeStart = getTime();
  for (size_t index = 0; index < loopIterations; index++) {
    isOdd = index & 1;
    if (0 == index) {
      typelessResult_div = performOp<tDivision>(inA, inB);
    } else if (isOdd) {
      typelessResult_div = performOp<tDivision>(inA, typelessResult_div);
    } else {
      typelessResult_div = performOp<tDivision>(typelessResult_div, inB);
    }
  }
  timeStop = getTime();
  timeDelta = timeStop - timeStart;
  performPrint<tPrint>(inA, inB, typelessResult_div, "division", fileContext, timeDelta, loopIterations);

  return;
}

/******************************************************************************
*
* @return
*****************************************************************************/
template<typename Type>
bool testTypes_Template_Pthread_init(threadContextArray_t *&threadVector, size_t indexThread, size_t dataSetSize) {
  const std::string fileHeader = "Type System, Operation Set Name, Time for Operations, Count of Operations Performed, LHS, RHS, R";
#if (defined(__WIN64__) && defined(__WIN64__))
  const char fileDirectory[] = "\\data\\";
#else // !(defined(__WIN64__) && defined(__WIN64__))
  const char fileDirectory[] = "/data/";
#endif // (defined(__WIN64__) && defined(__WIN64__))
  const char fileNamePrefix[] = "cpuBenchmarkPthreads_";
  const char fileExtension[] = "cvs";// self generation file name.
  char *directoryTree = NULL;
  char *fileNameAbsolute = NULL;
  char *typeNameBuffer = NULL;
  char *messages = NULL;
  Type operandsMeta[OPERANDS_2_IN];
  Type resultantsMeta;
  threadContextMeta_t *threadItem = NULL;
  bool isValid;

  safeAlloc<char>(directoryTree, CHAR_BUFFER_SIZE);
  safeAlloc<char>(fileNameAbsolute, CHAR_BUFFER_SIZE);
  safeAlloc<char>(typeNameBuffer, CHAR_BUFFER_SIZE);
  safeAlloc<char>(messages, CHAR_BUFFER_SIZE);

  // Move up one file directory
  isValid = fileUpCurrentDirectory(directoryTree);

  threadItem = &(threadVector->threadContextVectorMeta[indexThread]);

  do {
    operandsMeta[0] = typelessValid<Type>(RANDOM_METHOD, ENABLE_DEBUG);
  } while (0 == operandsMeta[0]);
  do {
    operandsMeta[1] = typelessValid<Type>(RANDOM_METHOD, ENABLE_DEBUG);
  } while (0 == operandsMeta[1]);
  setCharArray(messages);
  typelessStringName(operandsMeta[0], typeNameBuffer, false);
  isValid = isValid && threadContextMeta_init(threadItem);
  snprintf(fileNameAbsolute, CHAR_BUFFER_SIZE, "%s%s%s%s_thread-%ld_meta.%s",
           directoryTree, fileDirectory, fileNamePrefix, typeNameBuffer, indexThread, fileExtension);

  threadContextMeta_set<Type>(threadItem,
                              dataSetSize,
                              indexThread,
                              0,
                              fileNameAbsolute,
                              operandsMeta,
                              OPERANDS_2_IN,
                              resultantsMeta,
                              RESULTANTS_1_OUT,
                              messages,
                              fileHeader.c_str());
  return isValid;
}

/******************************************************************************
*
* @return
*****************************************************************************/
void *testTypes_Template_Pthread(void *inArgs) {
  void *danglePtr = NULL;
  threadContextMeta_t *threadInfo = (threadContextMeta_t *) inArgs;
  char *inOperandTypeName = NULL;
  threadInfo->isExecuting = 1;
  safeAlloc<char>(inOperandTypeName, CHAR_BUFFER_SIZE);
  size_t inOperandSize = sizeof(typeid(threadInfo->operandsMeta[0]).name());
  strncpy(inOperandTypeName, typeid(threadInfo->operandsMeta[0]).name(), inOperandSize);

  TypeSystemEnumeration_t mt = threadInfo->typeSystemName;
  switch (mt) {
    case tse_int8_e:
      testTypes_Template_typeless<int8_t>(threadInfo->operandsMeta[0].int8_data,
                                          threadInfo->operandsMeta[1].int8_data,
                                          threadInfo->saveFileContext,
                                          threadInfo->loopSetSize);
      break;
    case tse_uint8_e:
      testTypes_Template_typeless<uint8_t>(threadInfo->operandsMeta[0].uint8_data,
                                           threadInfo->operandsMeta[1].uint8_data,
                                           threadInfo->saveFileContext,
                                           threadInfo->loopSetSize);
      break;
    case tse_int16_e:
      testTypes_Template_typeless<int16_t>(threadInfo->operandsMeta[0].int16_data,
                                           threadInfo->operandsMeta[1].int16_data,
                                           threadInfo->saveFileContext,
                                           threadInfo->loopSetSize);
      break;
    case tse_uint16_e:
      testTypes_Template_typeless<uint16_t>(threadInfo->operandsMeta[0].uint16_data,
                                            threadInfo->operandsMeta[1].uint16_data,
                                            threadInfo->saveFileContext,
                                            threadInfo->loopSetSize);
      break;
    case tse_int32_e:
      testTypes_Template_typeless<int32_t>(threadInfo->operandsMeta[0].int32_data,
                                           threadInfo->operandsMeta[1].int32_data,
                                           threadInfo->saveFileContext,
                                           threadInfo->loopSetSize);
      break;
    case tse_uint32_e:
      testTypes_Template_typeless<uint32_t>(threadInfo->operandsMeta[0].uint32_data,
                                            threadInfo->operandsMeta[1].uint32_data,
                                            threadInfo->saveFileContext,
                                            threadInfo->loopSetSize);
      break;
    case tse_int64_e:
      testTypes_Template_typeless<int64_t>(threadInfo->operandsMeta[0].int64_data,
                                           threadInfo->operandsMeta[1].int64_data,
                                           threadInfo->saveFileContext,
                                           threadInfo->loopSetSize);
      break;
    case tse_uint64_e:
      testTypes_Template_typeless<uint64_t>(threadInfo->operandsMeta[0].uint64_data,
                                            threadInfo->operandsMeta[1].uint64_data,
                                            threadInfo->saveFileContext,
                                            threadInfo->loopSetSize);
      break;
    case tse_float_e:
      testTypes_Template_typeless<float>(threadInfo->operandsMeta[0].float_data,
                                         threadInfo->operandsMeta[1].float_data,
                                         threadInfo->saveFileContext,
                                         threadInfo->loopSetSize);
      break;
    case tse_double_e:
      testTypes_Template_typeless<double>(threadInfo->operandsMeta[0].double_data,
                                          threadInfo->operandsMeta[1].double_data,
                                          threadInfo->saveFileContext,
                                          threadInfo->loopSetSize);
      break;
    case tse_long_double_e:
      testTypes_Template_typeless<long double>(threadInfo->operandsMeta[0].longdouble_data,
                                               threadInfo->operandsMeta[1].longdouble_data,
                                               threadInfo->saveFileContext,
                                               threadInfo->loopSetSize);
      break;
    default:
      asserterrorthread(threadInfo->threadTag);
      break;
  }

  danglePtr = (void *) threadInfo;
  return danglePtr;
}

/******************************************************************************
*
* @return
*****************************************************************************/
bool threadContextMeta_init(threadContextMeta_t *&threadContextData) {
  bool isAllocated = false;
  bool isFilled = false;
  bool isValid;
  if (NULL == threadContextData) {
    safeAlloc<threadContextMeta_t>(threadContextData, 1);
  }
  if (NULL != threadContextData) {
    isAllocated = true;
    threadContextData->loopSetSize = 0;
    threadContextData->threadTag = 0;
    threadContextData->isExecuting = 0;
    threadContextData->saveFilename = NULL;
    threadContextData->saveFileContext = NULL;
    threadContextData->datatypeIDName = NULL;
    threadContextData->typeSystemName = tse_unknown_e;
    memset(threadContextData->operandsMeta, 0, sizeof(dynamicCompact_t) * OPERANDS_2_IN);
    memset(&threadContextData->resultantsMeta, 0, sizeof(dynamicCompact_t));
    threadContextData->messages = NULL;
    isFilled = true;
  }
  isValid = isAllocated && isFilled;
  return isValid;
}

/******************************************************************************
*
* @return
*****************************************************************************/
bool threadContextArray_init(threadContextArray_t *&threadContextArrayData,
                             size_t reserveSize) {
#if DISABLED
  bool isLevel_1_Allocated = false;
  bool isLevel_2_Allocated = false;
  bool isLevel_3_Allocated = false;
  if (NULL == threadContextArrayData) {
    safeAlloc<threadContextArray_t>(threadContextArrayData, 1);
  }
  if (NULL != threadContextArrayData){
    isLevel_1_Allocated = true;
  }

  if (NULL == threadContextArrayData->threadContextVectorMeta){
    safeAlloc(threadContextArrayData->threadContextVectorMeta, reserveSize);
  }
  if (NULL != threadContextArrayData->threadContextVectorMeta) {
    isLevel_2_Allocated = true;
  }

  threadContextArrayData->size = reserveSize;

  if (isLevel_1_Allocated && isLevel_2_Allocated) {
    for (size_t i = 0; i < reserveSize; i++){
      threadContextMeta_t *threadContextPtr = &threadContextArrayData->threadContextVectorMeta[i];
      if (0==i){
        isLevel_3_Allocated =  threadContextMeta_init(threadContextPtr);
      }
      else {
        isLevel_3_Allocated = isLevel_3_Allocated && threadContextMeta_init(threadContextPtr);
      }
    }
  }
 return isLevel_1_Allocated  && isLevel_2_Allocated && isLevel_3_Allocated;
#endif // DISABLED
  bool isLevel_1_Allocated = false;
  bool isLevel_2_Allocated = false;
  bool isLevel_3_Allocated = false;
  if (NULL == threadContextArrayData) {
    safeAlloc<threadContextArray_t>(threadContextArrayData, 1);
  }
  if (NULL != threadContextArrayData) {
    isLevel_1_Allocated = true;
  }

  if (threadContextArrayData->threadContextVectorMeta.capacity() < reserveSize) {
    threadContextArrayData->threadContextVectorMeta.reserve(reserveSize);
  }
  isLevel_2_Allocated = true;

  threadContextArrayData->size = reserveSize;

  if (isLevel_1_Allocated && isLevel_2_Allocated) {
    for (size_t i = 0; i < reserveSize; i++) {
      threadContextMeta_t *threadContextPtr = &threadContextArrayData->threadContextVectorMeta[i];
      if (0 == i) {
        isLevel_3_Allocated = threadContextMeta_init(threadContextPtr);
      } else {
        isLevel_3_Allocated = isLevel_3_Allocated && threadContextMeta_init(threadContextPtr);
      }
    }
  }
  return isLevel_1_Allocated && isLevel_2_Allocated && isLevel_3_Allocated;
}

/******************************************************************************
*
* @return
*****************************************************************************/
template<typename Type>
bool threadContextMeta_set(threadContextMeta_t *threadContextData,
                           size_t loopSetSize,
                           uint16_t threadTag,
                           uint8_t isExecuting,
                           char saveFilename[CHAR_BUFFER_SIZE],
                           Type operandsMeta[OPERANDS_2_IN],
                           size_t operandsMetaSize,
                           Type resultantsMeta,
                           size_t resultantsMetaSize,
                           char messages[CHAR_BUFFER_SIZE],
                           const std::string fileHeader) {
  Type tmp;
  size_t dataTypeSize;
  bool isAllocated = false;
  char directoryPath[CHAR_BUFFER_SIZE];
  setCharArray(directoryPath);

  if (NULL != threadContextData) {
    threadContextData->loopSetSize = loopSetSize;
    threadContextData->threadTag = threadTag;
    threadContextData->isExecuting = isExecuting;

    if (NULL == threadContextData->saveFilename) {
      threadContextData->saveFilename = NULL;
      safeAlloc<char>(threadContextData->saveFilename, CHAR_BUFFER_SIZE);
    }
    if (threadContextData->saveFilename != NULL) {
      setCharArray(threadContextData->saveFilename);
      strncpy(threadContextData->saveFilename, saveFilename, CHAR_BUFFER_SIZE);
    }

    if (NULL == threadContextData->saveFileContext) {
      if (fs_Found_vet == fileIsFound(threadContextData->saveFilename)) {
        fileDelete(threadContextData->saveFilename);
      } else {
        fileGetDirectory(threadContextData->saveFilename, directoryPath);
        fileMakeDirectories(directoryPath);
      }
      fileOverwriteNil(threadContextData->saveFilename);
      fileCreate(threadContextData->saveFilename, (char *) fileHeader.c_str());
      threadContextData->saveFileContext = fopen(threadContextData->saveFilename, "a+");
      printf("File %s opened in a+ mode.\n", threadContextData->saveFilename);
    } else {
      asserterror();
    }

    // Construct string array
    if (NULL == threadContextData->datatypeIDName) {
      safeAlloc<char>(threadContextData->datatypeIDName, CHAR_BUFFER_SIZE);
    }
    setCharArray(threadContextData->datatypeIDName);
    // Extract name from type
    dataTypeSize = sizeof(typeid(tmp).name());
    strncpy(threadContextData->datatypeIDName, typeid(tmp).name(), dataTypeSize);

    threadContextData->typeSystemName = typelessClassify<Type>(tmp);
    switch (threadContextData->typeSystemName) {
      case tse_int8_e:
        threadContextData->operandsMeta[0].int8_data = operandsMeta[0];
        threadContextData->operandsMeta[1].int8_data = operandsMeta[1];
        threadContextData->resultantsMeta.int8_data = resultantsMeta;
        break;
      case tse_uint8_e:
        threadContextData->operandsMeta[0].uint8_data = operandsMeta[0];
        threadContextData->operandsMeta[1].uint8_data = operandsMeta[1];
        threadContextData->resultantsMeta.uint8_data = resultantsMeta;
        break;
      case tse_int16_e:
        threadContextData->operandsMeta[0].int16_data = operandsMeta[0];
        threadContextData->operandsMeta[1].int16_data = operandsMeta[1];
        threadContextData->resultantsMeta.int16_data = resultantsMeta;
        break;
      case tse_uint16_e:
        threadContextData->operandsMeta[0].uint16_data = operandsMeta[0];
        threadContextData->operandsMeta[1].uint16_data = operandsMeta[1];
        threadContextData->resultantsMeta.uint16_data = resultantsMeta;
        break;
      case tse_int32_e:
        threadContextData->operandsMeta[0].int32_data = operandsMeta[0];
        threadContextData->operandsMeta[1].int32_data = operandsMeta[1];
        threadContextData->resultantsMeta.int32_data = resultantsMeta;
        break;
      case tse_uint32_e:
        threadContextData->operandsMeta[0].uint32_data = operandsMeta[0];
        threadContextData->operandsMeta[1].uint32_data = operandsMeta[1];
        threadContextData->resultantsMeta.uint32_data = resultantsMeta;
        break;
      case tse_int64_e:
        threadContextData->operandsMeta[0].int64_data = operandsMeta[0];
        threadContextData->operandsMeta[1].int64_data = operandsMeta[1];
        threadContextData->resultantsMeta.int64_data = resultantsMeta;
        break;
      case tse_uint64_e:
        threadContextData->operandsMeta[0].uint64_data = operandsMeta[0];
        threadContextData->operandsMeta[1].uint64_data = operandsMeta[1];
        threadContextData->resultantsMeta.uint64_data = resultantsMeta;
        break;
      case tse_float_e:
        threadContextData->operandsMeta[0].float_data = operandsMeta[0];
        threadContextData->operandsMeta[1].float_data = operandsMeta[1];
        threadContextData->resultantsMeta.float_data = resultantsMeta;
        break;
      case tse_double_e:
        threadContextData->operandsMeta[0].double_data = operandsMeta[0];
        threadContextData->operandsMeta[1].double_data = operandsMeta[1];
        threadContextData->resultantsMeta.double_data = resultantsMeta;
        break;
      case tse_long_double_e:
        threadContextData->operandsMeta[0].longdouble_data = operandsMeta[0];
        threadContextData->operandsMeta[1].longdouble_data = operandsMeta[1];
        threadContextData->resultantsMeta.longdouble_data = resultantsMeta;
        break;
      default:
        bool isValid_Numerical = false;
        printf("Operation=unknownType, A=unknown\n");
        asserterrorthread(threadContextData->threadTag);
        assertm(!isValid_Numerical, "Invalid type passing\n");
        break;
    }

    if (NULL == threadContextData->messages) {
      safeAlloc<char>(threadContextData->messages, CHAR_BUFFER_SIZE);
    }
    setCharArray(threadContextData->messages);
    strncpy(threadContextData->messages, messages, CHAR_BUFFER_SIZE);

    isAllocated = true;
  }
  return isAllocated;
}

/******************************************************************************
*
* @return
*****************************************************************************/
#ifndef LIBRARY_MODE

int main(int argc, char *argv[]) {
#elif (LIBRARY_MODE >= 0)
#pragma message("LIBRARY_MODE")
int testharness_CPUBenchmarkParallel_main(int argc, char *argv[]) {
#else // LIBRARY_MODE is defined
#pragma message("LIBRARY_MODE DEFAULT")
int testharness_CPUBenchmarkParallel_main(int argc, char *argv[]) {
#endif // LIBRARY_MODE
  showUsage();
  printArgs(argc, argv);
  const size_t waitTime = 6;
  const size_t oneMinute = (waitTime >= 1) ? (60 / waitTime) : waitTime;
  const size_t testSize = 11;
  volatile size_t dataSetSize = (1 << 30); // Choose 28 to 31 bits. Used to ensure compiler does not optimize.
  std::vector<size_t> notStartedQueue;
  std::vector<size_t> inProgressQueue;
  std::vector<size_t> completedQueue;
  std::vector<size_t> errorQueue;
  size_t waitCount;
  size_t coreCount;
  size_t activeThreadCount;
  size_t threadStatus;
  size_t threadIndex;
  size_t notStartedSize;
  size_t inProgressSize;
  func_ptr myTypelessTestFuncs;
  threadContextArray_t *threadVector;
  pthread_t *threadContext;
  size_t *threadID;

  setvbuf(stdout, NULL, _IONBF, BUFSIZ); // Set buffer size.
  waitCount = 0;
  threadVector = NULL;
  threadContext = NULL;
  threadID = NULL;
  coreCount = getNumCores();
  activeThreadCount = 0;
  printf("Cores for Pthread() usage are: %zu\n", coreCount);
  printf("CPU frequency %Lf KHz\n", getCPUFrequency());

  // Function pointer list
  myTypelessTestFuncs = testTypes_Template_Pthread;
  // Allocate
  threadContextArray_init(threadVector, testSize);
  // Construct and setup
  testTypes_Template_Pthread_init<int8_t>(threadVector, 0, dataSetSize);
  testTypes_Template_Pthread_init<uint8_t>(threadVector, 1, dataSetSize);
  testTypes_Template_Pthread_init<int16_t>(threadVector, 2, dataSetSize);
  testTypes_Template_Pthread_init<uint16_t>(threadVector, 3, dataSetSize);
  testTypes_Template_Pthread_init<int32_t>(threadVector, 4, dataSetSize);
  testTypes_Template_Pthread_init<uint32_t>(threadVector, 5, dataSetSize);
  testTypes_Template_Pthread_init<int64_t>(threadVector, 6, dataSetSize);
  testTypes_Template_Pthread_init<uint64_t>(threadVector, 7, dataSetSize);
  testTypes_Template_Pthread_init<float>(threadVector, 8, dataSetSize);
  testTypes_Template_Pthread_init<double>(threadVector, 9, dataSetSize);
  testTypes_Template_Pthread_init<long double>(threadVector, 10, dataSetSize);

  safeAlloc<pthread_t>(threadContext, testSize);
  safeAlloc<size_t>(threadID, testSize);
  // Prepare queue with function pointer index
  for (size_t i = 0; i < threadVector->size; i++) {
    threadID[i] = threadVector->threadContextVectorMeta[i].threadTag;
    push_back(notStartedQueue, threadID[i]);
  }

  // Loop to continue processing while threads are moved from "not started" to "in progress" then "completed".
  while (!notStartedQueue.empty() || !inProgressQueue.empty()) {
    // Prepare only if "not started queue" is not empty.
    if (!notStartedQueue.empty()) {
      // Try to execute each queue item once then move on to checking inProgress.
      notStartedSize = notStartedQueue.size();
      // Prepare thread to be moved from notStarted to inProgress.
      while ((notStartedSize > 0) && (activeThreadCount < coreCount)) {
        threadIndex = pop_front(notStartedQueue);
        // Passing function pointer to arg of pthread_create function
        threadStatus = pthread_create(&threadContext[threadIndex],
                                      NULL,
                                      testTypes_Template_Pthread, // myTestFuncs[threadIndex],
                                      &threadVector->threadContextVectorMeta[threadIndex]);
        if (0 == threadStatus) {
          push_back(inProgressQueue, threadIndex);
          activeThreadCount++;
          printf("Thread %ld started. Wait queue is %ld.\n", threadIndex, notStartedQueue.size());
        } else if (EINVAL == threadStatus || EPERM == threadStatus) {
          fprintf(stderr, "Error on line %d : %s\n.Cannot create thread %ld: invalid setting or permission.", __LINE__,
                  strerror(errno), threadIndex);
          push_front(errorQueue, threadIndex);
        } else { // (threadStatus == EAGAIN
          printf("Cannot create thread %ld: resource limited.", threadIndex);
          push_back(notStartedQueue, threadIndex);
        }
        notStartedSize--;
      }
    }

    // Wait for some time so threads cann to do work...
    sleep(waitTime);
    if (waitCount >= UINT64_MAX - 1) {
      waitCount = 0;
    } else {
      waitCount++;
    }

    if (waitCount * waitTime % oneMinute) {
      printf("Waiting for threads: total seconds=%ld, flushing stdout buffer.\n", waitCount * waitTime);
      fflush(stdout); // Change to stream buffer if you want to flush files.
    }

    // Verify the status of an "in progress" thread and if done move to complete.
    if (!inProgressQueue.empty()) {
      inProgressSize = inProgressQueue.size();
      while (inProgressSize > 0) {
        threadIndex = pop_front(inProgressQueue);
        threadStatus = pthread_tryjoin_np(threadContext[threadIndex], NULL);
        if (EBUSY == threadStatus) {
          push_back(inProgressQueue, threadIndex);
        } else if (EINVAL == threadStatus || ETIMEDOUT == threadStatus) {
          fprintf(stderr, "Error on line %d : %s.\nThread %ld check status timeout or invalid.\n", __LINE__,
                  strerror(errno), threadIndex);
          push_front(errorQueue, threadIndex);
        } else {
          printf("Thread %ld complete. Wait queue is %ld.\n", threadIndex, inProgressQueue.size());
          push_back(completedQueue, threadIndex);
        }
        inProgressSize--;
      }
    }
  }

  // Waits for threads to complete
  for (size_t i = 0; i < testSize; i++) {
    pthread_join(threadContext[i], NULL);
  }

  // Close and print paths.
  for (size_t threadIndexLocal = 0; threadIndexLocal < testSize; threadIndexLocal++) {
    fclose(threadVector->threadContextVectorMeta[threadIndexLocal].saveFileContext);
    printFullPath(threadVector->threadContextVectorMeta[threadIndexLocal].saveFilename);
  }
  // Deconstruct
  delete[] threadContext;
  delete[] threadID;
  pthread_exit(NULL);

  return EXIT_SUCCESS;
}

/*======================================================================================================================
 * Helper functions
 * ===================================================================================================================*/
/******************************************************************************
*
* @return
*****************************************************************************/
template<typename Type>
bool safeAlloc(Type *&address, size_t size) {
  bool isValid = false;

# if !ENABLE_BASIC_C_ALLOC
  if (size <= 1) {
    address = new Type;
  } else {
    address = new Type[size];
  }
  isValid = (NULL != address);
#else // ENABLE_BASIC_C_ALLOC
  address = malloc(sizeof(Type)*size);
  if (NULL != address) {
    isValid = true;
    memset(address, sizeof(Type) * size);
  }
#endif // ENABLE_BASIC_C_ALLOC
  assertm(isValid == true, "Invalid Allocation...");
  return isValid;
}

/******************************************************************************
*
* @return
*****************************************************************************/
void showUsage(void) {
  printf("Usage: <option(s)> PARAMETER\n");
  printf("Options:\n");
  printf("\t-h, --help\t\tShow this help message\n");
}

/******************************************************************************
*
* @return
*****************************************************************************/
void printArgs(int argc, char *argv[]) {
  size_t selectSize, bufferSize;
  size_t bufferMax = CHAR_BUFFER_SIZE - 1;
  char inputBuffer[CHAR_BUFFER_SIZE];
  printf("Inputs and parameters::\n");
  printf(" argc = %d\n", argc);
  for (size_t i = 0; i < (size_t) argc; i++) {
    bufferSize = sizeof(argv[i]);
    if (bufferSize >= bufferMax) {
      fprintf(stderr, " Error on line %d : %s.\n", __LINE__, strerror(errno));
      fprintf(stderr, " Buffer overflow attempt : bufferSize(%ld) >= CHAR_BUFFER_MAX(%ld)\n", bufferSize, bufferMax);
      exit(EXIT_FAILURE);
    } else {
      selectSize = bufferSize > bufferMax ? bufferMax : bufferSize;
      strncpy(inputBuffer, argv[i], selectSize);
      printf(" argv[%ld] = %s with size %ld\n", i, inputBuffer, selectSize);
    }
  }
  return;
}

/******************************************************************************
*
* @return
*****************************************************************************/
int32_t printFullPath(const char *partialPath) {
  int32_t rc = 0;
  char *fullPath = NULL;
  safeAlloc<char>(fullPath, PATH_MAX);
#if (defined(_WIN32) | defined(_WIN64))
  if (_fullpath(fullPath, partialPath, PATH_MAX) != NULL) {
    printf("Path, %s, %s\n", partialPath, fullPath);
  }
  else {
    rc = -1;
    printf("Path Error, %s\n", partialPath);
  }
#else // (defined(_WIN32) | defined(_WIN64))
  if (realpath(partialPath, fullPath) == 0) {
    rc = -1;
    printf("Path Error, %s\n", partialPath);
    asserterror();
  } else {
    printf("Path: PartialPath=%s, FullPath=%s\n", partialPath, fullPath);
  }
#endif // (defined(_WIN32) | defined(_WIN64))
  return rc;
}

/******************************************************************************
*
* @return
*****************************************************************************/
uint32_t getNumCores(void) {
  uint32_t coreCount;
#if defined(_WIN32) | defined(_WIN64)
  SYSTEM_INFO sysinfo;
  GetSystemInfo(&sysinfo);
  coreCount = sysinfo.dwNumberOfProcessors;
#elif !defined(__linux__) // Not Linux
  coreCount = 1;
#else // Linux OS
  coreCount = sysconf(_SC_NPROCESSORS_ONLN);
#endif
  return coreCount;
}

/******************************************************************************
*
* @return
*****************************************************************************/
double getTime(void) {
# if defined(_WIN32) | defined(_WIN64)
  struct _timeb tb;
  _ftime(&tb);
  return (double)tb.time + (0.001 * (double)tb.millitm);
# else
  struct timeval tv;
  if (gettimeofday(&tv, 0) < 0) {
    fprintf(stderr, "Error on line %d : %s.\n", __LINE__, strerror(errno));
  }
  return (double) tv.tv_sec + (0.000001 * (double) tv.tv_usec);
# endif
}

/******************************************************************************
*
* @return
*****************************************************************************/
/*Set array of chars to null for printf*/
void setCharArray(char content[CHAR_BUFFER_SIZE]) {
  unsigned int i;
  for (i = 0; i < CHAR_BUFFER_SIZE; i++) {
    content[i] = '\0';
  }
  return;
}

/******************************************************************************
*
* @return
*****************************************************************************/
template<typename Type>
Type pop_front(std::vector<Type> &v) {
  Type dValue;
  bool isEmpty = v.empty();
  if (!isEmpty) {
    dValue = v.front();
    v.erase(v.begin());
  } else {
    printf("pop_front use warning is empty\n");
    memset(&dValue, UINT_MAX, sizeof(Type));
  }
  return dValue;
}

/******************************************************************************
*
* @return
*****************************************************************************/
template<typename Type>
Type pop_back(std::vector<Type> &v) {
  Type dValue;
  bool isEmpty = v.empty();
  if (!isEmpty) {
    dValue = v.pop_back();
  } else {
    printf("pop_back use warning is empty");
    memset(&dValue, 0, sizeof(Type));
  }
  return dValue;
}

/******************************************************************************
*
* @return
*****************************************************************************/
template<typename Type>
void push_front(std::vector<Type> &v, Type val) {
  v.insert(v.begin(), 1, val);
  return;
}

/******************************************************************************
*
* @return
*****************************************************************************/
template<typename Type>
void push_back(std::vector<Type> &v, Type val) {
  v.insert(v.end(), 1, val);
  return;
}


/******************************************************************************
* Random number generator.
* Generate a uniformly distributed random value.
* References:
*  Knuth Sec. 3.4.1 p. 117
*  Box and Muller, 'A Note on the Generation of Random Normal Deviates'
*  Marsaglia and Bray, 'A Convenient Method for Generating Normal Variables'
*  Abramowitz and Stegun, Handbook of Mathematical Functions
*  Press et al., Numerical Recipes in C Sec. 7.2 pp. 288-290
* @return
*****************************************************************************/
template<class classType>
classType gauss_rand(int select) {
  static unsigned int ranOnce = 0;
  static time_t timeSinceEpoch = 0;
  static int timeSeed = 0;
  static double randomValue = 0.0;
  size_t escapeCount;
  const size_t escapeMax = UINT8_MAX;
  if (ranOnce == 0) {
    ranOnce++;
    timeSinceEpoch = time(NULL);
    timeSeed = timeSinceEpoch ^ clock() >> 1;
    srand(timeSeed); // srand(clock()) or srand(NULL)
  }
  const int NSUM = 25; // Used for random number generators.
  double PI = acos(-1.0); // Exact 'PI' number from math functions.
  double x, X, Z, selected;
  int i;
  static long double U, U1, U2, V, V1, V2, S;
  static int phase = 0;
  static int phase2 = 0;
  long long int c0, c1, c2, c3, c;
  classType outR;

  switch (select) {
    case 0:
      // Herb-grind, paper defined. Note: The randomness is not adequate for large machine learning data sets.
      randomValue = rand();
      c0 = (long long int) randomValue & 0xffff;
      randomValue = rand();
      c1 = (long long int) randomValue & 0xffff;
      randomValue = rand();
      c2 = (long long int) randomValue & 0xffff;
      randomValue = rand();
      c3 = (long long int) randomValue & 0xffff;
      c = ((c3 << 48) | (c2 << 32) | (c1 << 16) | c0);
      memcpy(&selected, &c, sizeof(c));
      break;
    case 1:
      // RAND based random number generator.
      randomValue = rand();
      selected = (randomValue + 1.0) / ((RAND_MAX) + 1.0);
      break;
    case 2:
      // Exploit the Central Limit Theorem (law of large numbers) and add up several uniformly-distributed random numbers.
      x = 0;
      for (i = 0; i < NSUM; i++) {
        randomValue = rand();
        x += randomValue / RAND_MAX;
      }
      x -= NSUM / 2.0;
      x /= sqrt(NSUM / 12.0);
      selected = x;
      break;
    case 3:
      // Use a method described by Abramowitz and Stegun.
      if (phase == 0) {
        randomValue = rand();
        U = (randomValue + 1.) / (RAND_MAX + 2.);
        V = rand() / (RAND_MAX + 1.);
        Z = sqrt(-2 * log(U)) * sin(2 * PI * V);
      } else {
        Z = sqrt(-2 * log(U)) * cos(2 * PI * V);
      }

      phase = 1 - phase;

      selected = Z;
      break;
    case 4:
    default:
      // Use a method discussed in Knuth and due originally to Marsaglia.
      if (phase2 == 0) {
        escapeCount = 0;
        do {
          randomValue = rand();
          U1 = randomValue / RAND_MAX;
          randomValue = rand();
          U2 = randomValue / RAND_MAX;

          V1 = 2 * U1 - 1;
          V2 = 2 * U2 - 1;
          S = V1 * V1 + V2 * V2;
          if (escapeCount > escapeMax) {
            asserterror();
            break;
          }
        } while (S >= 1 || S == 0);

        X = V1 * sqrt(-2 * log(S) / S);
      } else {
        X = V2 * sqrt(-2 * log(S) / S);
      }

      phase2 = 1 - phase2;
      selected = X;
      break;
  }
  return selected;
}

/******************************************************************************
*
* @return
*****************************************************************************/
template<class classType>
classType typelessValid(int select, bool debug) {
  // Ensure random values are valid integers or floats
  bool isValid_Numerical;
  classType inType = (classType) 0;
  char printBuffer[CHAR_BUFFER_SIZE];
  TypeSystemEnumeration_t mt = typelessClassify<classType>(inType);
  dynamicCompact_t candidateValue;
  do {
    inType = gauss_rand<classType>(select);
    switch (mt) {
      case tse_int8_e:
        candidateValue.int8_data = (int8_t) inType;
        isValid_Numerical = (candidateValue.int8_data >= INT8_MIN && candidateValue.int8_data <= INT8_MAX);
        snprintf(printBuffer, CHAR_BUFFER_SIZE, "Operation=int8, A=%d\n", candidateValue.int8_data);
        break;
      case tse_uint8_e:
        candidateValue.uint8_data = (uint8_t) inType;
        isValid_Numerical = (candidateValue.uint8_data >= 0 && candidateValue.uint8_data <= UINT8_MAX);
        snprintf(printBuffer, CHAR_BUFFER_SIZE, "Operation=uint8, A=%d\n", candidateValue.uint8_data);
        break;
      case tse_int16_e:
        candidateValue.int16_data = (int16_t) inType;
        isValid_Numerical = (candidateValue.int16_data >= INT16_MIN && candidateValue.int16_data <= INT16_MAX);
        snprintf(printBuffer, CHAR_BUFFER_SIZE, "Operation=int16, A=%d\n", candidateValue.int16_data);
        break;
      case tse_uint16_e:
        candidateValue.uint16_data = (uint16_t) inType;
        isValid_Numerical = (candidateValue.uint16_data >= 0 && candidateValue.uint16_data <= UINT16_MAX);
        snprintf(printBuffer, CHAR_BUFFER_SIZE, "Operation=uint16, A=%d\n", candidateValue.uint16_data);
        break;
      case tse_int32_e:
        candidateValue.int32_data = (int32_t) inType;
        isValid_Numerical = (candidateValue.int32_data >= INT32_MIN && candidateValue.int32_data <= INT32_MAX);
        snprintf(printBuffer, CHAR_BUFFER_SIZE, "Operation=int32, A=%d\n", candidateValue.int32_data);
        break;
      case tse_uint32_e:
        candidateValue.uint32_data = (uint32_t) inType;
        isValid_Numerical = (candidateValue.uint32_data >= 0 && candidateValue.uint32_data <= UINT32_MAX);
        snprintf(printBuffer, CHAR_BUFFER_SIZE, "Operation=uint32, A=%d\n", candidateValue.uint32_data);
        break;
      case tse_int64_e:
        candidateValue.int64_data = (int64_t) inType;
        isValid_Numerical = (candidateValue.int64_data >= INT64_MIN && candidateValue.int64_data <= INT64_MAX);
        snprintf(printBuffer, CHAR_BUFFER_SIZE, "Operation=int64, A=%ld\n", candidateValue.int64_data);
        break;
      case tse_uint64_e:
        candidateValue.uint64_data = (uint64_t) inType;
        isValid_Numerical = (candidateValue.uint64_data >= 0 && candidateValue.uint64_data <= UINT64_MAX);
        snprintf(printBuffer, CHAR_BUFFER_SIZE, "Operation=uint64, A=%ld\n", candidateValue.uint64_data);
        break;
      case tse_float_e:
        candidateValue.float_data = (float) inType;
        isValid_Numerical = (FP_NORMAL == std::fpclassify(candidateValue.float_data));
        snprintf(printBuffer, CHAR_BUFFER_SIZE, "Operation=float, A=%f\n", candidateValue.float_data);
        break;
      case tse_double_e:
        candidateValue.double_data = (double) inType;
        isValid_Numerical = (FP_NORMAL == std::fpclassify(candidateValue.double_data));
        snprintf(printBuffer, CHAR_BUFFER_SIZE, "Operation=double, A=%f\n", candidateValue.double_data);
        break;
      case tse_long_double_e:
        candidateValue.longdouble_data = (long double) inType;
        isValid_Numerical = (FP_NORMAL == std::fpclassify(candidateValue.longdouble_data));
        snprintf(printBuffer, CHAR_BUFFER_SIZE, "Operation=long_double, A=%Lf\n", candidateValue.longdouble_data);
        break;
      default:
        isValid_Numerical = false;
        asserterror();
        snprintf(printBuffer, CHAR_BUFFER_SIZE, "Operation=unknownType, A=unknown...\n");
        assertm(!isValid_Numerical, "Invalid type passing...\n");
        break;
    }
  } while (!isValid_Numerical);
  if (debug) {
    printf("%s", printBuffer);
  }
  return inType;
}

/******************************************************************************
*
* @return
*****************************************************************************/
template<class classType>
TypeSystemEnumeration_t typelessClassify(classType inType) {
  TypeSystemEnumeration_t detectedType;
  char *inName;
  size_t inASize;
  size_t minSize;

  detectedType = tse_unknown_e;
  inName = (char *) typeid(inType).name();
  inASize = strlen(inName);
  minSize = std::min(inASize, (size_t) CHAR_BUFFER_SIZE);

  const char *uint8_Name = typeid(uint8_t).name();
  const char *int8_Name = typeid(int8_t).name();
  const char *uint16_Name = typeid(uint16_t).name();
  const char *int16_Name = typeid(int16_t).name();
  const char *uint32_Name = typeid(uint32_t).name();
  const char *int32_Name = typeid(int32_t).name();
  const char *uint64_Name = typeid(uint64_t).name();
  const char *int64_Name = typeid(int64_t).name();
  const char *float_Name = typeid(float).name();
  const char *double_Name = typeid(double).name();
  const char *long_double_Name = typeid(long double).name();

  minSize = std::min(inASize, (size_t) strlen(uint8_Name));
  bool match_uint8_Name = (0 == strncmp(inName, uint8_Name, minSize));
  minSize = std::min(inASize, (size_t) strlen(int8_Name));
  bool match_int8_Name = (0 == strncmp(inName, int8_Name, minSize));
  minSize = std::min(inASize, (size_t) strlen(uint16_Name));
  bool match_uint16_Name = (0 == strncmp(inName, uint16_Name, minSize));
  minSize = std::min(inASize, (size_t) strlen(int16_Name));
  bool match_int16_Name = (0 == strncmp(inName, int16_Name, minSize));
  minSize = std::min(inASize, (size_t) strlen(uint32_Name));
  bool match_uint32_Name = (0 == strncmp(inName, uint32_Name, minSize));
  minSize = std::min(inASize, (size_t) strlen(int32_Name));
  bool match_int32_Name = (0 == strncmp(inName, int32_Name, minSize));
  minSize = std::min(inASize, (size_t) strlen(uint64_Name));
  bool match_uint64_Name = (0 == strncmp(inName, uint64_Name, minSize));
  minSize = std::min(inASize, (size_t) strlen(int64_Name));
  bool match_int64_Name = (0 == strncmp(inName, int64_Name, minSize));
  minSize = std::min(inASize, (size_t) strlen(float_Name));
  bool match_float_Name = (0 == strncmp(inName, float_Name, minSize));
  minSize = std::min(inASize, (size_t) strlen(double_Name));
  bool match_double_Name = (0 == strncmp(inName, double_Name, minSize));
  minSize = std::min(inASize, (size_t) strlen(long_double_Name));
  bool match_long_double_Name = (0 == strncmp(inName, long_double_Name, minSize));

  if (match_int8_Name) {
    detectedType = tse_int8_e;
  } else if (match_uint8_Name) {
    detectedType = tse_uint8_e;
  } else if (match_int16_Name) {
    detectedType = tse_int16_e;
  } else if (match_uint16_Name) {
    detectedType = tse_uint16_e;
  } else if (match_int32_Name) {
    detectedType = tse_int32_e;
  } else if (match_uint32_Name) {
    detectedType = tse_uint32_e;
  } else if (match_int64_Name) {
    detectedType = tse_int64_e;
  } else if (match_uint64_Name) {
    detectedType = tse_uint64_e;
  } else if (match_float_Name) {
    detectedType = tse_float_e;
  } else if (match_double_Name) {
    detectedType = tse_double_e;
  } else if (match_long_double_Name) {
    detectedType = tse_long_double_e;
  } else {
    detectedType = tse_unknown_e;
  }
  return detectedType;
}

/******************************************************************************
*
* @return
*****************************************************************************/
bool fileDelete(char fileName[CHAR_BUFFER_SIZE]) {
  bool isDeleted = false;
  if (0 == remove(fileName)) {
    printf("File %s has successful been deleted.\n", fileName);
    isDeleted = true;
  } else {
    fprintf(stderr, "File %s has successful been deleted.\n", fileName);
  }
  return isDeleted;
}

/******************************************************************************
*
* @return
*****************************************************************************/
bool fileCreate(char fileName[CHAR_BUFFER_SIZE], char headerString[CHAR_BUFFER_SIZE]) {
  bool isCreated = false;
  FILE *fileContext = fopen(fileName, "w");
  if (NULL != fileContext) {
    if (NULL != headerString) {
      fprintf(fileContext, "%s", headerString);
    }
    fprintf(fileContext, "\n");
    fclose(fileContext);
    isCreated = true;
    printf("File %s has been created.\n", fileName);
  }
  return isCreated;
}

/******************************************************************************
*
* @return
*****************************************************************************/
bool fileOverwriteNil(char fileName[CHAR_BUFFER_SIZE]) {
  bool isCreated = false;
  FILE *fileContext = fopen(fileName, "w");
  if (NULL != fileContext) {
    fprintf(fileContext, "\n");
    fclose(fileContext);
    isCreated = true;
    printf("File %s has been overwritten.\n", fileName);
  }
  return isCreated;
}

/******************************************************************************
* Checks to see if a file is exists.
* @return  fileState_et in state enumeration.
*****************************************************************************/
fileState_et fileIsFound(const char fileName[CHAR_BUFFER_SIZE]) {
  fileState_et activeFileState = fs_Unknown_vet;
  bool fileStatus = !access(fileName, F_OK);
  if (fileStatus) {
    printf("File %s was Found.\n", fileName);
    activeFileState = fs_Found_vet;
  } else {
    printf("File %s not Found.\n", fileName);
    activeFileState = fs_NotFound_vet;
  }
  return activeFileState;
}

/******************************************************************************
* Checks to see if a file is readable.
* @return  fileState_et in state enumeration.
*****************************************************************************/
fileState_et fileIsReadable(const char fileName[CHAR_BUFFER_SIZE]) {
  fileState_et activeFileState = fs_Unknown_vet;
  bool fileStatus = !access(fileName, R_OK);
  if (fileStatus) {
    printf("The File %s can be read\n", fileName);
    activeFileState = fs_CanBeRead_vet;
  } else {
    printf("The File %s cannot be read\n", fileName);
    activeFileState = fs_CannotBeRead_vet;
  }
  return activeFileState;
}

/******************************************************************************
* Checks to see if a file is editable.
* @return  fileState_et in state enumeration.
*****************************************************************************/
fileState_et fileIsEditable(const char fileName[CHAR_BUFFER_SIZE]) {
  fileState_et activeFileState = fs_Unknown_vet;
  bool fileStatus = !access(fileName, W_OK);
  if (fileStatus) {
    printf("The File %s  can be Edited\n", fileName);
    activeFileState = fs_CanBeEdited_vet;
  } else {
    printf("The File %s  cannot be Edited\n", fileName);
    activeFileState = fs_CannotBeEdited_vet;
  }
  return activeFileState;
}

/******************************************************************************
* Checks to see if a file is executable.
* @return  fileState_et in state enumeration.
*****************************************************************************/
fileState_et fileIsExecutable(const char fileName[CHAR_BUFFER_SIZE]) {
  fileState_et activeFileState = fs_Unknown_vet;
  bool fileStatus = !access(fileName, X_OK);
  if (fileStatus) {
    printf("The File %s is an Executable\n", fileName);
    activeFileState = fs_IsExecutable_vet;
  } else {
    printf("The File %s is not an Executable\n", fileName);
    activeFileState = fs_IsNotExecutable_vet;
  }
  return activeFileState;
}

/******************************************************************************
* Checks to see file state of existence and usage.
* @return  word with one hot bit(s) set for all given states.
*****************************************************************************/
uint32_t fileCheckAll(const char fileName[CHAR_BUFFER_SIZE]) {
  fileState_et activeFileState;
  uint32_t allStates = fs_Unknown_vet;

  activeFileState = fileIsFound(fileName);
  allStates |= activeFileState;

  activeFileState = fileIsReadable(fileName);
  allStates |= activeFileState;

  activeFileState = fileIsEditable(fileName);
  allStates |= activeFileState;

  activeFileState = fileIsExecutable(fileName);
  allStates |= activeFileState;

  return allStates;
}

/******************************************************************************
* Gets current directory and moves up one.
* @return  true if dir access occurred.
*          false if failure in access.
*****************************************************************************/
bool fileUpCurrentDirectory(char absoluteFolderPath[CHAR_BUFFER_SIZE]) {
  bool isAccessed = false;
  errno = 0;
  const char nilChar = '\0';
#if (defined(__WIN32__) or defined(__WIN64__))
  const char dirChar = '\\';
#else
  const char dirChar = '/';
#endif // (defined(__WIN32__) or defined(__WIN64__))
  fileGetCurrentWorkingDirectory(absoluteFolderPath);
  size_t endLoc;
  for (size_t i = 0; i < CHAR_BUFFER_SIZE; i++) {
    endLoc = i;
    if (nilChar == absoluteFolderPath[endLoc]) {
      break;
    }
  }
  for (size_t i = endLoc; i > 0; i--) {
    if (dirChar == absoluteFolderPath[i]) {
      absoluteFolderPath[i] = nilChar;
      isAccessed = true;
      break;
    }
  }
  return isAccessed;
}

/******************************************************************************
* Checks to see if able to get current directory.
* @return  true if dir access occurred.
*          false if failure in access.
*****************************************************************************/
bool fileGetCurrentWorkingDirectory(char absoluteFolderPath[CHAR_BUFFER_SIZE]) {
  bool isAccessed = false;
  errno = 0;
  char *ret = getcwd(absoluteFolderPath, CHAR_BUFFER_SIZE);
  if (NULL == ret) {
    switch (errno) {
      case EACCES:
        printf("the parent directory does not allow write");
        break;
      default:
        // EINVAL, EIO, ENOENT, ENOTDIR, ERANGE
        asserterror();
        break;
    }
  } else {
    isAccessed = true;
  }
  return isAccessed;
}

/******************************************************************************
* Checks to see if a directory exists. Note: This method only checks the
* existence of the full path AND if path leaf is a dir.
* @return  true if dir creation occurred.
*          false if failure in creation.
*****************************************************************************/
bool fileMakeDirectory(const char absoluteFolderPath[CHAR_BUFFER_SIZE]) {
  bool isCreated = false;
  errno = 0;
  if (absoluteFolderPath[0] == '\0') {
    // fprintf(stderr, "Error in absoluteFolderPath is nil token.\n");
    // asserterror();
    return isCreated;
  }
  int ret = mkdir(absoluteFolderPath, S_IRWXU | S_IRWXG);
  if (ret == -1) {
    fprintf(stderr, "Error in absoluteFolderPath=%s\n", absoluteFolderPath);
    switch (errno) {
      case EACCES:
        printf("the parent directory does not allow write\n");
        break;
      case EEXIST:
        printf("pathname already exists\n");
        break;
      case ENAMETOOLONG:
        printf("pathname is too long\n");
        break;
      case EMLINK:
        asserterror();
        break;
      case ELOOP:
        asserterror();
        break;
      case ENOENT:
        asserterror();
        break;
      case ENOSPC:
        asserterror();
        break;
      case ENOTDIR:
        asserterror();
        break;
      case EROFS:
        asserterror();
        break;
      default:
        // Other failure
        asserterror();
        break;
    }
  } else {
    isCreated = true;
  }
  return isCreated;
}

/******************************************************************************
* Creates directory path recursively.
* @return  None
*****************************************************************************/
void fileMakeDirectories(const char absoluteFolderPath[CHAR_BUFFER_SIZE]) {
#if (defined(_WIN32) | defined(_WIN64))
  const char pathSeparator = '\\';
#else // !(defined(_WIN32) | defined(_WIN64))
  const char pathSeparator = '/';
#endif // (defined(_WIN32) | defined(_WIN64))
  const char nilToken = '\0';
  char outPath[CHAR_BUFFER_SIZE];
  char *p;
  size_t len;
  size_t charIndex;
  char sampleChar;
  char sampleBuffer[CHAR_BUFFER_SIZE];
  setCharArray(sampleBuffer);

  strncpy(outPath, absoluteFolderPath, CHAR_BUFFER_SIZE);
  outPath[CHAR_BUFFER_SIZE - 1] = nilToken;
  len = strlen(outPath);
  if (0 == len) {
    return;
  } else if (pathSeparator == outPath[len - 1]) {
    outPath[len - 1] = nilToken;
  }
  charIndex = 0;
  for (p = outPath; *p; p++) {
    sampleChar = *p;
    sampleBuffer[charIndex] = sampleChar;
    if (pathSeparator == *p) {
      *p = nilToken;
      if (access(outPath, F_OK)) {
        fileMakeDirectory(outPath);
      }
      *p = pathSeparator;
    }
    charIndex++;
  }
  if (access(outPath, F_OK)) {
    /* if path is not terminated with / */
    fileMakeDirectory(outPath);
  }
  return;
}

/******************************************************************************
* Checks to see if a directory exists. Note: This method only checks the
* existence of the full path AND if path leaf is a dir.
* @return  >0 if dir exists AND is a dir,
*           0 if dir does not exist OR exists but not a dir,
*          <0 if an error occurred (errno is also set)
*****************************************************************************/
int fileDirectoryExists(const char selectPath[CHAR_BUFFER_SIZE]) {
  struct stat info;
  uint32_t statRC = stat(selectPath, &info);
  uint32_t returnStatus;
  if (0 != statRC) {
    if (errno == ENOENT) {
      // Something along the path does not exist
      returnStatus = 0;
    } else if (errno == ENOTDIR) {
      // something in path prefix is not a dir
      returnStatus = 0;
    } else {
      returnStatus = -1;
    }
  }
  if (info.st_mode & S_IFDIR) {
    returnStatus = 1;
  } else {
    returnStatus = 0;
  }
  return returnStatus;
}

/******************************************************************************
* Checks to see if a directory exists. Note: This method only checks the
* existence of the full path AND if path leaf is a dir.
* @return  >0 if file has path,
*           0 if dir does not exist OR exists but not a dir,
*          <0 if an error occurred (errno is also set)
*****************************************************************************/
int fileGetDirectory(const char selectPath[CHAR_BUFFER_SIZE],
                     char foundDirectory[CHAR_BUFFER_SIZE]) {
#if (defined(_WIN32) | defined(_WIN64))
  const char pathSeparator = '\\';
#else // !(defined(_WIN32) | defined(_WIN64))
  const char pathSeparator = '/';
#endif // (defined(_WIN32) | defined(_WIN64))
  const char charNil = '\0';
  char directoryPath[CHAR_BUFFER_SIZE];
  bool isCVS;
  bool isFolder;
  bool isNotEmpty;
  bool canHaveFileExtension;
  size_t len;
  uint32_t returnStatus;

  returnStatus = -1;
  setCharArray(directoryPath);
  setCharArray(foundDirectory);
  strncpy(directoryPath, selectPath, CHAR_BUFFER_SIZE);

  len = strlen(directoryPath);
  canHaveFileExtension = (len > 4);
  isNotEmpty = (len > 1);
  isFolder = isNotEmpty && (pathSeparator == directoryPath[len - 1]);
  if (canHaveFileExtension) {
    // System is build for *.cvs file creation.
    isCVS = (directoryPath[len - 4] == '.');
    isCVS = isCVS && (directoryPath[len - 3] == 'c');
    isCVS = isCVS && (directoryPath[len - 2] == 'v');
    isCVS = isCVS && (directoryPath[len - 1] == 's');
    if (isCVS) {
      returnStatus = 0;
      for (size_t charIndex = len - 1; charIndex > 0; charIndex--) {
        if (pathSeparator == directoryPath[charIndex]) {
          directoryPath[charIndex] = charNil;
          returnStatus = 1;
          break;
        } else {
          directoryPath[charIndex] = charNil;
        }
      }
    } else if (isFolder) {
      // Last char is path separator, so directory.
      returnStatus = 1;
    } else if (isNotEmpty) {
      // Could be directory path, if path separator exists.
      for (size_t charIndex = len - 1; charIndex > 0; charIndex--) {
        if (pathSeparator == directoryPath[charIndex]) {
          isFolder = true;
          returnStatus = 1;
          break;
        }
      }
    } else {
      returnStatus = -1;
    }
  }
  strncpy(foundDirectory, directoryPath, CHAR_BUFFER_SIZE);
  return returnStatus;
}

/* Processor Specific Model Notes:
 $ lscpu | grep MHz
 CPU MHz:                         3200.011
 CPU max MHz:                     3500.0000
 CPU min MHz:                     1200.0000

 $ cat /proc/cpuinfo
 processor	: 0
 vendor_id	: GenuineIntel
 cpu family	: 6
 model		: 79
 model name	: Intel(R) Xeon(R) CPU E5-2687W v4 @ 3.00GHz
 stepping	: 1
 microcode	: 0xb00003e
 cpu MHz		: 3200.015
 cache size	: 30720 KB
 physical id	: 0
 siblings	: 12
 core id		: 0
 cpu cores	: 12
 apicid		: 0
 initial apicid	: 0
 fpu		: yes
 fpu_exception	: yes
 cpuid level	: 20
 wp		: yes
 flags		: fpu vme de pse tsc msr pae mce cx8 apic sep mtrr pge mca cmov pat pse36 clflush dts acpi mmx fxsr sse sse2 ss ht tm pbe syscall nx pdpe1gb rdtscp lm constant_tsc arch_perfmon pebs bts rep_good nopl xtopology nonstop_tsc cpuid aperfmperf pni pclmulqdq dtes64 monitor ds_cpl vmx smx est tm2 ssse3 sdbg fma cx16 xtpr pdcm pcid dca sse4_1 sse4_2 x2apic movbe popcnt tsc_deadline_timer aes xsave avx f16c rdrand lahf_lm abm 3dnowprefetch cpuid_fault epb cat_l3 cdp_l3 invpcid_single pti intel_ppin ssbd ibrs ibpb stibp tpr_shadow vnmi flexpriority ept vpid ept_ad fsgsbase tsc_adjust bmi1 hle avx2 smep bmi2 erms invpcid rtm cqm rdt_a rdseed adx smap intel_pt xsaveopt cqm_llc cqm_occup_llc cqm_mbm_total cqm_mbm_local dtherm ida arat pln pts md_clear flush_l1d
 bugs		: cpu_meltdown spectre_v1 spectre_v2 spec_store_bypass l1tf mds swapgs taa itlb_multihit
 bogomips	: 6000.02
 clflush size	: 64
 cache_alignment	: 64
 address sizes	: 46 bits physical, 48 bits virtual
 power management:
*/
double getCPUInfoTokenDouble(const char *matchString, uint64_t matchStringSize) {
  double foundFrequency = 0.0;
  static char buffer[CHAR_BUFFER_SIZE];
  static char digits[CHAR_BUFFER_SIZE];
  int64_t bufferLimit = CHAR_BUFFER_SIZE - 1;
  int64_t minCompareSize;
  int64_t characters;
  uint64_t bufferIndex;
  uint64_t digitsIndex;
  uint64_t nextIndex;
  FILE *cpuInfoFile;
  int matchCode;
  int foundFirstDigit, foundDigit;

  // Set buffer ends to NULL char.
  buffer[bufferLimit] = '\0';
  digits[bufferLimit] = '\0';

  // Open CPU info file.
  cpuInfoFile = fopen("/proc/cpuinfo", "rb");
  if (NULL == cpuInfoFile) {
    fprintf(stderr, "ERROR! Cannot retrieve CPU Information from /proc/cpuinfo\n.");
    return foundFrequency;
  } else {
    do {
      characters = (int64_t)(fgets(buffer, bufferLimit, cpuInfoFile));
      if ((int64_t)matchStringSize <= characters) {
        minCompareSize = (int64_t)matchStringSize;
      }
      else {
        minCompareSize = characters;
      }
      if (minCompareSize <= bufferLimit) {
        minCompareSize = minCompareSize;
      }
      else {
        minCompareSize = bufferLimit;
      }
      matchCode = strncmp(matchString, buffer, matchStringSize);
      if (0 == matchCode) {
        digitsIndex = 0;
        foundFirstDigit = 0;
        for (bufferIndex = 0; bufferIndex < bufferLimit; bufferIndex++) {
          foundDigit = isdigit(buffer[bufferIndex]);
          if ((0 == foundFirstDigit) && (1 == foundDigit)) {
            foundFirstDigit = 1;
          }

          if ((1 == foundFirstDigit) && (1 == foundDigit)) {
              digits[digitsIndex] = buffer[bufferIndex];
              digitsIndex++;
          }
          else if ((1 == foundFirstDigit) && (0 == foundDigit)) {
            break;
          }
        }
        if (digitsIndex > 0) {
          nextIndex = digitsIndex + 1;
          if (nextIndex < bufferLimit) {
            digits[digitsIndex + 1] = '\0';
          }
          foundFrequency = strtod(digits, NULL);
          break;
        }
      }
    } while ((int64_t) NULL != characters);
  }
  return foundFrequency;
}

long double getCPUFrequency(void) {
  const double convertMegaToKilo = 1000;
  char matchString[7];
  matchString[0] = 'c';
  matchString[1] = 'p';
  matchString[2] = 'u';
  matchString[3] = ' ';
  matchString[4] = 'M';
  matchString[5] = 'H';
  matchString[6] = 'z';
  int64_t matchStringSize = sizeof(matchString) / sizeof(matchString[0]);
  double foundFreq = getCPUInfoTokenDouble(matchString, matchStringSize);
  // Convert MHz to KHz
  foundFreq = foundFreq * convertMegaToKilo;
  return foundFreq;
}

#endif // _CPUBENCHMARKPARALLEL_CPP_

#ifndef __cplusplus
}
#endif // __cplusplus