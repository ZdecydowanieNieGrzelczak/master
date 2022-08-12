### Master thesis

The following repo is containing the code used in the research part of my master thesis.
To run it, please use the CMake configurations that are within CMakeLists.txt.
You are probably going to run it with OpenMP - if not, please adjust the THREAD_COUNT variable.

The code is definitely hacky (using raw pointers in 2022, etc.) but the speed was the core
property of the algorithm, causing me to prefer hacky-but-faster solutions over a more understandable ways of doing things.

A lot of things are commented, as a result of testing different configurations - generally, it is hard to understand unless
you worked 5 months with this, as I did - sorry about that.
