#pragma once

#ifdef NDEBUG
#define Assert(expr) ((void)0)
#else
#define Assert(expr) { if(!(expr)) { std::cerr << "Assertion " << #expr << " failed in " << __FILE__ << " line " << __LINE__ << std::endl; } }
#endif // NDEBUG

//32-bit floats almost always have sufficient precision for ray tracing, but it’s helpful to be able to switch to double for numerically tricky situations as well as to verify that rounding error with floats isn’t causing errors for a given scene.
#define PBRT_FLOAT_AS_DOUBLE = 0;

#ifdef PBRT_FLOAT_AS_DOUBLE
typedef double Float;
#else
typedef float Float;
#endif // PBRT_FLOAT_AS_DOUBLE