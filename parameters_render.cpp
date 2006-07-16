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

#include "parameters_render.h"

#include "xyz.h"

boost::program_options::options_description ParametersRender::options()
{
  boost::program_options::options_description desc("Render options");
  desc.add_options()
    ("display-list,d","use display list rendering")
    ("wireframe,w","render in wireframe mode")
    ("invert-mouse-y,y","invert mouse-y in flight mode");
  return desc;
}

ParametersRender::ParametersRender(const boost::program_options::variables_map& opts)
  :wireframe(opts.count("wireframe"))
  ,display_list(opts.count("display-list"))
  ,joystick_mouse(!opts.count("invert-mouse-y"))
  ,ambient(0.1f)
  ,illumination_azimuth(-M_PI/3)
  ,illumination_elevation(M_PI/6)
  ,background_colour_low(0.25f,0.25f,1.0f,0.0f)
  ,background_colour_high(0.0f,0.0f,0.0f,0.0f)
  ,fps_target(75.0f)
  ,notify(0)
{}

ParametersRender::~ParametersRender()
{}

const XYZ ParametersRender::illumination_direction() const
{
  return XYZ
    (
     cos(illumination_azimuth)*cos(illumination_elevation),
     sin(illumination_azimuth)*cos(illumination_elevation),
     sin(illumination_elevation)
     );
}
