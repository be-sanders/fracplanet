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

template <typename T> void Raster<T>::scan(uint y,float x0,const ComputeType& v0,float x1,const ComputeType& v1)
{
  std::cerr << "[" << y << ",(" << x0 << "," << x1 << ")]";

  const float dx=x1-x0;
  const ComputeType dv(v1-v0);
  const ComputeType kv(dv*(1.0f/dx));
  const int xmin=std::max(0,static_cast<int>(ceil(x0)));
  if (x1>width()) x1=width();
  T*const row_ptr=row(y);
  for (uint xi=xmin;xi<x1;xi++)
    {
      const float x=xi-x0;
      const ComputeType v(v0+x*kv);
      row_ptr[xi]=static_cast<T>(v);
      xi++;
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

template <> void Raster<uchar>::write_pgm(std::ostream& out) const
{
  out << "P5" << std::endl;
  out << width() << " " << height() << std::endl;
  out << "255" << std::endl;
  for (ConstRowIterator row=row_begin();row!=row_end();++row)
    out.write(reinterpret_cast<const char*>(&(*(row->begin()))),row->size());
}

template class Raster<uchar>;
template class Image<uchar>;

template class Raster<ByteRGBA>;
template class Image<ByteRGBA>;
