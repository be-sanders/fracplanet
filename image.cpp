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
  \brief Implementation for templated Image class.
*/

#include "image.h"

#include "rgb.h"
#include <iostream>

template <typename T> void Raster<T>::fill(const T& v)
{
  if (contiguous())
    {
      std::fill(contiguous_begin(),contiguous_end(),v);
    }
  else 
    {
      for (RowIterator row=row_begin();row!=row_end();++row)
	std::fill((*row).begin(),(*row).end(),v);
    }
}

template <typename T> const typename Raster<T>::ScalarType Raster<T>::maximum_scalar_pixel_value() const
{
  ScalarType m(scalar(*_data));
  for (ConstRowIterator row=row_begin();row!=row_end();++row)
    for (const T* it=row->begin();it!=row->end();++it)
      {
	const ScalarType v(scalar(*it));
	if (v>m) m=v;
      }
  return m;
}

/*! Scan x0 to x1 into image.
  Pixel centres are at 0.5 , so x0=0.75 goes to pixel 1.
  Rightmost pixel is at width()-0.5.
*/
template <typename T> void Raster<T>::scan(uint y,float x0,const ComputeType& v0,float x1,const ComputeType& v1)
{
  //if (!(x0<=x1)) {std::cerr << y << ":" << x0 << " " << x1 << "\n";std::cerr.flush();}
  assert(x0<=x1);
  
  if (x1<0.5f || width()-0.5f<x0) return;  // Early out for spans off edges
  
  const int ix_min=static_cast<int>(std::max(0.0f        ,ceilf(x0-0.5f)));
  const int ix_max=static_cast<int>(std::min(width()-0.5f,floorf(x1-0.5f)));
  
  const ComputeType kv((v1-v0)/(x1-x0));
  
  T*const row_ptr=row(y);
  for (int ix=ix_min;ix<=ix_max;ix++)
    {
      const ComputeType v(v0+kv*(ix+0.5f-x0));
      row_ptr[ix]=static_cast<T>(v);
    }
}

template <> void Raster<uchar>::write_pgm(std::ostream& out) const
{
  out << "P5" << std::endl;
  out << width() << " " << height() << std::endl;
  out << "255" << std::endl;
  for (ConstRowIterator row=row_begin();row!=row_end();++row)
    out.write(reinterpret_cast<const char*>(&(*(row->begin()))),row->size());
}

template <> void Raster<ushort>::write_pgm(std::ostream& out) const
{
  out << "P5" << std::endl;
  out << width() << " " << height() << std::endl;
  out << maximum_scalar_pixel_value() << std::endl;
  for (ConstRowIterator row=row_begin();row!=row_end();++row)
    for (const ushort* it=row->begin();it!=row->end();++it)
      {
	// PGM spec is most significant byte first
	const uchar p[2]={((*it)>>8),(*it)};
	out.write(reinterpret_cast<const char*>(p),2);
      }
}

template <> void Raster<ByteRGBA>::write_ppm(std::ostream& out) const
{
  out << "P6" << std::endl;
  out << width() << " " << height() << std::endl;
  out << "255" << std::endl;
  for (ConstRowIterator row=row_begin();row!=row_end();++row)
    for (const ByteRGBA* it=row->begin();it!=row->end();++it)
      out.write(reinterpret_cast<const char*>(&((*it).r)),3);
}


template class Raster<uchar>;
template class Image<uchar>;

template class Raster<ushort>;
template class Image<ushort>;

template class Raster<ByteRGBA>;
template class Image<ByteRGBA>;
