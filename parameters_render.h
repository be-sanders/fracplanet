// Source file for fracplanet
// Copyright (C) 2002,2003 Tim Day
/*
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

/*! \file
  \brief Interface for class ParametersRender.
*/

#ifndef _parameters_render_h_
#define _parameters_render_h_

#include "useful.h"

class Notifiable;

//! Aggregates controllable parameters for all things related to OpenGL rendering.
class ParametersRender
{
public:
  //! Flag selecting OpenGL wireframe rendering.
  bool wireframe;

  //! Render via display list
  bool display_list;

  //! Target frame rate
  float fps_target;
  
  //! Place to send status reports
  Notifiable* notify;

  //! Constructor.
  ParametersRender()
    :wireframe(false)
    ,display_list(false)
    ,fps_target(100.0)
    ,notify(0)
  {}

  //! Destructor.
  ~ParametersRender()
    {}
};

// Abstract mixin class for classes with a report(const std::string&) method.
class Notifiable
{
 public:

  virtual void notify(const std::string&)
    =0;
};

#endif
