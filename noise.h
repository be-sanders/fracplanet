// Source file for fracplanet
// Copyright (C) 2005 Tim Day
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
  \brief Interface to Noise class.
*/

#ifndef _noise_h_
#define _noise_h_

#include "useful.h"
#include "xyz.h"

#include <vector>

//! Perlin noise generator.
class Noise
{
public:
  //! Constructor.
  Noise(uint seed);

  //! Return noise value at a point.
  const float operator()(const XYZ& p) const;

protected:
  //! Number of table entries.
  enum {N=256};
  
  int _p[N+N+2];
  XYZ _g[N+N+2];
};

//! Multiscale noise generator.
class MultiscaleNoise
{
 public:
  //! Constructor.
  MultiscaleNoise(uint seed,uint terms,float decay);

  //! Destructor.
  ~MultiscaleNoise();

  //! Return noise value at a point.
  const float operator()(const XYZ& p) const;

 protected:
  //! Noise functions for each frequency.
  std::vector<const Noise*> noise;

  //! Amplitude for each frequency.
  std::vector<float> amplitude;
};

#endif
