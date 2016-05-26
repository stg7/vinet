/**
    part of TraceView

    \author stg7

    \brief visualizer

    \date 13.11.2015

    Copyright 2016 Steve Göring

    This file is part of TraceView.

    TraceView is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    TraceView is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with TraceView.  If not, see <http://www.gnu.org/licenses/>.
**/
#include <iostream>

#include <boost/program_options.hpp>

#define LOG(x) std::cout << x << std::endl;
#define ERR(x) std::cout << x << std::endl;

#include "math/graph.hpp"
#include "gfx/SDLWindow.hpp"
#include "gfx/circle.hpp"
#include "gfx/quad.hpp"

namespace traceview {
    namespace gui {
        class GraphVisualizer : public SDLWindow {
         private:
            int _light;
            int _filter;
            GLfloat _xrot = 0;      /* X Rotation */
            GLfloat _yrot = 0;      /* Y Rotation */
            GLfloat _xspeed = 0;    /* X Rotation Speed */
            GLfloat _yspeed = 0;    /* Y Rotation Speed */
            GLfloat _z = -30.0f; /* Depth Into The Screen */

            gfx::Circle _circle;
            gfx::Quad _quad;

            math::Graph _graph;

         protected:
            virtual void drawScene() {
                 /* Clear The Screen And The Depth Buffer */
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                /* Reset the view */
                glLoadIdentity();

                /* Translate Into/Out Of The Screen By z */
                glTranslatef(0.0f, 0.0f, _z);

                glRotatef(_xrot, 1.0f, 0.0f, 0.0f); /* Rotate On The X Axis By xrot */
                glRotatef(_yrot, 0.0f, 1.0f, 0.0f); /* Rotate On The Y Axis By yrot */

                glColor4f(0, 1, 0, 1);
                for (int i = -10; i < 10; i += 2) {
                    _circle.draw(i, 0, 1);
                }

                // _quad.draw(100, 100, 1);

                _xrot += _xspeed; /* Add xspeed To xrot */
                _yrot += _yspeed; /* Add yspeed To yrot */
            }

            virtual void handleKeyPress(SDL_keysym* keysym) {
                switch(keysym->sym) {
                    case SDLK_ESCAPE:
                        /* ESC key was pressed */
                        quit();
                        break;
                    case SDLK_f:
                        /* 'f' key was pressed
                         * this pages through the different filters
                         */
                         ++_filter;
                        _filter %= 3;
                        break;
                    case SDLK_l:
                        /* 'l' key was pressed
                         * this toggles the light
                         */
                        _light = !_light;
                        if(!_light)
                            glDisable(GL_LIGHTING);
                        else
                            glEnable(GL_LIGHTING);
                        break;
                    case SDLK_PAGEUP:
                        /* PageUp key was pressed
                         * this zooms into the scene
                         */
                        _z -= 1.0f;
                        break;
                    case SDLK_PAGEDOWN:
                        /* PageDown key was pressed
                         * this zooms out of the scene
                         */
                        _z += 1.0f;
                        break;
                    case SDLK_UP:
                        /* Up arrow key was pressed
                         * this affects the x rotation
                         */
                        _xspeed -= 0.01f;
                        break;
                    case SDLK_DOWN:
                        /* Down arrow key was pressed
                         * this affects the x rotation
                         */
                        _xspeed += 0.01f;
                        break;
                    case SDLK_RIGHT:
                        /* Right arrow key was pressed
                         * this affects the y rotation
                         */
                        _yspeed += 0.01f;
                        break;
                    case SDLK_LEFT:
                        /* Left arrow key was pressed
                         * this affects the y rotation
                         */
                        _yspeed -= 0.01f;
                        break;
                    case SDLK_F1:
                        /* 'f1' key was pressed
                         * this toggles fullscreen mode
                         */
                         toggleFullscreen();
                        break;
                    case SDLK_h:
                        break;
                    default:
                        break;
                }

            }
         public:
            GraphVisualizer() {
                _filter = 0;
                _light = false;

                _graph.addEdge("A", "B");
            }

            virtual int init() {
                SDLWindow::init();
                SDL_WM_SetCaption("TITLE", "TITLE");
                return 0;
            }

        };
    }
}
/**
    TraceView: visualizer
**/
int main(int argc, const char* argv[]) {
    // TODO(stg7) do some command line arg parsing with boost
    LOG("[..] start")
    traceview::gui::GraphVisualizer window;
    window.init();
    window.loop();
    return 0;
}
