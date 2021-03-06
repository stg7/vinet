#!/bin/bash
#   Script for style checking
#
#   This file is part of ViNet.
#
#   ViNet is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   ViNet is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with ViNet.  If not, see <http://www.gnu.org/licenses/>.

cpplint() {
    python2 ./tools/cpplint.py "$@"
}
cpplint --extensions=hpp,cpp \
    --filter=-runtime/references,-build/include_order,-runtime/indentation_namespace,-runtime/int,-readability/namespace,-runtime/explicit,-build/namespaces,-whitespace/line_length,-readability/streams "$@"
