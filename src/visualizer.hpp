/**
    part of ViNet

    \author stg7

    \brief opengl trace visualizer

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

#ifndef VISUALIZER_H
#define VISUALIZER_H

#include <iostream>

#include "utils/log.hpp"
#include "math/graph.hpp"
#include "gfx/SDLWindow.hpp"
#include "gfx/circle.hpp"
#include "gfx/quad.hpp"
#include "gfx/line.hpp"
#include "gfx/vgraph.hpp"


#include "build.hpp"
#include "visualizer.hpp"

namespace vinet {
    namespace gui {
        class GraphVisualizer : public gfx::SDLWindow {
         private:
            int _light;
            int _filter;
            GLfloat _xrot = 0;      /* X Rotation */
            GLfloat _yrot = 0;      /* Y Rotation */
            GLfloat _xspeed = 0;    /* X Rotation Speed */
            GLfloat _yspeed = 0;    /* Y Rotation Speed */
            GLfloat _z = -30.0f; /* Depth Into The Screen */

            math::Graph _graph;
            gfx::Circle _circle;
            gfx::Quad _quad;
            gfx::Line _line;

            gfx::VGraph _vgraph;

         protected:
            virtual void drawScene() {
                 /* Clear The Screen And The Depth Buffer */
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                /* Reset the view */
                glLoadIdentity();

                glTranslatef(0.0f, 0.0f, 0);

                glRotatef(_xrot, 1.0f, 0.0f, 0.0f); /* Rotate On The X Axis By xrot */
                glRotatef(_yrot, 0.0f, 1.0f, 0.0f); /* Rotate On The Y Axis By yrot */

                /*
                glColor4f(1, 0, 0, 1);
                _line.draw(100, 100, 300, 100);

                glColor4f(0, 1, 0, 1);
                for (int i = 100; i < 300; i += 40) {
                    _circle.draw(i, 100, 20);
                }
                */
                // _quad.draw(100, 100, 1);

                _vgraph.draw();


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
                    case SDLK_r:
                        _vgraph.init(800, 600, _graph);
                        break;
                    case SDLK_s:
                        saveScreenshotBMP("screen.bmp");
                        break;
                    case SDLK_u:
                        _vgraph.dooIT();
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

                _graph.add_edge("A", "B");
                _graph.add_edge("A", "C");
                _graph.add_edge("C", "D");
                _graph.add_edge("D", "E");
                _graph.add_edge("D", "F");

                _vgraph.init(800, 600, _graph);
            }

            virtual int init() {
                SDLWindow::init();
                SDL_WM_SetCaption("TITLE", "TITLE");
                return 0;
            }

        };
    }
}
#endif
