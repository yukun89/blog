#include <iostream>
#include "stack.hpp"

template <typename Type> stack<Type>::stack() {
        std::cerr << "Hello, stack " << this << "!" << std::endl;
}

template <typename Type> stack<Type>::~stack() {
        std::cerr << "Goodbye, stack " << this << "." << std::endl;
}
