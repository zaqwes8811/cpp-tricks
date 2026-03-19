module;
#include <iostream>
 
export module demo;
 
export namespace demo {
    void emit(const std::string& msg) {
        std::cout << msg << std::endl;
    }
}