/**
    part of ViNet

    \author stg7

    \brief draw a quad with opengl

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

#ifndef GFX_QUAD_H
#define GFX_QUAD_H

#include <iostream>

// opengl & sdl
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL.h>

#include "utils/log.hpp"

namespace vinet {
    namespace gfx {
        class Quad {
         private:
            int _x;
            int _y;
            int _r;
         public:
            Quad() { }
            void draw(int x, int y, int r) {
                glBegin(GL_QUADS);
                  glColor4f(1, 0, 0, 1);
                  glVertex3f(-1.0f, -1.0f,  1.0f);

                  glColor4f(1, 0, 0, 1);
                  glVertex3f(1.0f, -1.0f,  1.0f);

                  glColor4f(1, 0, 0, 1);
                  glVertex3f(1.0f,  1.0f,  1.0f);

                  glColor4f(1, 0, 0, 1);
                  glVertex3f(-1.0f,  1.0f, 1.0f);
                glEnd();

            }
        };
    }
}
#endif