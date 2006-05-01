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

#include <iosfwd>
#include <boost/scoped_array.hpp>
#include <boost/range.hpp>
#include <boost/noncopyable.hpp>

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
/*! Assumes explicit instantiation.
  NB The base type just refers to storage allocated elsewhere.
  Used to be gratuitously implemented using boost multiarray type,
  but that was too inefficient
 */
template <typename T> class Raster
{
 public:
  typedef typename PixelTraits<T>::ComputeType ComputeType;

  Raster(uint w,uint h,uint p,T* d)
    :_width(w)
    ,_height(h) 
    ,_pitch(p)
    ,_data(d)
    ,_row_end(row_range(h),p)
    ,_const_row_end(row_range(h),p)
    {}
  virtual ~Raster()
    {}

  const uint width() const
    {
      return _width;
    }
  const uint height() const
    {
      return _height;
    }
  const uint pitch() const
    {
      return _pitch;
    }
  const bool contiguous() const
    {
      return (_width==_pitch);
    }
  const uint contiguous_size() const
    {
      return _width*_height;
    }
  T*const contiguous_begin()
    {
      return _data;
    }
  const T*const contiguous_end() const
    {
      return _data+contiguous_size();
    }

  T* row_begin(uint r)
    {
      return _data+r*_pitch;
    }
  const T* row_begin(uint r) const
    {
      return _data+r*_pitch;
    }
  T* row_end(uint r)
    {
      return row_begin(r)+_width;
    }
  const T* row_end(uint r) const
    {
      return row_begin(r)+_width;
    }
  boost::iterator_range<T*> row_range(uint r)
    {
      T*const it=row_begin(r);
      return boost::iterator_range<T*>(it,it+_width);
    }

  boost::iterator_range<const T*> row_range(uint r) const
    {
      const T*const it=row_begin(r);
      return boost::iterator_range<const T*>(it,it+_width);
    }

  class RowIterator : public std::iterator<std::forward_iterator_tag, boost::iterator_range<T*> >
    {
    public:
      RowIterator(const boost::iterator_range<T*>& row, uint p)
	:_row(row)
	,_pitch(p)
	{}
      ~RowIterator()
	{}
      RowIterator& operator=(const RowIterator& it)
	{
	  _row=it._row;
	  assert(_pitch==it._pitch);
	  return (*this);
	}
      bool operator==(const RowIterator& it) const
	{
	  return _row.begin()==it._row.begin();
	}
      bool operator!=(const RowIterator& it) const
	{
	  return _row.begin()!=it._row.begin();
	}
      RowIterator& operator++()
	{
	  _row=boost::iterator_range<T*>
	    (
	     _row.begin()+_pitch,
	     _row.end()+_pitch
	     );
	  return (*this);
	}
      RowIterator operator++(int)
	{
	  RowIterator tmp(*this);
	  _row=boost::iterator_range<T*>
	    (
	     _row.begin()+_pitch,
	     _row.end()+_pitch
	     );
	  return tmp;
	}
      boost::iterator_range<T*>& operator*()
	{
	  return _row;
	}
      boost::iterator_range<T*>* operator->()
	{
	  return &_row;
	}
      
    private:
      boost::iterator_range<T*> _row;
      const uint _pitch;
    };

  RowIterator row_begin()
    {
      return RowIterator(row_range(0),_pitch);
    }
  RowIterator row_end()
    {
      return _row_end;
    }
  
  class ConstRowIterator : public std::iterator<std::forward_iterator_tag, boost::iterator_range<const T*> >
    {
    public:
      ConstRowIterator(const boost::iterator_range<const T*>& row, uint p)
	:_row(row)
	,_pitch(p)
	{}
      ~ConstRowIterator()
	{}
      ConstRowIterator& operator=(const ConstRowIterator& it)
	{
	  _row=it._row;
	  assert(_pitch==it._pitch);
	  return (*this);
	}
      bool operator==(const ConstRowIterator& it) const
	{
	  return _row.begin()==it._row.begin();
	}
      bool operator!=(const ConstRowIterator& it) const
	{
	  return _row.begin()!=it._row.begin();
	}
      ConstRowIterator& operator++()
	{
	  _row=boost::iterator_range<const T*>
	    (
	     _row.begin()+_pitch,
	     _row.end()+_pitch
	     );
	  return (*this);
	}
      ConstRowIterator operator++(int)
	{
	  ConstRowIterator tmp(*this);
	  _row=boost::iterator_range<const T*>
	    (
	     _row.begin()+_pitch,
	     _row.end()+_pitch
	     );
	  return tmp;
	}
      boost::iterator_range<const T*>& operator*()
	{
	  return _row;
	}
      boost::iterator_range<const T*>* operator->()
	{
	  return &_row;
	}
      
    private:
      boost::iterator_range<const T*> _row;
      const uint _pitch;
    };

  ConstRowIterator row_begin() const
    {
      return ConstRowIterator(row_range(0),_pitch);
    }
  ConstRowIterator row_end() const
    {
      return _const_row_end;
    }
  
  //! Clear the image to a constant value.
  void fill(const T& v);

  //! Fill a line segment on the given half-open range [x0,x1), interpolating between the two given values.
  void scan(uint y,float x0,const ComputeType& v0,float x1,const ComputeType& v1);

  void write_ppm(std::ostream&) const;
  void write_pgm(std::ostream&) const;

 private:
  const uint _width;
  const uint _height;
  const uint _pitch;
  T*const _data;
  const RowIterator _row_end;
  const ConstRowIterator _const_row_end;
};

template <typename T> class ImageStorage
{
 public:
  ImageStorage(int n)
    :_storage(new T[n])
    {}
  ~ImageStorage()
    {}
 protected:
  boost::scoped_array<T> _storage;
};

template <typename T> class Image : private ImageStorage<T>, public Raster<T>, public boost::noncopyable
{
 public:
  Image(uint w,uint h)
    :ImageStorage<T>(w*h)
    ,Raster<T>(w,h,w,ImageStorage<T>::_storage.get())
    {}
  ~Image()
    {}  
};

#endif


