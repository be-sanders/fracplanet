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
  \brief Implementation for class ByteRGBA and class FloatRGBA.
*/

#include <iostream>
#include <sstream>

#include "rgb.h"

std::ostream& ByteRGBA::write(std::ostream& out) const
{
  return out << static_cast<uint>(r) << " " << static_cast<uint>(g) << " " << static_cast<uint>(b) << " " << static_cast<uint>(a);
}

const std::string ByteRGBA::format_comma() const
{
  std::ostringstream s;
  s << static_cast<uint>(r) << "," << static_cast<uint>(g) << "," << static_cast<uint>(b) << "," << static_cast<uint>(a);
  return s.str();
}

std::ostream& FloatRGBA::write(std::ostream& out) const
{
  return out << r << " " << g << " " << b << " " << a;
}

const std::string FloatRGBA::format_pov_rgb() const
{
  std::ostringstream s;
  s << "<" << r << "," << g << "," << b << ">";
  return s.str();
}

const std::string FloatRGBA::format_pov_rgbf() const
{
  std::ostringstream s;
  s << "<" << r << "," << g << "," << b << "," << 1.0f-a << ">";
  return s.str();
}
