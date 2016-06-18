/**
    part of ViNet

    \author stg7

    \brief small helper functions, mostly copied from phrasit,
        TODO(stg7): think about extracting of those helper methods to a extra git repository

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

#ifndef HELPER_H
#define HELPER_H

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
#include <sstream>

namespace helper {
    /*
    *   perform a system call and get output as string
    *   \param cmd command to run (e.g. "ls -l /")
    *   \return output of command
    */
    std::string ssystem(const char* cmd) {
        char buffer[128];
        std::string result = "";
        std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
        if (!pipe)
            throw std::runtime_error("popen() failed!");
        while (!feof(pipe.get())) {
            if (fgets(buffer, 128, pipe.get()) != NULL)
                result += buffer;
        }
        return result;
    }
    // from phrasit/helper.hpp:
    inline std::vector<std::string>& split(const std::string& s, char delim, std::vector<std::string>& elems) {
        std::stringstream ss(s);
        std::string item;
        while (std::getline(ss, item, delim)) {
            elems.emplace_back(item);
        }
        return elems;
    }

    /*
    *   split a string by delimiter and return result in a vector of strings
    *   \param s string to split
    *   \param delim using this character as a delimiter
    *   \return splitted strings as a vector
    */
    inline std::vector<std::string> split(const std::string& s, char delim) {
        std::vector<std::string> elems;
        split(s, delim, elems);
        return elems;
    }
    inline std::string join(const std::vector<std::string> values, const std::string& connector) {
        std::string res = "";

        for (size_t i = 0; i < values.size(); i++) {
            res += values[i];
            if (i != values.size() - 1) {
                res += connector;
            }
        }
        return res;
    }
    /*
    *   trim spaces from a string
    *
    *   \param s string for trimming
    *   \return string s without leading/trailing spaces
    */
    inline std::string trim(const std::string& s) {
        std::string::const_iterator it = s.begin();
        while (it != s.end() && *it == ' ') {
            it++;
        }

        std::string::const_reverse_iterator rit = s.rbegin();
        while (rit.base() != it && *rit == ' ') {
            rit++;
        }

        return std::string(it, rit.base());
    }
}

#endif
