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
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <typeinfo>
#include <type_traits>
#include <vector>

#define __STDC_LIMIT_MACROS

#ifdef _WIN32
#include <sys/timeb.h>
#include <windows.h>
#else //  !_WIN32

#include <sys/time.h>
#include <unistd.h>

#endif // _WIN32

#define CHAR_BUFFER_SIZE 1024
#define PATH_MAX 4096
#define CHAR_PTHREAD_NAMELEN_MAX 16

// Use (void) to silent unused warnings.
#define assertm(exp, msg) assert(((void)msg, exp))

// @todo remove and modularize
volatile size_t DATASET_SIZE = 4294967291; // 100000007;
const char filenameCPUData[] = "cpu_benchmark.csv"; // Random self generation file name.
FILE *writingFileContext = (FILE *) calloc(1, sizeof(FILE));

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
inline void errorAtLine(void);

inline void errorAtLineThread(size_t threadIndex);

void testTypes(void);

void showUsage(void);

void printArgs(int argc, char *argv[]);

int32_t printFullPath(const char *partialPath);

uint32_t getNumCores(void);

double getTime(void);

void setCharArray(char content[CHAR_BUFFER_SIZE]);

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
classType typelessValid(int select);

// Pass pointer-to-template-function as function argument.
// Arithmetic Call template method on class template parameters.
// @note typedef template function pointer
template<template<typename> class tFunctor, class classType>
classType perform(classType a, classType b);

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
classType typelessPrint(classType inA, classType inB, classType outR, const char operationName[CHAR_BUFFER_SIZE],
                        FILE *writeFileContext, long double timeDelta, size_t loopIterations);

// Tests exact
void *testTypesExact(void *inArgs);

void testTypesTemplateFocused(void);

// Tests templates
template<typename Type>
void typelessTest(Type inA, Type inB);

template<typename Type>
void *testTypesTemplate(void *inArgs);

// Tests Pthreads
//  exact
template<typename Type>
void *my_test(void *args);

/*======================================================================================================================
 * Pthread generic struct definitions and prototypes for usage in arithmetic
 * ===================================================================================================================*/
typedef struct threadContextMeta {
  size_t loopSetSize;
  uint16_t threadTag;
  uint8_t isExecuting;
  char *saveFilename;
  FILE *saveFileContext;
  char *datatypeIDName;
  uintptr_t *operandsMeta;
  uintptr_t *resultantsMeta;
  char *messages;
} threadContextMeta_t;

typedef struct threadContextArray {
  std::vector<threadContextMeta_t> threadContextVectorMeta;
} threadContextArray_t;

bool threadContextMeta_init(threadContextMeta_t *threadContextData);

template<typename Type>
bool threadContextMeta_set(threadContextMeta_t *threadContextData,
                           size_t loopSetSize,
                           uint16_t threadTag,
                           uint8_t isExecuting,
                           char saveFilename[CHAR_BUFFER_SIZE],
                           FILE *saveFileContext,
                           Type *operandsMeta,
                           size_t operandsMetaSize,
                           Type *resultantsMeta,
                           size_t resultantsMetaSize,
                           char messages[CHAR_BUFFER_SIZE]);

/*======================================================================================================================
 * Typedef template function pointers for pthreads_create
 * ===================================================================================================================*/
// Code reads inside out such that *func_ptr is the function declaration.
// func_ptr is a function pointer such that the first void* is the return
// data and takes in the second void*.
typedef void *(*func_ptr)(void *);

// @todo disabled and not in use
/*======================================================================================================================
 * Struct for template function pointers
 * ===================================================================================================================*/
// @note investigate deconstructing function pointers in a C++ templates for more generic usage.
// Function passed as template argument
// typedef struct StructBinaryOperation {
//  template<typename typeResult, typename typeS, typename typeT, typename typeU, typename typeV>
//  typeResult operator()(typeResult (*binaryOperation )(typeS, typeT), typeU u, typeV v) {
//    return binaryOperation(u, v);
//  }
//} StructBinaryOperation_t;

/*======================================================================================================================
 * Template function pointer helpers
 * ===================================================================================================================*/
// @todo disabled and not in use
// Uses pointer to a function that takes a void and returns void. Explicitly gives the template argument for fakeTemplateFunction
// void topFunctionPointer(returnData (*functionPointer)(type *arguments)) {}
// Example: return_type=char*, input_type=int*
//          void headlessFunction(char* (*pf)(int*)) {}
// void headlessFunction(void (*pf)(void)) {}

// Template function pointer that takes a template type
// template<typename Type> void fakeTemplateFunction(Type *) {}

// Function that wants the function pointer as an argument can itself be a template.
// template<typename Type> void gateFunction(void (*pf)(Type *)) {}

// Taking the address of a generated function template
// I.E. Usages and explanation
// headlessFunction(&fakeTemplateFunction<int*>); // Full type specification
// headlessFunction(&fakeTemplateFunction); // Type deduction
// gateFunction<int>(&fakeTemplateFunction<int>); // Full type specification
// gateFunction(&fakeTemplateFunction<int>); // Type deduction
// gateFunction<int>(&fakeTemplateFunction); // Partial (but sufficient) specification
// void headlessFunctionDux(float (*pf)(float)) {}

// Template function pointer that takes a template type
// template<typename Type> Type fakeTemplateFunctionDux(Type *) {}

// Function that wants the function pointer as an argument can itself be a template.
// template<typename Type> Type gateFunctionDux(void (*pf)(Type *)) {}
// I.E. Usages and explanation
// headlessFunction(&fakeTemplateFunction<int*>); // Full type specification
// headlessFunction(&fakeTemplateFunction); // Type deduction
// gateFunction<int>(&fakeTemplateFunction<int>); // Full type specification
// gateFunction(&fakeTemplateFunction<int>); // Type deduction
// gateFunction<int>(&fakeTemplateFunction); // Partial (but sufficient) specification

//template<class ClassType> struct ExecFunc {
//  typedef void(*type)(ClassType);
//};

//template <typename T, typename R, typename ...Args>
//R proxyCall(T & obj, R (T::*mf)(Args...), Args &&... args)
//{
//  return (obj.*mf)(std::forward<Args>(args)...);
//} // proxycall(obj, &hello::f)

// typedef {
//   void (*)(void);
//} fptr_t;
// Function pointer alias
//typedef StructBinaryOperation_t func_ptr;
// typedef void (*func_ptr <typename>)(void);
// template <typename T, typename U>
// using fPtrType = T(*)(U);
/*{
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
 */

/*======================================================================================================================
 * Function definition and implementation
 * ===================================================================================================================*/
template<class classType>
classType typelessSubtraction(classType u, classType v) {
  return u - v;
}

template<class classType>
struct tSubtract {
  classType operator()(classType a, classType b) {
    return typelessSubtraction<classType>(a, b);
  }
};

template<class classType>
classType typelessAddition(classType u, classType v) {
  return u + v;
}

template<class classType>
struct tAddition {
  classType operator()(classType a, classType b) {
    return typelessAddition<classType>(a, b);
  }
};

template<class classType>
classType typelessMultiplication(classType u, classType v) {
  return u * v;
}

template<class classType>
struct tMultiplication {
  classType operator()(classType a, classType b) {
    return typelessMultiplication<classType>(a, b);
  }
};

template<class classType>
classType typelessDivision(classType u, classType v) {
  return u / v;
}

template<class classType>
struct tDivision {
  classType operator()(classType a, classType b) {
    return typelessDivision<classType>(a, b);
  }
};

template<template<typename> class tFunctor, class classType>
classType perform(classType a, classType b) {
  // Equivalent to this:
  // tFunctor<classType> functor;
  // return functor(a, b);
  return tFunctor<classType>()(a, b);
}

template<template<typename> class tPFunctor, class classType>
classType performPrint(classType inA, classType inB, classType outR, const char operationName[CHAR_BUFFER_SIZE],
                       FILE *writeFileContext, long double timeDelta, size_t loopIterations) {
  // Equivalent to this:
  // tPFunctor<classType> functor;
  // return functor(inA, inB, outR, operationName);
  return tPFunctor<classType>()(inA, inB, outR, operationName, writeFileContext, timeDelta, loopIterations);
}

// Note: using std::enable_if with anonymous type parameters
// template <class classType, std::enable_if_t<!std::is_arithmetic<classType>::value>* = nullptr>
template<class classType>
classType typelessPrint(classType inA, classType inB, classType outR,
                        const char operationName[CHAR_BUFFER_SIZE],
                        FILE *writeFileContext,
                        long double timeDelta,
                        size_t loopIterations) {
  char printBuffer[CHAR_BUFFER_SIZE];
  char inATypeName[CHAR_BUFFER_SIZE];
  char inBTypeName[CHAR_BUFFER_SIZE];
  char outRTypeName[CHAR_BUFFER_SIZE];
  size_t inASize = sizeof(typeid(inA).name());
  size_t inBSize = sizeof(typeid(inB).name());
  size_t outRSize = sizeof(typeid(outR).name());
  strncpy(inATypeName, typeid(inA).name(), inASize);
  strncpy(inBTypeName, typeid(inB).name(), inBSize);
  strncpy(outRTypeName, typeid(outR).name(), outRSize);
  bool matchAB = strcmp(inATypeName, inBTypeName);
  bool matchAR = strcmp(inATypeName, outRTypeName);
  bool matchBR = strcmp(inBTypeName, outRTypeName);
  bool areAllSameType = (matchAB && matchAR && matchBR);

  if (areAllSameType) {
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

    bool match_uint8_Name = strcmp(inATypeName, uint8_Name);
    bool match_int8_Name = strcmp(inATypeName, int8_Name);
    bool match_uint16_Name = strcmp(inATypeName, uint16_Name);
    bool match_int16_Name = strcmp(inATypeName, int16_Name);
    bool match_uint32_Name = strcmp(inATypeName, uint32_Name);
    bool match_int32_Name = strcmp(inATypeName, int32_Name);
    bool match_uint64_Name = strcmp(inATypeName, uint64_Name);
    bool match_int64_Name = strcmp(inATypeName, int64_Name);
    bool match_float_Name = strcmp(inATypeName, float_Name);
    bool match_double_Name = strcmp(inATypeName, double_Name);
    bool match_long_double_Name = strcmp(inATypeName, long_double_Name);
    /* IEEE-754 Precision of the representation for printing values.
     * A 32-bit, single-precision IEEE754 number has 24 mantissa bits, which gives about 23+1 * log10(2) = 7.22 ~ 8 digits of precision.
     * A 64-bit, double precision IEEE754 number has 53 mantissa bits, which gives about 52+1 * log10(2) = 15.95 ~ 16 digits of precision.
     * A 128-bit, long double precision IEEE754 number has 112 mantissa bits, which gives about 112+1 * log10(2) = 34.01 ~ 35 digits of precision.
     */
    // Header is: Type System, Operation Set Name, Time for Operations, Count of Operations Performed, LHS, RHS, R
    if (match_int8_Name) {
      snprintf(printBuffer, CHAR_BUFFER_SIZE, "int8_t, %s, %Lf, %lu, %d, %d, %d",
               operationName, timeDelta, loopIterations,
               (int8_t) inA, (int8_t) inB, (int8_t) outR);
    } else if (match_uint8_Name) {
      snprintf(printBuffer, CHAR_BUFFER_SIZE, "uint8_t, %s, %Lf, %lu, %d, %d, %d",
               operationName, timeDelta, loopIterations,
               (uint8_t) inA, (uint8_t) inB, (uint8_t) outR);
    } else if (match_int16_Name) {
      snprintf(printBuffer, CHAR_BUFFER_SIZE, "int16_t, %s, %Lf, %lu, %d, %d, %d",
               operationName, timeDelta, loopIterations,
               (int16_t) inA, (int16_t) inB, (int16_t) outR);
    } else if (match_uint16_Name) {
      snprintf(printBuffer, CHAR_BUFFER_SIZE, "uint16_t, %s, %Lf, %lu, %d, %d, %d",
               operationName, timeDelta, loopIterations,
               (uint16_t) inA, (uint16_t) inB, (uint16_t) outR);
    } else if (match_int32_Name) {
      snprintf(printBuffer, CHAR_BUFFER_SIZE, "int32_t, %s, %Lf, %lu, %d, %d, %d",
               operationName, timeDelta, loopIterations,
               (int32_t) inA, (int32_t) inB, (int32_t) outR);
    } else if (match_uint32_Name) {
      snprintf(printBuffer, CHAR_BUFFER_SIZE, "uint32_t, %s, %Lf, %lu, %d, %d, %d",
               operationName, timeDelta, loopIterations,
               (uint32_t) inA, (uint32_t) inB, (uint32_t) outR);
    } else if (match_int64_Name) {
      snprintf(printBuffer, CHAR_BUFFER_SIZE, "int64_t, %s, %Lf, %lu, %ld, %ld, %ld",
               operationName, timeDelta, loopIterations,
               (int64_t) inA, (int64_t) inB, (int64_t) outR);
    } else if (match_uint64_Name) {
      snprintf(printBuffer, CHAR_BUFFER_SIZE, "uint64_t, %s, %Lf, %lu, %ld, %ld, %ld",
               operationName, timeDelta, loopIterations,
               (uint64_t) inA, (uint64_t) inB, (uint64_t) outR);
    } else if (match_float_Name) {
      snprintf(printBuffer, CHAR_BUFFER_SIZE, "float, %s, %Lf, %lu, %.8f, %.8f, %.8f",
               operationName, timeDelta, loopIterations,
               (float) inA, (float) inB, (float) outR);
    } else if (match_double_Name) {
      snprintf(printBuffer, CHAR_BUFFER_SIZE, "double, %s, %Lf, %lu, %.16f, %.16f, %.16f",
               operationName, timeDelta, loopIterations,
               (double) inA, (double) inB, (double) outR);
    } else if (match_long_double_Name) {
      snprintf(printBuffer, CHAR_BUFFER_SIZE, "long double, %s, %Lf, %lu, %.35Lf, %.35Lf, %.35Lf",
               operationName, timeDelta, loopIterations,
               (long double) inA, (long double) inB, (long double) outR);
    } else {
      // Type System, Operation Set Name, Time for Operations, Count of Operations Performed, LHS, RHS, R
      snprintf(printBuffer, CHAR_BUFFER_SIZE,
               "TypeSystem=unknown_AtLine_%d, OperationSetName=%s, TimeForOperations=%Lf, CountOfOperationsPerformed=%ld, "
               "LHS=unknown, RHS=unknown, R=unknown",
               __LINE__, operationName, timeDelta, loopIterations);
    }
    printf("%s\n", printBuffer);
    fprintf(writingFileContext, "%s\n", printBuffer);
  }
  return outR;
}

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

template<typename Type>
void typelessTest(Type inA, Type inB) {
  // float typelessResult = StructBinaryOperation<float, float, float, float, float>()(&(typelessSubtract), inA, inB);
  FILE *writeFileContext;
  long double timeStart, timeStop, timeDelta;
  size_t loopIterations = DATASET_SIZE;
  Type typelessResult_add, typelessResult_sub, typelessResult_mul, typelessResult_div;

  // Simple delete of old file.
  writingFileContext = fopen(filenameCPUData, "w");
  fprintf(writingFileContext, "\n");
  fclose(writingFileContext);

  // Create new file to append data.
  writingFileContext = fopen(filenameCPUData, "a+");
  fprintf(writingFileContext, "Type, Time for Operations, Count of Operations Performed, LHS, RHS, R\n");

  timeStart = getTime();
  for (size_t index = 0; index < loopIterations; index++) {
    typelessResult_add = perform<tAddition>(inA, inB);
  }
  timeStop = getTime();
  timeDelta = timeStop - timeStart;
  performPrint<tPrint>(inA, inB, typelessResult_add, "addition", writeFileContext, timeDelta, loopIterations);

  timeStart = getTime();
  for (size_t index = 0; index < loopIterations; index++) {
    typelessResult_sub = perform<tSubtract>(inA, inB);
  }
  timeStop = getTime();
  timeDelta = timeStop - timeStart;
  performPrint<tPrint>(inA, inB, typelessResult_sub, "subtraction", writeFileContext, timeDelta, loopIterations);

  timeStart = getTime();
  for (size_t index = 0; index < loopIterations; index++) {
    typelessResult_mul = perform<tMultiplication>(inA, inB);
  }
  timeStop = getTime();
  timeDelta = timeStop - timeStart;
  performPrint<tPrint>(inA, inB, typelessResult_mul, "multiplication", writeFileContext, timeDelta, loopIterations);

  timeStart = getTime();
  for (size_t index = 0; index < loopIterations; index++) {
    typelessResult_div = perform<tDivision>(inA, inB);
  }
  timeStop = getTime();
  timeDelta = timeStop - timeStart;
  performPrint<tPrint>(inA, inB, typelessResult_div, "division", writeFileContext, timeDelta, loopIterations);

  return;
}

void *testTypesExact(void *inArgs) {
  void *danglePtr = NULL;
  typelessTest<int8_t>(3.14, 42);
  typelessTest<uint8_t>(3.14, 42);
  typelessTest<int16_t>(3.14, 42);
  typelessTest<uint16_t>(3.14, 42);
  typelessTest<int32_t>(3.14, 42);
  typelessTest<uint32_t>(3.14, 42);
  typelessTest<int64_t>(3.14, 42);
  typelessTest<uint64_t>(3.14, 42);
  typelessTest<float>(3.14, 42);
  typelessTest<double>(3.14, 42);
  typelessTest<long double>(3.14, 42);
  return danglePtr;
}

void testTypesTemplateFocused(void) {
  const size_t arraySize = 2;
  const int randSelect = 3;
  int8_t randomInputs_int8_t[arraySize];
  uint8_t randomInputs_uint8_t[arraySize];
  int16_t randomInputs_int16_t[arraySize];
  uint16_t randomInputs_uint16_t[arraySize];
  int32_t randomInputs_int32_t[arraySize];
  uint32_t randomInputs_uint32_t[arraySize];
  int64_t randomInputs_int64_t[arraySize];
  uint64_t randomInputs_uint64_t[arraySize];
  float randomInputs_float_t[arraySize];
  double randomInputs_double_t[arraySize];
  long double randomInputs_long_double_t[arraySize];

  for (size_t i = 0; i < arraySize; i++) {
    randomInputs_int8_t[i] = gauss_rand<int8_t>(randSelect);
    randomInputs_uint8_t[i] = gauss_rand<uint8_t>(randSelect);
    randomInputs_int16_t[i] = gauss_rand<int16_t>(randSelect);
    randomInputs_uint16_t[i] = gauss_rand<uint16_t>(randSelect);
    randomInputs_int32_t[i] = gauss_rand<int32_t>(randSelect);
    randomInputs_uint32_t[i] = gauss_rand<uint32_t>(randSelect);
    randomInputs_int64_t[i] = gauss_rand<int64_t>(randSelect);
    randomInputs_uint64_t[i] = gauss_rand<uint64_t>(randSelect);
    randomInputs_float_t[i] = gauss_rand<float>(randSelect);
    randomInputs_double_t[i] = gauss_rand<double>(randSelect);
    randomInputs_long_double_t[i] = gauss_rand<long double>(randSelect);
  }

  typelessTest<int8_t>(randomInputs_int8_t[0], randomInputs_int8_t[1]);
  typelessTest<uint8_t>(randomInputs_uint8_t[0], randomInputs_uint8_t[1]);
  typelessTest<int16_t>(randomInputs_int16_t[0], randomInputs_int16_t[1]);
  typelessTest<uint16_t>(randomInputs_uint16_t[0], randomInputs_uint16_t[1]);
  typelessTest<int32_t>(randomInputs_int32_t[0], randomInputs_int32_t[1]);
  typelessTest<uint32_t>(randomInputs_int32_t[0], randomInputs_int32_t[1]);
  typelessTest<int64_t>(randomInputs_int64_t[0], randomInputs_int64_t[1]);
  typelessTest<uint64_t>(randomInputs_int64_t[0], randomInputs_int64_t[1]);
  typelessTest<float>(randomInputs_float_t[0], randomInputs_float_t[1]);
  typelessTest<double>(randomInputs_double_t[0], randomInputs_double_t[1]);
  typelessTest<long double>(randomInputs_long_double_t[0], randomInputs_long_double_t[1]);
  return;
}

template<typename Type>
void *testTypesTemplate(void *inArgs) {
  void *danglePtr = NULL;
  threadContextMeta_t *thread_info = (threadContextMeta_t *) inArgs;

  const size_t arraySize = 2;
  const int randSelect = 3;
  Type randomInputs_Type_t[arraySize];

  for (size_t i = 0; i < arraySize; i++) {
    randomInputs_Type_t[i] = gauss_rand<Type>(randSelect);
  }

  typelessTest<Type>(randomInputs_Type_t[0], randomInputs_Type_t[1]);
  return danglePtr;
}

bool threadContextMeta_init(threadContextMeta_t *threadContextData) {
  bool isAllocated = false;
  if (NULL == threadContextData) {
    threadContextData = (threadContextMeta_t *) malloc(sizeof(threadContextMeta_t));
    if (NULL != threadContextData) {
      threadContextData->loopSetSize = 0;
      threadContextData->threadTag = 0;
      threadContextData->isExecuting = 0;
      threadContextData->saveFilename = NULL;
      threadContextData->saveFileContext = NULL;
      threadContextData->datatypeIDName = NULL;
      threadContextData->operandsMeta = NULL;
      threadContextData->resultantsMeta = NULL;
      threadContextData->messages = NULL;
      isAllocated = true;
    }
  }
  return isAllocated;
}

bool threadContextArray_init(threadContextArray_t *threadContextArrayData,
                             size_t reserveSize) {
  bool isAllocated = false;
  if (NULL == threadContextArrayData) {
    threadContextArrayData = (threadContextArray_t *) malloc(sizeof(threadContextArray_t));
    if (reserveSize > threadContextArrayData->threadContextVectorMeta.capacity()) {
      threadContextArrayData->threadContextVectorMeta.reserve(reserveSize);
    }
  }
  return isAllocated;
}

template<typename Type>
bool threadContextMeta_set(threadContextMeta_t *threadContextData,
                           size_t loopSetSize,
                           uint16_t threadTag,
                           uint8_t isExecuting,
                           char saveFilename[CHAR_BUFFER_SIZE],
                           FILE *saveFileContext,
                           Type *operandsMeta,
                           size_t operandsMetaSize,
                           Type *resultantsMeta,
                           size_t resultantsMetaSize,
                           char messages[CHAR_BUFFER_SIZE]) {
  bool isAllocated = false;

  if (NULL != threadContextData) {
    threadContextData->loopSetSize = loopSetSize;
    threadContextData->threadTag = threadTag;
    threadContextData->isExecuting = isExecuting;

    if (NULL == threadContextData->saveFilename) {
      threadContextData->saveFilename = malloc(sizeof(char) * CHAR_BUFFER_SIZE);
    }
    strncpy(threadContextData->saveFilename, saveFilename, CHAR_BUFFER_SIZE);

    if (NULL == threadContextData->saveFileContext) {
      threadContextData->saveFileContext = fopen(threadContextData->saveFilename, "w");
    }

    // Construct string array
    if (NULL == threadContextData->datatypeIDName) {
      threadContextData->datatypeIDName = malloc(sizeof(char) * CHAR_BUFFER_SIZE);
    }
    // Extract name from type
    Type tmp;
    size_t dataTypeSize = sizeof(typeid(tmp).name());
    strncpy(threadContextData->datatypeIDName, typeid(tmp).name(), dataTypeSize);

    // Copy over meta data for operands and resultants
    memcpy(threadContextData->operandsMeta, operandsMeta, operandsMetaSize);
    memcpy(threadContextData->resultantsMeta, resultantsMeta, resultantsMetaSize);

    if (NULL == threadContextData->messages) {
      threadContextData->messages = malloc(sizeof(char) * CHAR_BUFFER_SIZE);
    }
    strncpy(threadContextData->messages, messages, CHAR_BUFFER_SIZE);

    isAllocated = true;
  }
  return isAllocated;
}

#ifndef LIBRARY_MODE

int main(int argc, char *argv[])
#elif (LIBRARY_MODE >= 0)
#pragma message("LIBRARY_MODE")
int testharness_CPUBenchmarkParallel_main(int argc, char *argv[])
#else // LIBRARY_MODE is defined
#pragma message("LIBRARY_MODE DEFAULT")
int testharness_CPUBenchmarkParallel_main(int argc, char *argv[])
#endif // LIBRARY_MODE
{
  printArgs(argc, argv);

  std::vector<size_t> notStartedQueue;
  std::vector<size_t> inProgressQueue;
  std::vector<size_t> completedQueue;
  std::vector<size_t> errorQueue;
  size_t coreCount = getNumCores();
  size_t activeThreadCount = 0;
  size_t threadStatus, threadIndex;
  size_t notStartedSize, inProgressSize;
  threadContextMeta_t *threadContextData = (threadContextMeta_t *) malloc(sizeof(threadContextMeta_t));
  // Function pointer list
  // @todo C++: Generate array to function pointers in a loop.
  func_ptr myTestFuncs[] = {
    (&my_test<volatile signed char>),
    (&my_test<volatile unsigned char>),
    (&my_test<volatile signed short>),
    (&my_test<volatile unsigned short>),
    (&my_test<volatile signed int>),
    (&my_test<volatile unsigned int>),
    (&my_test<volatile signed long>),
    (&my_test<volatile unsigned long>),
    (&my_test<volatile signed long long>),
    (&my_test<volatile unsigned long long>),
    (&my_test<volatile float>),
    (&my_test<volatile double>),
    (&my_test<volatile long double>)
  };

  func_ptr myTypelessTestFuncs[] = {
    testTypesTemplate<int8_t>,
    testTypesTemplate<uint8_t>,
    testTypesTemplate<int16_t>,
    testTypesTemplate<uint16_t>,
    testTypesTemplate<int32_t>,
    testTypesTemplate<uint32_t>,
    testTypesTemplate<int64_t>,
    testTypesTemplate<uint64_t>,
    testTypesTemplate<float>,
    testTypesTemplate<double>,
    testTypesTemplate<long double>
  };
  size_t functionPointerListSize = sizeof(myTestFuncs) / sizeof(myTestFuncs[0]);
  pthread_t *threadContext = new pthread_t[functionPointerListSize];
  size_t *threadID = new size_t[functionPointerListSize];

  // volatile size_t DATASET_SIZE = 4294967291; // 100000007;
  // const char filenameCPUData[] = "cpu_benchmark.csv"; // Random self generation file name.
  // FILE *writingFileContext = (FILE *) calloc(1, sizeof(FILE));

  // Simple delete of old file.
  writingFileContext = fopen(filenameCPUData, "w");
  fprintf(writingFileContext, "\n");
  fclose(writingFileContext);
  // Create new file to append data.
  writingFileContext = fopen(filenameCPUData, "a+");
  // Header is: Type System, Operation Set Name, Time for Operations, Count of Operations Performed, LHS, RHS, R
  fprintf(writingFileContext,
          "Type System, Operation Set Name, Time for Operations, Count of Operations Performed, LHS, RHS, R\n");

  // Prepare queue with function pointer index
  for (size_t i = 0; i < functionPointerListSize; i++) {
    threadID[i] = i;
    push_back(notStartedQueue, i);
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
                                      myTestFuncs[threadIndex],
                                      &threadID[threadIndex]);
        if (0 == threadStatus) {
          push_back(inProgressQueue, threadIndex);
          activeThreadCount++;
        } else if (EINVAL == threadStatus || EPERM == threadStatus) {
          fprintf(stderr, "Error on line %d : %s\n.Cannot create thread %ld: invalid setting or permission.", __LINE__,
                  strerror(errno), threadIndex);
          push_front(errorQueue, threadIndex);
        } else { // (threadStatus == EAGAIN
          printf("Cannot create thread %ld: resource limited.", threadIndex);
          push_back(notStartedQueue, threadIndex);
        }
      }
      notStartedSize--;
    }

    // Wait for some time so threads cann to do work...
    sleep(32);

    // PTHREAD_MAX_NAMELEN_NP, pthread_setname_np()

    // Verify the status of an "in progress" thread and if done move to complete.
    if (!inProgressQueue.empty()) {
      inProgressSize = inProgressQueue.size();
      while (inProgressSize > 0) {
        threadIndex = pop_front(inProgressQueue);
        threadStatus = pthread_tryjoin_np(inProgressQueue[threadIndex], NULL);
        if (EBUSY == threadStatus) {
          push_back(inProgressQueue, threadIndex);
        } else if (EINVAL == threadStatus || ETIMEDOUT == threadStatus) {
          fprintf(stderr, "Error on line %d : %s.\nThread %ld check status timeout or invalid.", __LINE__,
                  strerror(errno), threadIndex);
          push_front(errorQueue, threadIndex);
        } else {
          printf("Thread %ld complete.", threadIndex);
          push_back(completedQueue, threadIndex);
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
  pthread_exit(NULL);
  return EXIT_SUCCESS;
}

/*======================================================================================================================
 * Helper functions
 * ===================================================================================================================*/
inline void errorAtLineThread(size_t threadIndex) {
  // https://gcc.gnu.org/onlinedocs/cpp/Standard-Predefined-Macros.html
  // https://gcc.gnu.org/onlinedocs/gcc-4.5.1/gcc/Function-Names.html#Function-Names
  fprintf(stderr, "Error in %s at line %d with %s.\n"
                  "PRETTY_FUNCTION=%s\n"
                  "CONTEXT=%s.\n"
                  "Cannot create thread %ld: invalid setting or permission.",
          __FILE__,
          __LINE__,
          __func__,
          __PRETTY_FUNCTION__,
          strerror(errno),
          threadIndex);
  return;
}

inline void errorAtLine(void) {
  // https://gcc.gnu.org/onlinedocs/cpp/Standard-Predefined-Macros.html
  // https://gcc.gnu.org/onlinedocs/gcc-4.5.1/gcc/Function-Names.html#Function-Names
  fprintf(stderr, "Error in %s at line %d with %s.\n"
                  "PRETTY_FUNCTION=%s\n"
                  "CONTEXT=%s.\n",
          __FILE__,
          __LINE__,
          __func__,
          __PRETTY_FUNCTION__,
          strerror(errno));
  return;
}

void show_usage(void) {
  printf("Usage: <option(s)> PARAMETER");
  printf("Options:\n");
  printf("\t-h, --help\t\tShow this help message\n");
}

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

int32_t printFullPath(const char *partialPath) {
  int32_t rc = 0;
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

double getTime(void) {
# ifdef _WIN32
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

/*Set array of chars to null for printf*/
void setCharArray(char content[CHAR_BUFFER_SIZE]) {
  unsigned int i;
  for (i = 0; i < CHAR_BUFFER_SIZE; i++) {
    content[i] = '\0';
  }
  return;
}

template<typename Type>
void *my_test(void *args) {
  void *voidPtr = NULL;
  size_t threadIDNumber = (size_t) &args;
  printf("Starting %ld...", threadIDNumber);
  Type inType;
  char *name = (char *) (typeid(inType).name());
  double t1;
  volatile Type v_add, v_sub, v_addsub, v_subadd, v_mul, v_div, v_muldiv, v_sqsqrtmul;
  v_add = v_sub = v_addsub = v_subadd = v_mul = v_div = v_muldiv = v_sqsqrtmul = 0;
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
  t1 = getTime();
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
  printf("%s, add, %f, [%d]\n", name, getTime() - t1, (int) v_add & 1);
  fprintf(writingFileContext, "%s, add, %f, %llu, [%d]\n", name, getTime() - t1,
          (unsigned long long int) (DATASET_SIZE * 10), (int) v_add & 1);

  // Subtraction
  t1 = getTime();
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
  printf("%s, sub, %f, [%d]\n", name, getTime() - t1, (int) v_sub & 1);
  fprintf(writingFileContext, "%s, sub, %f, %llu, [%d]\n", name, getTime() - t1,
          (unsigned long long int) (DATASET_SIZE * 10), (int) v_sub & 1);

  // Addition/Subtraction
  t1 = getTime();
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
  printf("%s, add/sub, %f, [%d]\n", name, getTime() - t1, (int) v_addsub & 1);
  fprintf(writingFileContext, "%s, add/sub, %f, %llu, [%d]\n", name, getTime() - t1,
          (unsigned long long int) (DATASET_SIZE * 10), (int) v_addsub & 1);

  // Multiply
  t1 = getTime();
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
  printf("%s, mul, %f, [%d]\n", name, getTime() - t1, (int) v_mul & 1);
  fprintf(writingFileContext, "%s, mul, %f, %llu, [%d]\n", name, getTime() - t1,
          (unsigned long long int) (DATASET_SIZE * 10), (int) v_mul & 1);

  // Divide
  t1 = getTime();
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
  printf("%s, div, %f, [%d]\n", name, getTime() - t1, (int) v_div & 1);
  fprintf(writingFileContext, "%s, div, %f, %llu, [%d]\n", name, getTime() - t1,
          (unsigned long long int) (DATASET_SIZE * 10), (int) v_div & 1);

  // Multiply/Divide
  t1 = getTime();
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
  printf("%s, mul/div, %f, [%d]\n", name, getTime() - t1, (int) v_muldiv & 1);
  fprintf(writingFileContext, "%s, mul/div, %f, %llu, [%d]\n", name, getTime() - t1,
          (unsigned long long int) (DATASET_SIZE * 10), (int) v_muldiv & 1);

  // Square/SquareRoot/Multiply
  t1 = getTime();
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
  printf("%s, mul/sqrt/sq, %f, [%d]\n", name, getTime() - t1, (int) v_sqsqrtmul & 1);
  fprintf(writingFileContext, "%s, sq/sqrt/mul, %f, %llu, [%d]\n", name, getTime() - t1,
          (unsigned long long int) (DATASET_SIZE * 10 * 3), (int) v_sqsqrtmul & 1);
  printf("Ending %ld...", threadIDNumber);

  return voidPtr;
}

template<typename Type>
Type pop_front(std::vector<Type> &v) {
  Type dValue;
  bool isEmpty = v.empty();
  if (!isEmpty) {
    dValue = v.front();
    v.erase(v.begin());
  } else {
    printf("pop_front use warning is empty");
    memset(&dValue, 0, sizeof(Type));
  }
  return dValue;
}

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

template<typename Type>
void push_front(std::vector<Type> &v, Type val) {
  v.insert(v.begin(), 1, val);
  return;
}

template<typename Type>
void push_back(std::vector<Type> &v, Type val) {
  v.insert(v.end(), 1, val);
  return;
}

/* Random number generator.
 * Generate a uniformly distributed random value.
 * References:
 *  Knuth Sec. 3.4.1 p. 117
 *  Box and Muller, 'A Note on the Generation of Random Normal Deviates'
 *  Marsaglia and Bray, 'A Convenient Method for Generating Normal Variables'
 *  Abramowitz and Stegun, Handbook of Mathematical Functions
 *  Press et al., Numerical Recipes in C Sec. 7.2 pp. 288-290
 */
template<class classType>
classType gauss_rand(int select) {
  const int NSUM = 25; // Used for random number generators.
  classType PI = acos(-1.0); // Exact PI number from math functions.
  classType x, X, Z, selected;
  int i;
  static classType U, U1, U2, V, V1, V2, S;
  static int phase = 0;
  static int phase2 = 0;
  classType outR;

  switch (select) {
    case 4:
      // RAND based random number generator.
      selected = ((classType) (rand()) + 1.0) / ((classType) (RAND_MAX) + 1.0);
      break;
    case 1:
      // Exploit the Central Limit Theorem (law of large numbers) and add up several uniformly-distributed random numbers.
      x = 0;
      for (i = 0; i < NSUM; i++) {
        x += (classType) rand() / (classType) RAND_MAX;
      }
      x -= NSUM / 2.0;
      x /= sqrt(NSUM / 12.0);
      selected = x;
      break;
    case 2:
      // Use a method described by Abramowitz and Stegun.
      if (phase == 0) {
        U = (rand() + 1.) / (RAND_MAX + 2.);
        V = rand() / (RAND_MAX + 1.);
        Z = sqrt(-2 * log(U)) * sin(2 * PI * V);
      } else {
        Z = sqrt(-2 * log(U)) * cos(2 * PI * V);
      }

      phase = 1 - phase;

      selected = Z;
      break;
    case 3:
    default:
      // Use a method discussed in Knuth and due originally to Marsaglia.
      if (phase2 == 0) {
        do {
          U1 = (classType) rand() / (classType) RAND_MAX;
          U2 = (classType) rand() / (classType) RAND_MAX;

          V1 = 2 * U1 - 1;
          V2 = 2 * U2 - 1;
          S = V1 * V1 + V2 * V2;
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

template<class classType>
classType typelessValid(int select) {
  // Ensure random values are valid integers or floats
  classType inA = (classType) 0;
  bool isValid_Numerical = false;
  char inATypeName[CHAR_BUFFER_SIZE];
  size_t inASize = sizeof(typeid(inA).name());
  strncpy(inATypeName, typeid(inA).name(), inASize);

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

  bool match_uint8_Name = strcmp(inATypeName, uint8_Name);
  bool match_int8_Name = strcmp(inATypeName, int8_Name);
  bool match_uint16_Name = strcmp(inATypeName, uint16_Name);
  bool match_int16_Name = strcmp(inATypeName, int16_Name);
  bool match_uint32_Name = strcmp(inATypeName, uint32_Name);
  bool match_int32_Name = strcmp(inATypeName, int32_Name);
  bool match_uint64_Name = strcmp(inATypeName, uint64_Name);
  bool match_int64_Name = strcmp(inATypeName, int64_Name);
  bool match_float_Name = strcmp(inATypeName, float_Name);
  bool match_double_Name = strcmp(inATypeName, double_Name);
  bool match_long_double_Name = strcmp(inATypeName, long_double_Name);
  do {
    inA = gauss_rand<classType>(select);

    if (match_int8_Name) {
      int8_t candidateValue = (int8_t) inA;
      isValid_Numerical = (candidateValue >= INT8_MIN && candidateValue <= INT8_MAX);
      printf("Operation=int8, A=%d", candidateValue);
    } else if (match_uint8_Name) {
      uint8_t candidateValue = (uint8_t) inA;
      isValid_Numerical = (candidateValue >= 0 && candidateValue <= UINT8_MAX);
      printf("Operation=uint8, A=%d", candidateValue);
    } else if (match_int16_Name) {
      int16_t candidateValue = (int16_t) inA;
      isValid_Numerical = (candidateValue >= INT16_MIN && candidateValue <= INT16_MAX);
      printf("Operation=int16, A=%d", candidateValue);
    } else if (match_uint16_Name) {
      uint16_t candidateValue = (uint16_t) inA;
      isValid_Numerical = (candidateValue >= 0 && candidateValue <= UINT16_MAX);
      printf("Operation=uint16, A=%d", candidateValue);
    } else if (match_int32_Name) {
      int32_t candidateValue = (int32_t) inA;
      isValid_Numerical = (candidateValue >= INT32_MIN && candidateValue <= INT32_MAX);
      printf("Operation=int32, A=%d", candidateValue);
    } else if (match_uint32_Name) {
      uint32_t candidateValue = (uint32_t) inA;
      isValid_Numerical = (candidateValue >= 0 && candidateValue <= UINT32_MAX);
      printf("Operation=uint32, A=%d", candidateValue);
    } else if (match_int64_Name) {
      int64_t candidateValue = (int64_t) inA;
      isValid_Numerical = (candidateValue >= INT64_MIN && candidateValue <= INT64_MAX);
      printf("Operation=int64, A=%ld", candidateValue);
    } else if (match_uint64_Name) {
      uint64_t candidateValue = (uint64_t) inA;
      isValid_Numerical = (candidateValue >= 0 && candidateValue <= UINT64_MAX);
      printf("Operation=uint64, A=%ld", candidateValue);
    } else if (match_float_Name) {
      float candidateValue = (float) inA;
      isValid_Numerical = (FP_NORMAL == std::fpclassify(candidateValue));
      printf("Operation=float, A=%f", candidateValue);
    } else if (match_double_Name) {
      double candidateValue = (double) inA;
      isValid_Numerical = (FP_NORMAL == std::fpclassify(candidateValue));
      printf("Operation=double, A=%f", candidateValue);
    } else if (match_long_double_Name) {
      long double candidateValue = (long double) inA;
      isValid_Numerical = (FP_NORMAL == std::fpclassify(candidateValue));
      printf("Operation=long_double, A=%Lf", candidateValue);
    } else {
      isValid_Numerical = false;
      printf("Operation=unknownType, A=unknown");
      assertm(!isValid_Numerical, "Invalid type passing");
    }
  } while (!isValid_Numerical);
  return inA;
}

#endif // _CPUBENCHMARKPARALLEL_CPP_

#ifndef __cplusplus
}
#endif // __cplusplus