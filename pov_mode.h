// Source file for fracplanet
// Copyright (C) 2002 Tim Day
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
#ifndef _pov_mode_h_
#define _pov_mode_h_

#include "useful.h"

//! Placeholder for POV-format output flag.
/*! Originally the intention was that classes with a POV-format output option (e.g FloatRGB, XYZ) 
    would inherit from this class, but that seems to be overkill.
 */
class POVMode
{
 protected:
  //! Whether co-ordinates are output in POV-ray format.
  static bool _pov_mode;

 public:
  POVMode()
    {}
  ~POVMode()
    {}

  //! Returns true if POV output format enabled
  static const bool pov_mode()
    {
      return _pov_mode;
    }

  //! Enable/disable POV output format.
  static void pov_mode(bool m)
    {
      _pov_mode=m;
    }
};

#endif
