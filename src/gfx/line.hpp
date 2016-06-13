#ifndef GFX_LINE_H
#define GFX_LINE_H

#include <iostream>
#include <cmath>

// opengl & sdl
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL.h>

#include "utils/log.hpp"

namespace traceview {
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