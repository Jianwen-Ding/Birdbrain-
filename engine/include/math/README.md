
#  Deterministic Math Library
## Purpose
The point of such a math library is to be used within the rollback logic of the Birdbrain engine cross-platform deterministic, meaning that all given inputs to a function must result in the **exact same** output every single time on every machine. 

The most important implication the determinism requirement is that floating point numbers must not be part of any of the logic within this library. [IEEE 754](https://en.wikipedia.org/wiki/IEEE_754) (A standard on how floating point numbers/operations are implemented) isn't supported across all platforms leading to potential non-deterministic outcomes in math. Integer based math thus makes up the entirety of this math library. The only exception comes when classes within the library needs to be converted to floating point numbers to connect game logic to outside modules like rendering.

The standard library lacks cross-platform deterministic decimal math and is reliant on float for many of parts of their basic math functions such as square root and trigonometry functions. Thus necessitates this library.

However, outside game logic affected by rollback, floating points and standard library should be used. They are supported better by outside libraries and are more likely to be optimized and bug-free.

## Main Components

###  Fixed Point Numbers
Comprised of class FixedPoint

This is essentially this library's stand in for decimals since float cannot be used.
It is essentially a integer that represents the integer value divided by a set power of two.

#### Drawbacks
The overflow checkers when is active has a pretty noticeable impact on performance.
That's fine for now as the current purpose of it is for debugging potential overflow issues and preventing string literals from resulting in the wrong number without warning. The performance issues would be disabled on non-debug builds anyways.



Some conversions from FixedPoints 
###  Det Math Functions
Comprised of DetMathInt and DetMathFixed(soon to be implemented).

This is meant to act as replacement to standard library math functions that are reliant on non-deterministic floating point numbers.

###  Vectors

###  Potential Future Additions
#### Big Integer
A big integer library could get rid of some of the restrictions
#### Rounding
#### Performance Testing
#### 3D Math


> Written with [StackEdit](https://stackedit.io/).