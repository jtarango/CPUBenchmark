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

#define CHAR_PTHREAD_NAMELEN_MAX 16

typedef enum TypeSystemEnumeration_e
{
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
  tse_long_double_e = 11
} TypeSystemEnumeration_t;

/* Template
classType (TypeSystemEnumeration_t mt)
{
  switch (mt)
  {
    case tse_int8_e:

      break;
    case tse_uint8_e:

      break;
    case tse_int16_e:

      break;
    case tse_uint16_e:

      break;
    case tse_int32_e:

      break;
    case tse_uint32_e:

      break;
    case tse_int64_e:

      break;
    case tse_uint64_e:

      break;
    case tse_float_e:

      break;
    case tse_double_e:

      break;
    case tse_long_double_e:

      break;
  }
*/

template < TypeSystemEnumeration_t T_mt > class TypeSystemClass;
template < > class TypeSystemClass<tse_int8_e> { public: typedef int8_t type; };
template < > class TypeSystemClass<tse_uint8_e> { public: typedef uint8_t type; };
template < > class TypeSystemClass<tse_int16_e> { public: typedef int16_t type; };
template < > class TypeSystemClass<tse_uint16_e> { public: typedef uint16_t type; };
template < > class TypeSystemClass<tse_int32_e> { public: typedef int32_t type; };
template < > class TypeSystemClass<tse_uint32_e> { public: typedef uint32_t type; };
template < > class TypeSystemClass<tse_int64_e> { public: typedef int64_t type; };
template < > class TypeSystemClass<tse_uint64_e> { public: typedef uint64_t type; };
template < > class TypeSystemClass<tse_float_e> { public: typedef float type; };
template < > class TypeSystemClass<tse_double_e> { public: typedef double type; };
template < > class TypeSystemClass<tse_long_double_e> { public: typedef long double type; };


class TypeSystemFactory {
public:
  ~TypeSystemFactory() { deleteAll(); }
  void * createClass(TypeSystemEnumeration_t mt)
  {
    void * p = 0;
    switch (mt)
    {
      case tse_int8_e:
        p = new c_int8_ts();
        break;
      case tse_uint8_e:
        p = new c_uint8_ts();
        break;
      case tse_int16_e:
        p = new c_int16_ts();
        break;
      case tse_uint16_e:
        p = new c_uint16_ts();
        break;
      case tse_int32_e:
        p = new c_int32_ts();
        break;
      case tse_uint32_e:
        p = new c_uint32_ts();
        break;
      case tse_int64_e:
        p = new c_int64_ts();
        break;
      case tse_uint64_e:
        p = new c_uint64_ts();
        break;
      case tse_float_e:
        p = new c_float_ts();
        break;
      case tse_double_e:
        p = new c_double_ts();
        break;
      case tse_long_double_e:
        p = new c_long_double_ts();
        break;
    }
    if (p) {
      m_Objects.push_back(p);
    }
    return p;
  }

  void deleteAll()
  {
    while (!m_Objects.empty())
    {
      delete m_Objects.front();
      m_Objects.pop_front();
    }
  }
private:
  std::list<SomeBaseClass*> m_Objects;
};




inline bool stringToTypeSystem(const std::string typeName, const std::string dataType){
  size_t minSize = std::min((size_t)typeName.size(), (size_t)dataType.size());
  return strncmp(typeName.c_str(), dataType.c_str(), minSize);
}

// Specialize base_typename_struct to define your primitive types for type systems
template <const char* str, int len, char... suffix>
struct append {
  static constexpr const char* value() {
    return append<str, len-1, str[len-1], suffix...>::value();
  }
};

template <const char* str, char... suffix>
struct append<str, 0, suffix...> {
  static const char value_str[];
  static constexpr const char* value() {
    return value_str;
  }
};

template <const char* str, char... suffix>
const char append<str, 0, suffix...>::value_str[] = { suffix..., 0 };


template <typename T>
struct base_typename_struct;

template <>
struct base_typename_struct<int8_t> {
  static constexpr const char name[] = "int8_t";
};
template <>
struct base_typename_struct<uint8_t> {
  static constexpr const char name[] = "uint8_t";
};
template <>
struct base_typename_struct<int16_t> {
  static constexpr const char name[] = "int16_t";
};
template <>
struct base_typename_struct<uint16_t> {
  static constexpr const char name[] = "uint16_t";
};
template <>
struct base_typename_struct<int32_t> {
  static constexpr const char name[] = "int32_t";
};
template <>
struct base_typename_struct<uint32_t> {
  static constexpr const char name[] = "uint32_t";
};
template <>
struct base_typename_struct<int64_t> {
  static constexpr const char name[] = "int64_t";
};
template <>
struct base_typename_struct<uint64_t> {
  static constexpr const char name[] = "uint64_t";
};
template <>
struct base_typename_struct<float> {
  static constexpr const char name[] = "float";
};
template <>
struct base_typename_struct<double> {
  static constexpr const char name[] = "double";
};
template <>
struct base_typename_struct<long double> {
  static constexpr const char name[] = "long double";
};

template <typename T, char... suffix>
struct typename_struct {
  typedef base_typename_struct<T> base;
  static const char* name() {
    return append<base::name, sizeof(base::name)-1, suffix...>::value();
  }
};

template <typename T, char... suffix>
struct typename_struct<T*, suffix...>:
  public typename_struct<T, '*', suffix...> {
};

void printBaseTypeSystem(void){
  printf("%s", typename_struct<int8_t>::name());
  printf("%s", typename_struct<uint8_t*>::name());
  printf("%s", typename_struct<int16_t**>::name());
  printf("%s", typename_struct<uint16_t***>::name());
  printf("%s", typename_struct<int32_t****>::name());
  printf("%s", typename_struct<uint32_t****>::name());
  printf("%s", typename_struct<int64_t****>::name());
  printf("%s", typename_struct<uint64_t****>::name());
  printf("%s", typename_struct<float****>::name());
  printf("%s", typename_struct<double****>::name());
  printf("%s", typename_struct<long double****>::name());
  return;
}

inline auto determineType(const std::string type_in){
  const std::string type_8s = "int8_t";
  const std::string type_8u = "uint8_t";
  const std::string type_16s = "int16_t";
  const std::string type_16u = "uint16_t";
  const std::string type_32s = "int32_t";
  const std::string type_32u = "uint32_t";
  const std::string type_64s = "int64_t";
  const std::string type_64u = "uint64_t";
  const std::string type_f = "float";
  const std::string type_d = "double";
  const std::string type_ld = "long double";
  bool isMatch;
  isMatch = stringToTypeSystem(type_8s, type_in);
  if (isMatch) {
    return TypeSystemClass<tse_int8_e>::type();
  }

  isMatch = stringToTypeSystem(type_8u, type_in);
  if (isMatch) {
    return TypeSystemClass<tse_uint8_e>::type();
  }

  isMatch = stringToTypeSystem(type_16s, type_in);
  if (isMatch) {
    return TypeSystemClass<tse_int16_e>::type();
  }
  isMatch = stringToTypeSystem(type_16u, type_in);
  if (isMatch) {
    return TypeSystemClass<tse_uint16_e>::type();
  }

  isMatch = stringToTypeSystem(type_32s, type_in);
  if (isMatch) {
    return TypeSystemClass<tse_int32_e>::type();
  }
  isMatch = stringToTypeSystem(type_32u, type_in);
  if (isMatch) {
    return TypeSystemClass<tse_uint32_e>::type();
  }

  isMatch = stringToTypeSystem(type_64s, type_in);
  if (isMatch) {
    return TypeSystemClass<tse_int64_e>::type();
  }
  isMatch = stringToTypeSystem(type_64u, type_in);
  if (isMatch) {
    return TypeSystemClass<tse_uint64_e>::type();
  }

  isMatch = stringToTypeSystem(type_f, type_in);
  if (isMatch) {
    return TypeSystemClass<tse_float_e>::type();
  }
  isMatch = stringToTypeSystem(type_d, type_in);
  if (isMatch) {
    return TypeSystemClass<tse_double_e>::type();
  }
  isMatch = stringToTypeSystem(type_ld, type_in);
  if (isMatch) {
    return TypeSystemClass<tse_long_double_e>::type();
  }
  else{
    return TypeSystemClass<tse_long_double_e>::type();
  }
}

template<class classType>
struct typeDetect {
  classType operator()(const std::string type_in) {
    const std::string type_8s = "int8_t";
    const std::string type_8u = "uint8_t";
    const std::string type_16s = "int16_t";
    const std::string type_16u = "uint16_t";
    const std::string type_32s = "int32_t";
    const std::string type_32u = "uint32_t";
    const std::string type_64s = "int64_t";
    const std::string type_64u = "uint64_t";
    const std::string type_f = "float";
    const std::string type_d = "double";
    const std::string type_ld = "long double";
    bool isMatch;
    isMatch = stringToTypeSystem(type_8s, type_in);
    if (isMatch) {
      return TypeSystemClass<tse_int8_e>::type();
    }
    isMatch = stringToTypeSystem(type_8u, type_in);
    if (isMatch) {
      return TypeSystemClass<tse_uint8_e>::type();
    }

    isMatch = stringToTypeSystem(type_16s, type_in);
    if (isMatch) {
      return TypeSystemClass<tse_int16_e>::type();
    }
    isMatch = stringToTypeSystem(type_16u, type_in);
    if (isMatch) {
      return TypeSystemClass<tse_uint16_e>::type();
    }

    isMatch = stringToTypeSystem(type_32s, type_in);
    if (isMatch) {
      return TypeSystemClass<tse_int32_e>::type();
    }
    isMatch = stringToTypeSystem(type_32u, type_in);
    if (isMatch) {
      return TypeSystemClass<tse_uint32_e>::type();
    }

    isMatch = stringToTypeSystem(type_64s, type_in);
    if (isMatch) {
      return TypeSystemClass<tse_int64_e>::type();
    }
    isMatch = stringToTypeSystem(type_64u, type_in);
    if (isMatch) {
      return TypeSystemClass<tse_uint64_e>::type();
    }

    isMatch = stringToTypeSystem(type_f, type_in);
    if (isMatch) {
      return TypeSystemClass<tse_float_e>::type();
    }
    isMatch = stringToTypeSystem(type_d, type_in);
    if (isMatch) {
      return TypeSystemClass<tse_double_e>::type();
    }
    isMatch = stringToTypeSystem(type_ld, type_in);
    if (isMatch) {
      return TypeSystemClass<tse_long_double_e>::type();
    }
    else{
      return TypeSystemClass<tse_long_double_e>::type();
    }
  }
};

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

// @todo fixme
threadContextMeta_t * newContextMeta(size_t size){
  return new threadContextMeta_t[size];
}

// @todo fixme
typedef struct threadContextMetaArray {
private:
  std::vector<threadContextMeta_t> data;
public:
  threadContextMetaArray(size_t size){
    std::vector<threadContextMeta_t> NuData;
    this->data = NuData;
  }
  threadContextMeta_t& operator [] (size_t size) {
    return data[size];
  }
} threadContextMetaArray_t;

inline void errorAtLine(void);
inline void errorAtLineThread(size_t threadIndex);
void testTypes(void);
void *testTypes_Exact(void *inArgs);
void testTypes_Template_Focused(void);



/******************************************************************************
*
* @return
*****************************************************************************/
void *testTypes_Exact(void *inArgs) {
  void *danglePtr = NULL; // @todo fixme
  FILE* fileContext = NULL; // @todo fixme
  size_t dataSetsSize =  1; // @todo fixme
  testTypes_Template_typeless<int8_t>(3.14, 42, fileContext, dataSetsSize);
  testTypes_Template_typeless<uint8_t>(3.14, 42, fileContext, dataSetsSize);
  testTypes_Template_typeless<int16_t>(3.14, 42, fileContext, dataSetsSize);
  testTypes_Template_typeless<uint16_t>(3.14, 42, fileContext, dataSetsSize);
  testTypes_Template_typeless<int32_t>(3.14, 42, fileContext, dataSetsSize);
  testTypes_Template_typeless<uint32_t>(3.14, 42, fileContext, dataSetsSize);
  testTypes_Template_typeless<int64_t>(3.14, 42, fileContext, dataSetsSize);
  testTypes_Template_typeless<uint64_t>(3.14, 42, fileContext, dataSetsSize);
  testTypes_Template_typeless<float>(3.14, 42, fileContext, dataSetsSize);
  testTypes_Template_typeless<double>(3.14, 42, fileContext, dataSetsSize);
  testTypes_Template_typeless<long double>(3.14, 42, fileContext, dataSetsSize);
  return danglePtr;
}

/******************************************************************************
*
* @return
*****************************************************************************/
void testTypes_Template_Focused(void) {
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
  FILE * fileContext = NULL; // @todo fixme
  size_t dataSetsSize = 1; // @todo

  for (size_t i = 0; i < arraySize; i++) {
    randomInputs_int8_t[i] = gauss_rand<int8_t>(RANDOM_METHOD);
    randomInputs_uint8_t[i] = gauss_rand<uint8_t>(RANDOM_METHOD);
    randomInputs_int16_t[i] = gauss_rand<int16_t>(RANDOM_METHOD);
    randomInputs_uint16_t[i] = gauss_rand<uint16_t>(RANDOM_METHOD);
    randomInputs_int32_t[i] = gauss_rand<int32_t>(RANDOM_METHOD);
    randomInputs_uint32_t[i] = gauss_rand<uint32_t>(RANDOM_METHOD);
    randomInputs_int64_t[i] = gauss_rand<int64_t>(RANDOM_METHOD);
    randomInputs_uint64_t[i] = gauss_rand<uint64_t>(RANDOM_METHOD);
    randomInputs_float_t[i] = gauss_rand<float>(RANDOM_METHOD);
    randomInputs_double_t[i] = gauss_rand<double>(RANDOM_METHOD);
    randomInputs_long_double_t[i] = gauss_rand<long double>(RANDOM_METHOD);
  }

  testTypes_Template_typeless<int8_t>(randomInputs_int8_t[0], randomInputs_int8_t[1], fileContext, dataSetsSize);
  testTypes_Template_typeless<uint8_t>(randomInputs_uint8_t[0], randomInputs_uint8_t[1], fileContext, dataSetsSize);
  testTypes_Template_typeless<int16_t>(randomInputs_int16_t[0], randomInputs_int16_t[1], fileContext, dataSetsSize);
  testTypes_Template_typeless<uint16_t>(randomInputs_uint16_t[0], randomInputs_uint16_t[1], fileContext, dataSetsSize);
  testTypes_Template_typeless<int32_t>(randomInputs_int32_t[0], randomInputs_int32_t[1], fileContext, dataSetsSize);
  testTypes_Template_typeless<uint32_t>(randomInputs_int32_t[0], randomInputs_int32_t[1], fileContext, dataSetsSize);
  testTypes_Template_typeless<int64_t>(randomInputs_int64_t[0], randomInputs_int64_t[1], fileContext, dataSetsSize);
  testTypes_Template_typeless<uint64_t>(randomInputs_int64_t[0], randomInputs_int64_t[1], fileContext, dataSetsSize);
  testTypes_Template_typeless<float>(randomInputs_float_t[0], randomInputs_float_t[1], fileContext, dataSetsSize);
  testTypes_Template_typeless<double>(randomInputs_double_t[0], randomInputs_double_t[1], fileContext, dataSetsSize);
  testTypes_Template_typeless<long double>(randomInputs_long_double_t[0], randomInputs_long_double_t[1], fileContext, dataSetsSize);
  return;
}

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

/******************************************************************************
*
* @return
*****************************************************************************/
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

/******************************************************************************
*
* @return
*****************************************************************************/
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

/******************************************************************************
*
* @return
*****************************************************************************/
template<typename Type>
void *my_test(void *args) {
  void *voidPtr = NULL;
  size_t threadIDNumber = (size_t) &args;
  printf("Starting %ld...\n", threadIDNumber);
  FILE * fileContext = NULL; // @todo fixme
  size_t dataSetSize = 1; // @todo fixme
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
  for (volatile size_t i = 0; i < dataSetSize; ++i) {
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
  fprintf(fileContext, "%s, add, %f, %llu, [%d]\n", name, getTime() - t1,
          (unsigned long long int) (dataSetSize * 10), (int) v_add & 1);

  // Subtraction
  t1 = getTime();
  for (volatile size_t i = 0; i < dataSetSize; ++i) {
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
  fprintf(fileContext, "%s, sub, %f, %llu, [%d]\n", name, getTime() - t1,
          (unsigned long long int) (dataSetSize * 10), (int) v_sub & 1);

  // Addition/Subtraction
  t1 = getTime();
  for (volatile size_t i = 0; i < dataSetSize; ++i) {
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
  fprintf(fileContext, "%s, add/sub, %f, %llu, [%d]\n", name, getTime() - t1,
          (unsigned long long int) (dataSetSize * 10), (int) v_addsub & 1);

  // Multiply
  t1 = getTime();
  for (volatile size_t i = 0; i < dataSetSize; ++i) {
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
  fprintf(fileContext, "%s, mul, %f, %llu, [%d]\n", name, getTime() - t1,
          (unsigned long long int) (dataSetSize * 10), (int) v_mul & 1);

  // Divide
  t1 = getTime();
  for (volatile size_t i = 0; i < dataSetSize; ++i) {
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
  fprintf(fileContext, "%s, div, %f, %llu, [%d]\n", name, getTime() - t1,
          (unsigned long long int) (dataSetSize * 10), (int) v_div & 1);

  // Multiply/Divide
  t1 = getTime();
  for (volatile size_t i = 0; i < dataSetSize; ++i) {
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
  fprintf(fileContext, "%s, mul/div, %f, %llu, [%d]\n", name, getTime() - t1,
          (unsigned long long int) (dataSetSize * 10), (int) v_muldiv & 1);

  // Square/SquareRoot/Multiply
  t1 = getTime();
  for (volatile size_t i = 0; i < dataSetSize; ++i) {
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
  fprintf(fileContext, "%s, sq/sqrt/mul, %f, %llu, [%d]\n", name, getTime() - t1,
          (unsigned long long int) (dataSetSize * 10 * 3), (int) v_sqsqrtmul & 1);
  printf("Ending %ld...", threadIDNumber);

  return voidPtr;
}