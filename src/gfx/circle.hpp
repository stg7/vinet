#ifndef GFX_CIRCLE_H
#define GFX_CIRCLE_H

#include <iostream>
#include <cmath>

// opengl & sdl
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL.h>

#include "utils/log.hpp"

namespace traceview {
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