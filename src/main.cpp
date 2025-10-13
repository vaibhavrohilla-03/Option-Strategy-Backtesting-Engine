#include <iostream>

int main() {
    long cpp_version = __cplusplus;

    if (cpp_version == 199711L) {
        std::cout << "C++98/03" << std::endl;
    }
    else if (cpp_version == 201103L) {
        std::cout << "C++11" << std::endl;
    }
    else if (cpp_version == 201402L) {
        std::cout << "C++14" << std::endl;
    }
    else if (cpp_version == 201703L) {
        std::cout << "C++17" << std::endl;
    }
    else if (cpp_version == 202002L) {
        std::cout << "C++20" << std::endl;
    }
    else if (cpp_version > 202002L) {
        std::cout << "C++23 or later" << std::endl;
    }
    else {
        std::cout << "Unknown C++ standard version: " << cpp_version << std::endl;
    }
    return 0;
}