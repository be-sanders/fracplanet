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
#include "rgb.h"

//! Class to support specialisation for particular pixel formats
template <typename T> class PixelTraits
{
  typedef T ComputeType;
};

template<> class PixelTraits<uchar>
{
 public:
  typedef float ComputeType;
};

template<> class PixelTraits<ByteRGBA>
{
 public:
  typedef FloatRGBA ComputeType;
};

//! Class for 2D raster images of a specified type.
/*! Completely gratuitously implemented using boost multiarray type.
  Assumes explicit instantiation.
  /todo Multiarray has GOT TO GO.  Far too heavyweight.
 */
template <typename T> class Image
{
 public:
  typedef boost::multi_array<T,2> RasterType;
  typedef T PixelType;
  typedef typename PixelTraits<T>::ComputeType ComputeType;

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

  const RasterType& raster() const
    {
      return _raster;
    }
  RasterType& raster()
    {
      return _raster;
    }

  const T& operator()(uint x,uint y) const
    {
      return _raster[y][x];
    }
  T& operator()(uint x,uint y)
    {
      return _raster[y][x];
    }

  //! Clear the image to a constant value.
  void clear(const T& v);

  //! Fill a line segment on the given half-open range [x0,x1), interpolating between the two given values.
  void scan(uint y,float x0,const ComputeType& v0,float x1,const ComputeType& v1);

  void write_ppm(std::ostream&) const;
  void write_pgm(std::ostream&) const;

 private:
  RasterType _raster;
};

template class Image<uchar>;
template class Image<ByteRGBA>;


#endif


