This project consists of macros and functions useful for debugging and profiling C++ programs in circumstances where running a proper debugger or profiler is impractical.

How to use the profile
======================

Profiling is achieved through adding the macro JEB_PROFILE() to every function that you want included in the profiler report. In addition a call to Profiler::write must be added to print the profiler report once the profiling is over. Both the macro and the function are defined in 
