/**
    part of ViNet

    \author stg7

    \brief roudimentary logging class, maybe use phrasit::log

    \date 27.05.2016

    Copyright 2016 Steve Göring

    This file is part of ViNet.

    ViNet is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    ViNet is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with ViNet.  If not, see <http://www.gnu.org/licenses/>.
**/

#ifndef LOG_H
#define LOG_H

#include <iostream>

#define LOG(x) std::cout << "\033[1;92m[Info]  \033[0m" << x << std::endl;
#define ERR(x) std::cout << "\033[1;91m[Error] \033[0m" << x << std::endl;
#define OUT(x) std::cout << "\033[1;94m" << x << "\033[0m" << std::endl;

#endif