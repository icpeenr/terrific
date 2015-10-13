#pragma once
/*
#include <cmath>
#include <unordered_set>
#include <sstream>
#include <cstdlib>
#include <memory>
#include <string>
#include <vector>
#include <iostream>
*/
#include <cmath>
#include <unordered_set>
#include <sstream>
#include <cstdlib>
#include <memory>
#include <string>
#include <vector>

//#pragma warning(push, 0)
#include <entityx/entityx.h>
#include <entityx/deps/Dependencies.h>
//#pragma warning(pop)

//#define internal static
#define local_persist static
#define global_variable static

#define Pi32 3.141519265359f

#define GRAVITY 0.0f//-9.81f

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;
typedef int32 bool32;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef size_t memory_index;

typedef float real32;
typedef double real64;

using std::cout;
using std::endl;
using std::cerr;

namespace ex = entityx;