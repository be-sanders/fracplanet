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
  \brief Interface for class ParametersNoise.
*/

#ifndef _parameters_noise_h_
#define _parameters_noise_h_

#include "useful.h"

class ParametersNoise
{
 public:
  //! Number of Perlin noise terms
  uint terms;

  //! Frequency of 1st noise term
  float frequency;

  //! Amplitude of 1st noise term
  float amplitude;

  //! Amplitude decay rate for successive terms
  float amplitude_decay;

  //! Construct with given number of terms
  ParametersNoise(uint);
};

#endif
