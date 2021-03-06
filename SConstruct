# -*- coding: utf8 -*-
"""
    part of ViNet

    author: Steve Göring
    contact: stg7@gmx.de
    2016

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
"""
import os
import sys
import glob
import multiprocessing

def terminal_width():
    rows, columns = os.popen('stty size', 'r').read().split()
    return int(columns)

def colorgreen(m):
    return "\033[92m" + m + "\033[0m"

def lInfo(msg):
    print(colorgreen("[INFO ] ") + str(msg))

def line():
    print("-" * terminal_width())

def dline():
    print("=" * terminal_width())

SetOption('num_jobs', multiprocessing.cpu_count()) # build with all aviable cpu cores/threads

line()
lInfo("build target(s): " + str(map(str, BUILD_TARGETS)))
lInfo("run with cpus  : " + str(multiprocessing.cpu_count()))
line()

stylechecker = Builder(action='./test_convention.sh $SOURCES', suffix='', src_suffix='')
valgrind = Builder(action='valgrind ./$SOURCES', suffix='', src_suffix='')

lInfo("check local installed libs")
dline()
os.system("./prepare.sh")
line()
lInfo("add build infos")
dline()
os.system("./addbuildinfos.sh")
line()

env = Environment(CPPPATH = ["src/", "libs/"],
                  BUILDERS = {'StyleCheck' : stylechecker, 'Valgrind': valgrind})

# include local installed libs
libspath = 'libs/'
libs = ["boost"]

env.Append(
    LIBPATH=['.'] + [libspath + x +  "/build/lib" for x in libs],
    CPPPATH=[libspath + x + "/build/include" for x in libs]
)

env.Append(LIBS = ['GL','GLU','SDL','SDL_image'])

env.Append(LINKFLAGS=['-pthread',
        '-Wl,--rpath,' + libspath + 'boost/build/lib',
        ])

env.Decider('MD5')

conf = Configure(env)

needed_libs = [
    'stdc++fs',
    'boost_unit_test_framework',
    'boost_program_options',
    'boost_system',
    'boost_iostreams',
    'GL',
    'GLU',
    'SDL',
    'SDL_image']
env.ParseConfig('sdl-config --cflags --libs')


for lib in needed_libs:
    if not conf.CheckLib(lib, language="c++"):
        print "Unable to find lib: " + lib + ". Exiting."
        exit(-1)

# TODO: do it automatically
needed_headers = [
    'boost/program_options.hpp',
    'boost/test/unit_test.hpp',
    'GL/gl.h',
    'GL/glu.h',
    'SDL.h'] #

for header in needed_headers:
    if not conf.CheckCXXHeader(header):
        print "Unable to find header: " + header + ". Exiting."
        sys.exit(-1)

env.Append(CXXFLAGS=['-std=c++14'])

# if you call scons debug=1 debug build is activated
if ARGUMENTS.get('debug', 0) != 0:
    # more checks
    env.Append(CXXFLAGS=['-Wall','-pedantic-errors', '-g'])
    # "no" optimization
    env.Append(CXXFLAGS=['-O0'])
else:
    env.Append(CXXFLAGS=['-march=native']) # use native architecture
    #env.Append(CXXFLAGS=['-mavx'])
    env.Append(CXXFLAGS=['-Wall'])
    env.Append(CXXFLAGS=['-O3'])

    # loop unrolling and link time optimization, options should be tested
    env.Append(CXXFLAGS=['-funroll-loops', '-flto', '-fwhole-program'])


testcases = set(glob.glob("src/tests/*.cpp"))

header = set(glob.glob("src/*.hpp") +  glob.glob("src/*/*.hpp"))
sources = set(glob.glob("src/*.cpp") + glob.glob("src/*/*.cpp")) - set(["src/tracer.cpp", "src/visualizer.cpp", "src/tests/unittests.cpp"]) - testcases


# check code conventions and build programm
#env.StyleCheck("conventions", ["src/main.cpp"] + list(sources) + list(header) + list(testcases))


tracer = env.Program('tracer', ["src/tracer.cpp"] + list(sources))
visualizer = env.Program('visualizer', ["src/visualizer.cpp"] + list(sources))
#unittest = env.Program('unittest', ["src/tests/unittests.cpp"] + list(sources))

Default(tracer, visualizer)#, unittest)