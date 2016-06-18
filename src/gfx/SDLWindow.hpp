/**
    part of ViNet

    \author stg7

    \brief sdl base class for rendering an opengl window

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

#ifndef SDLWINDOW_H
#define SDLWINDOW_H

#include <iostream>

// opengl & sdl
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL.h>

#include "utils/log.hpp"

namespace vinet {
    namespace gfx {
        GLfloat LightAmbient[]  = { 0.5f, 0.5f, 0.5f, 1.0f };
        GLfloat LightDiffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
        GLfloat LightPosition[] = { 0.0f, 0.0f, 2.0f, 1.0f };
        class SDLWindow {
         private:
            SDL_Surface* _surface;

            int _done;
            int _Frames;
            int _T0;

             /* Flags to pass to SDL_SetVideoMode */
            int getVideoFlags() {
                 /* the flags to pass to SDL_SetVideoMode */
                int videoFlags  = SDL_OPENGL;          /* Enable OpenGL in SDL */
                videoFlags |= SDL_GL_DOUBLEBUFFER; /* Enable double buffering */
                videoFlags |= SDL_HWPALETTE;       /* Store the palette in hardware */
                videoFlags |= SDL_RESIZABLE;       /* Enable window resizing */


                /* Fetch the video info */
                const SDL_VideoInfo* videoInfo = SDL_GetVideoInfo();

                if (!videoInfo) {
                    ERR("Video query failed " << SDL_GetError());
                    return 0;
                }

                /* This checks to see if surfaces can be stored in memory */
                videoFlags |= ( videoInfo->hw_available) ? SDL_HWSURFACE : SDL_SWSURFACE;

                /* This checks if hardware blits can be done */
                if( videoInfo->blit_hw)
                    videoFlags |= SDL_HWACCEL;

                return videoFlags;
            }

            /* function to reset our viewport after a window resize */
            bool resizeWindow(int width, int height) {
                /* Height / width ration */
                //GLfloat ratio;

                /* Protect against a divide by zero */
                /*
                if( height == 0)
                    height = 1;

                ratio = width / (GLfloat) height;
                */
                /* Setup our viewport. */
                glViewport(0, 0, (GLint) width, (GLint) height);

                /* change to the projection matrix and set our viewing volume. */
                glMatrixMode(GL_PROJECTION);
                glLoadIdentity();

                /* Set our perspective */
                glOrtho(0, 800, 600, 0, 1, -1);
                //        gluPerspective(45.0f, ratio, 0.1f, 100.0f);

                /* Make sure we're chaning the model view and not the projection */
                glMatrixMode(GL_MODELVIEW);

                /* Reset The View */
                glLoadIdentity();

                return true;
            }

         protected:

            virtual void drawScene() {}
            virtual void handleKeyPress(SDL_keysym* keysym) {}

         public:
            SDLWindow() {
                _done = false;
                _Frames = 0;
                _T0 = 0;
            }
            virtual ~SDLWindow() {
                SDL_FreeSurface(_surface);
                SDL_Quit();
            }

            void initGL() {

                glShadeModel(GL_SMOOTH); // enable smooth shading
                glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

                glClearDepth(1.0f); // depth buffer setup
                glEnable(GL_DEPTH_TEST); // enables Depth Testing
                glDepthFunc(GL_LEQUAL); // set depth test function


                glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // set perspective calculations

                // light settings
                glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
                glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
                glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);

                glEnable(GL_MULTISAMPLE);

                // enable light1
                glEnable(GL_LIGHT1);

            }

            virtual int init() {
                LOG("[SDL] initialize SDL settings");

                SDL_EnableUNICODE(1);
                SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

                // initialize SDL
                if(SDL_Init(SDL_INIT_VIDEO) < 0) {
                    ERR("ERROR: [SDL] Video initialization failed: " << SDL_GetError());
                    return -1;
                }

                // OpenGL double buffering
                SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
                // antialasing things
                SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
                SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
                SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

                // get a SDL surface
                _surface = SDL_SetVideoMode(800, 600, 32, getVideoFlags());

                // valid surface?
                if(!_surface) {
                    ERR("ERROR: [SDL] surface allocation failed: " << SDL_GetError());
                    return -1;
                }

                // initialize OpenGL
                initGL();

                // resize the initial windowsize
                resizeWindow(800, 600);

                return 0;
            }

            void loop() {
                SDL_Event event;

                while(!_done) {

                    // handle each event in queue
                    while(SDL_PollEvent(&event)) {
                        switch(event.type) {
                            case SDL_VIDEORESIZE: // resize event

                                _surface = SDL_SetVideoMode(event.resize.w, event.resize.h, 16, getVideoFlags());
                                if(!_surface) {
                                    ERR("ERROR: [SDL] [surface] :could not get a surface after resize: " << SDL_GetError());
                                    return ;
                                }
                                resizeWindow(event.resize.w, event.resize.h);
                                break;
                            case SDL_KEYDOWN: // key pressed
                                handleKeyPress(&event.key.keysym);
                                break;
                            case SDL_QUIT: // window close button pressed
                                _done = 1;
                                break;
                            default:
                                break;
                        }

                    }

                    // draw it all
                    drawScene();

                    SDL_GL_SwapBuffers();

                    // calc frames per second
                    _Frames++;
                    GLint t = SDL_GetTicks();
                    if( t - _T0 >= 5000) {
                        GLfloat seconds = (t - _T0) / 1000.0;
                        int fps = _Frames / (int) seconds;

                        LOG("[FPS] " << _Frames << " frames in " << seconds << "s, fps=" << fps);
                        _T0 = t;
                        _Frames = 0;
                    }
                    // frames done
                }
            }
            bool saveScreenshotBMP(const std::string filename) {
                int w = 800;
                int h = 600;
                int x = 0;
                int y = 0;
                unsigned char* pixels = new unsigned char[w * h * 4]; // 4 bytes for RGBA
                glReadPixels(x, y, w, h, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

                SDL_Surface* surf = SDL_CreateRGBSurfaceFrom(pixels, w, h, 8 * 4, w * 4, 0, 0, 0, 0);
                SDL_SaveBMP(surf, filename.c_str());

                SDL_FreeSurface(surf);
                delete [] pixels;
                return true;
            }

            void quit() {
                _done = 1;
            }
            void toggleFullscreen() {
                SDL_WM_ToggleFullScreen(_surface);
            }


        };
    }
}
#endif
