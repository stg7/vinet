/**
    part of ViNet

    \author stg7

    \brief draw circles with opengl

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

#ifndef GFX_CIRCLE_H
#define GFX_CIRCLE_H

#include <iostream>
#include <cmath>

// opengl & sdl
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL.h>

#include "utils/log.hpp"

namespace vinet {
    namespace gfx {
        class Circle {
         private:
            int _x;
            int _y;
            int _r;
         public:
            Circle() { }
            void draw(int x, int y, int r, int num_segments=40) {
                GLfloat twicePi = 2.0f * 3.1415926;
                glBegin(GL_TRIANGLE_FAN);
                    glVertex2f(x, y); // center of circle
                    for(int i = 0; i <= num_segments;i++) {
                        glVertex2f(
                            x + (r * cos(i *  twicePi / num_segments)),
                            y + (r * sin(i * twicePi / num_segments))
                        );
                    }
                glEnd();
            }
        };
    }
}
#endif