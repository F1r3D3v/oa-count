#ifndef MACRO_H
#define MACRO_H

// Helper macros
#define SIZE(X) (sizeof(X) / sizeof(X[0]))
#define MIN(A,B) (((A)<(B))?(A):(B))
#define MAX(A,B) (((A)>(B))?(A):(B))

// Define operating platform
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#define PLATFORM "Windows"
typedef long long int ssize_t;
#elif defined(__APPLE__)
#define PLATFORM "MacOS"
#include <sys/types.h>
#elif defined(__linux__)
#define PLATFORM "Linux"
#include <sys/types.h>
#elif defined(unix) || defined(__unix__) || defined(__unix)
#define PLATFORM "Unix"
#include <sys/types.h>
#endif

// BOOL
#define TRUE !(FALSE)
#define FALSE 0

// Typedefs
typedef unsigned char BOOL;

#endif // MACRO_H
