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
/* Copyright (C) 1998 T Day */

#include <iostream>
#include <sstream>

#include "rgb.h"

/*! State of pov_mode() determines output format
 */
std::ostream& FloatRGBA::write(std::ostream& out) const
{
  return out << r << " " << g << " " << b << " " << a;
}

const std::string format_pov_rgb(const FloatRGBA& c)
{
  std::ostringstream msg;
  msg << "<" << c.r << "," << c.g << "," << c.b << ">";
  return msg.str();
}

const std::string format_pov_rgbf(const FloatRGBA& c)
{
  std::ostringstream msg;
  msg << "<" << c.r << "," << c.g << "," << c.b << "," << 1.0f-c.a << ">";
  return msg.str();
}
