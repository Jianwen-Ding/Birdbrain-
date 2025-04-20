#ifndef DEBUG_H
#define DEBUG_H

#define DEBUGGING true

// Allows for 
#if DEBUGGING
    #include <iostream>
    #include <cassert>

    // Will produce an error if debugging is enabled and the statement given is false.
    #define ASSERT(x)\
    {\
        assert(x);\
    } 
    // Will simply not log on compile time
    #define LOG(x)\
    {\
        if(!(std::is_constant_evaluated())) {\
            std::cout << x << std::endl;\
        }\
    }
    // Will produce an error if debugging is enabled and the statement given is false.
    #define ASSERT_LOG(x, y) \
    {\
        if(!(x)) {\
            LOG(y);\
            ASSERT(x);\
        }\
    }
#else
    // 
    #define ASSERT(x) ((void)0)
    #define ASSERT_LOG(x) ((void)0)
    #define ((void)0)
#endif

#endif