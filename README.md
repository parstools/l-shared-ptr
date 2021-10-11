# l-shared-ptr
Lighweight shared pointers

A shared-ptr have 16 bytes per pointer (in 64-bit environment) and alloc block. This pointers uses only 4 bytes in object begin. But it is unusable in C++, because set of functions can't be changed to class because it need &this address. This functions can be a simple library for possible own programic language, which manage memory like Swift.
