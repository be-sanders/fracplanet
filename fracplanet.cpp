// Source file for fracplanet
// Copyright (C) 2002 Tim Day
/*! \page License License

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

/*! \mainpage Fracplanet : fractal terrain generator

  \author Tim Day 

  \section introduction Introduction
  "Fracplanet" is an interactive tool for generating fractal planets and terrains.
  It can output the generated meshes to files suitable for use by POV-Ray.

  \todo Figure out how to build RPMs.
  \todo Add "flight simulator" mode to get in close.
  \todo Add colour-chooser for picking terrain colours.
  \todo Add Unix "man" page.
  \todo Improve code documentation.

  \section Known bugs
  - I recently installed the newly released Nvidia drivers for RH8.0 (was using Mesa before), and this app now sometimes causes the whole screen to jitter left-right by perhaps one pixel.  It only seems to be a problem at high frame rates (low levels of subdivision).  The fact that it affects the whole screen suggests its not me, but then I can't see any similar problems in e.g the OpenGL screensavers.  Hmmm.

  \section history History
  This was written for fun after a year off from computing, just to check I could still find my way around emacs, C++, STL, linux, OpenGL, doxygen etc.

  What did I learn ?  Well:
    - Qt is wonderful, but it's a shame it looks so ugly amongst all my themed gtk windows.  Next time I write a GUI app I'll use gtkmm and see how it works out.
    - Using emacs is like riding a bike; you never forget.  
 */

#include <qapplication.h>

#include "fracplanet_main.h"

//! Application code
/*! Currently this simply creates a TriangleMesh object of some sort,
  then passes it to a viewer.
 */
int main(int argc,char* argv[])
{
  QApplication app(argc,argv);

  FracplanetMain*const main_widget=new FracplanetMain(0);

  main_widget->resize(800,600);
  app.setMainWidget(main_widget);
  main_widget->show();

  std::cerr << "Commencing main loop...\n";

  return app.exec();
}
