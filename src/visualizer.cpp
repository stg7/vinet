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
#include <boost/program_options.hpp>
#include "visualizer.hpp"

/**
    TraceView: visualizer
**/
int main(int argc, const char* argv[]) {
    // TODO(stg7) do some command line arg parsing with boost
    LOG("[..] start");
    OUT("[version] " << version);
    OUT("[branch]  " << branch);

    traceview::gui::GraphVisualizer window;
    window.init();
    window.loop();
    return 0;
}
