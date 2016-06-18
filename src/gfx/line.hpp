/**
    part of ViNet

    \author stg7

    \brief draw lines with opengl

    \date 13.06.2016

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

#ifndef GFX_LINE_H
#define GFX_LINE_H

#include <iostream>
#include <cmath>

// opengl & sdl
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL.h>

#include "utils/log.hpp"

namespace vinet {
    namespace gfx {
        class Line {
         public:
            Line() { }
            void draw(int x, int y, int _x, int _y, float thickness=10) {
                glLineWidth(thickness);
                glBegin(GL_LINES);
                    glVertex2f(x, y);
                    glVertex2f(_x, _y);
                glEnd();
            }
        };
    }
}
#endif