// Source file for fracplanet
// Copyright (C) 2006 Tim Day
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

#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/options_description.hpp>

#include "useful.h"

#include "notifiable.h"
#include "rgb.h"

class XYZ;

//! Aggregates controllable parameters for all things related to OpenGL rendering.
class ParametersRender
{
public:
  //! Flag selecting OpenGL wireframe rendering.
  bool wireframe;

  //! Render via display list
  bool display_list;

  //! Joystick mode for flight
  bool joystick_mouse;

  //! Amount of global ambient illumination (0-1)
  float ambient;

  //! Controls illumination direction.
  float illumination_azimuth;

  //! Controls illumination direction.
  float illumination_elevation;

  //! Background colour at low altitude
  FloatRGBA background_colour_low;

  //! Background colour at high altitude
  FloatRGBA background_colour_high;

  //! Target frame rate
  float fps_target;
  
  //! Place to send status reports
  Notifiable* notify;

  //! Constructor.
  ParametersRender(const boost::program_options::variables_map& opts);

  //! Destructor.
  ~ParametersRender();

  //! Illumination direction computed from azimuth and elevation angles
  const XYZ illumination_direction() const;

  static boost::program_options::options_description options();
};

#endif
