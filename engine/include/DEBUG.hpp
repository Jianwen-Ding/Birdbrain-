#ifndef DEBUG_H
#define DEBUG_H

#include <iostream>
#include <cassert>

#define DEBUGGING true

#if DEBUGGING
    #define ASSERT(x) assert(x)
    #define LOG(x) std::cout << x << std::endl
#else
#define ASSERT(x) ((void)0)
    #define ASSERT(x) ((void)0)  // No-op when debugging is disabled
#endif

#endif