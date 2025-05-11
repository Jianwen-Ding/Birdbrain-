
#  Deterministic Math Library
## Purpose
The point of such a math library is to be used within the rollback logic of the Birdbrain engine cross-platform deterministic, meaning that all given inputs to a function must result in the **exact same** output every single time on every machine. 

The most important implication the determinism requirement is that floating point numbers must not be part of any of the logic within this library. [IEEE 754](https://en.wikipedia.org/wiki/IEEE_754) (A standard on how floating point numbers/operations are implemented) isn't supported across all platforms leading to potential non-deterministic outcomes in math. Integer based math thus makes up the entirety of this math library. The only exception comes when classes within the library needs to be converted to floating point numbers to connect game logic to outside modules like rendering.

The standard library lacks cross-platform deterministic decimal math and is reliant on float for many of parts of their basic math functions such as square root and trigonometry functions. Thus necessitates this library.

However, outside game logic affected by rollback, floating points and standard library should be used. They are supported better by outside libraries and are more likely to be optimized and bug-free.

## Main Components
These are the main components that make up this math library.

### Math Constants
Typedefs and constants often used within the Birdbrain Engine.

### Fixed Point Numbers
Comprised of class FixedPoint.

This is this library's stand in for decimal numbers since float cannot be used.
It is a integer that represents the integer value divided by a set power of two.

#### Templating
FixedPoint has three templates as a class, including...

- Base: The number type that will value of the FixedPoint will be stored as.
- DecimalBits: The amount of bits allocated to represent a fractional part of the number. 
- FlowGuard: Whether arithmetic operations will be shifted to avoid overflow and whether **if debugging**, asserts will go off if an overflow happens

#### Arithmetic Operations and Promotion Rules
Rather than casting numbers into a certain FixedPoint number and then doing the operation, this library used templates to allow all integers and all other FixedPoint numbers to directly be involved with math operations with any FixedNumber. 

When an arithmetic operation is had between a FixedPoint number and an integer, the result will be the same type of FixedPoint number.
    // Since 1 is a fixed point number the result is the fixed point number
    FixedPoint<uint32,8, false> example = FixedPoint<uint32,8, false>("1") + 2;

When an arithmetic operation is had between two FixedPoint numbers ...
- If both FixedPoint numbers are have the same type, the result is the same type 
        // Since both FixedPoint numbers are the same, the result is the 
        FixedPoint<uint32,8, false> example = FixedPoint<uint32,8, false>("1") + FixedPoint<uint32,8, false>("1");
- If one FixedPoint number has a higher integer max than another, that FixedPoint will be the result type
        // The integer max of left hand side is 4294967295 and the integer right hand side' integer max is 2147483647
        // 4294967295 > 2147483647 thus the left hand side's type is the result type
        FixedPoint<uint64,32,false> example = FixedPoint<uint64,32,false>("1") + FixedPoint<int64,32,true>("1");
- Otherwise, if one FixedPoint number has more DecimalBits than another, that FixedPoint will be the result type
        // The left hand side has 9 DecimalBits while the right has 1
        // Thus the left hand side's type is the result type
        FixedPoint<int8,1,true> example = FixedPoint<int16,9,true>("1") + FixedPoint<int8,1,true>("1");
- Otherwise, if one FixedPoint number is signed and the other is unsigned, the signed FixedNumber will be the result type (This is honestly kind of made redundant by the previous rules, in here just in case)
- Otherwise, if one FixedPoint number has FlowGuard and the other doesn't, the FixedNumber with FlowGuard with FlowGuard
        // The right hand side has FlowGuard and the left hand side does not
        // Thus the right hand side's type is the result type
        FixedPoint<uint8,1,true> example = FixedPoint<uint8,1,false>("1") + FixedPoint<uint8,1,true>("1");
#### Overflow Checking
As a class based on integers, numbers overflowing in FixedPoint numbers are a more prevalent problem than with floating point numbers.
In order to counteract this assertions are made that are hit in the case that the build is in debug mode and if an operation would result in an overflowed/underflowed fixed point number.

These overflow checkers when is active has a pretty noticeable impact on performance. However, that's fine for now as these performance issues would be disabled on non-debug builds anyways.
###  Det Math Functions
Comprised of DetMathInt and DetMathFixed(soon to be implemented).

This is meant to act as replacement to standard library math functions that are reliant on non-deterministic floating point numbers.

###  Vectors
Custom math library meant to be plugged into outside libraries and their version of this.

##  Potential Future Additions
- Big Integer: A big integer library could be use  restrictions currently put on the string conversion functions for FixedPoint numbers.
- Rounding: If precision ever becomes an issue, use templated programming to created a version of all classes that rounds outside of string representation functions.
- Performance Testing: If performance ever becomes a problem with the math as a bottleneck, implement a performance testing library along with the existing test library.
- 3D Math: As 3D is explored more in gameplay logic for the Birdbrain Engine, more container classes like Matrices and Quaternions will be needed in order to represent that.

> Written with [StackEdit](https://stackedit.io/).