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
  \brief Interface for class ParametersSave.
*/

#ifndef _parameters_save_h_
#define _parameters_save_h_

#include "useful.h"

//! Aggregates controllable parameters for all things related to save.
class ParametersSave
{
 public:
  //! Whether to emit an atmosphere object to POV file.
  bool atmosphere;

  //! Whether to emit a single sea-level object to POV file.
  bool sea_object;
  
  //! Base filename for save.
  std::string basename;

  //! Constructor.
  ParametersSave()
    :atmosphere(false)
    ,sea_object(true)
    ,basename("terrain")
    {}

  //! Destructor.
  ~ParametersSave()
    {}
};

#endif
