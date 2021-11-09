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