#ifndef DEBUG_H
#define DEBUG_H

#define DEBUGGING true

// Allows for 
#if DEBUGGING
    #include <iostream>
    #include <cassert>

    #define ASSERT(x) assert(x)
    #define ASSERT_LOG(x, y) \
    {   \
        if(!(x)) { \
            std::cout << y << std::endl; \
            assert(x); \
        } \
    }
    #define LOG(x) std::cout << x << std::endl
#else
    #define ASSERT(x) ((void)0)
    #define ASSERT_LOG(x) ((void)0)
    #define ((void)0)
#endif

#endif