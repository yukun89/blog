[Question](https://stackoverflow.com/questions/1724036/splitting-templated-c-classes-into-hpp-cpp-files-is-it-possible.)

首先，模板是一种编译时技术，不是一种运行时技术。对于模板类来说，编译器需要在在编译结束时，知道模板类的大小; 对于模板函数来说, main.cpp 在编译时，对应的符号表是undef的状态。而由于模板函数没有实例化，所以会在链接阶段报错.
