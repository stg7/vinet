#ifndef LOG_H
#define LOG_H

#include <iostream>

#define LOG(x) std::cout << "\033[1;92m[Info]  \033[0m" << x << std::endl;
#define ERR(x) std::cout << "\033[1;91m[Error] \033[0m" << x << std::endl;
#define OUT(x) std::cout << "\033[1;94m" << x << "\033[0m" << std::endl;

#endif