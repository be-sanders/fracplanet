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
  \brief Interface (and implementation) for templated Image class.
*/

#ifndef _image_h_
#define _image_h_

#include <boost/multi_array.hpp>
#include <iosfwd>

class ByteRGBA;

#include "useful.h"

//! Class for 2D raster images of a specified type.
/*! Completely gratuitously implemented using boost multiarray type.
 */
template <typename T> class Image
{
 public:
  Image(uint width,uint height)
    :_raster(boost::extents[height][width])
    {}
  ~Image()
    {}

  const uint width() const
    {
      return _raster.shape()[1];
    }
  const uint height() const
    {
      return _raster.shape()[0];
    }

  const boost::multi_array<T,2>& raster() const
    {
      return _raster;
    }
  boost::multi_array<T,2>& raster()
    {
      return _raster;
    }

  const T& operator()(int x,int y) const
    {
      return _raster[y][x];
    }
  T& operator()(int x,int y)
    {
      return _raster[y][x];
    }

  void clear(const T& v)
    {
      for (typename RasterType::iterator row=_raster.begin();row!=_raster.end();row++)
	std::fill((*row).begin(),(*row).end(),v);
    }

  void write_ppm(std::ostream&) const;
  void write_pgm(std::ostream&) const;

 private:
  typedef boost::multi_array<T,2> RasterType;
  RasterType _raster;
};

template <> extern void Image<ByteRGBA>::write_ppm(std::ostream&) const;

template <> extern void Image<uchar>::write_pgm(std::ostream&) const;

#endif


