#ifndef GFX_QUAD_H
#define GFX_QUAD_H

#include <iostream>

// opengl & sdl
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL.h>


namespace traceview {
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