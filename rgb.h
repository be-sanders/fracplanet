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
/* Copyright (C) 1998,1999 T Day */

#ifndef _image_rgb_h_
#define _image_rgb_h_

#include "useful.h"

class FloatRGB;

//! Class to represent red-green-blue colours stored with 8-bit resolution.
/*! Direct access to class members is permitted.
    This is intended as a minimal class for efficient storage of colours.
    For computations FloatRGB should be used.
 */
class ByteRGB
{
 public:
  uchar r;
  uchar g;
  uchar b;

  //! Null constructor.
  /* NB There are no default values.
   */
  ByteRGB()
    {}

  //! Copy constructor.
  ByteRGB(const ByteRGB& c)
    :r(c.r),g(c.g),b(c.b)
    {}

  //! Initialise from separate components.
  ByteRGB(uchar vr,uchar vg,uchar vb)
    :r(vr),g(vg),b(vb)
    {}

  //! Convert from FloatRGB.
  explicit ByteRGB(const FloatRGB&);

  ~ByteRGB()
    {}

  //! Colour addition.
  void operator+=(const ByteRGB& v)
    {
      r+=v.r;
      g+=v.g;
      b+=v.b;
    }
  
  //! Colour subtraction.
  void operator-=(const ByteRGB& v)
    {
      r-=v.r;
      g-=v.g;
      b-=v.b;
    }
};

//! Class to represent red-green-blue colours stored to floating point accuracy.
/*! Direct access to class members is permitted.
 */
class FloatRGB
{
public:
  float r;
  float g;
  float b;

  //! Null constructor.
  /* NB There are no default values.
   */
  FloatRGB()
    {}

  //! Copy constructor.
  FloatRGB(const FloatRGB& c)
    :r(c.r),g(c.g),b(c.b)
    {}

  //! Initialise from separate components.
  FloatRGB(float vr,float vg,float vb)
    :r(vr),g(vg),b(vb){}

  //! Initialise from ByteRGB.
  /*! Byte values [0,255] are normalised to [0.0,1.0]
   */
  FloatRGB(const ByteRGB& c)
    :r(c.r/255.0),g(c.g/255.0),b(c.b/255.0){}

  ~FloatRGB()
    {}

  //! Colour addition.
  void operator+=(const FloatRGB& v)
    {
      r+=v.r;
      g+=v.g;
      b+=v.b;
    }

  //! Colour subtraction.
  void operator-=(const FloatRGB& v)
    {
      r-=v.r;
      g-=v.g;
      b-=v.b;
    }

  //! Colour multiplication by another colour (componentwise).
  void operator*=(const FloatRGB& v)
    {
      r*=v.r;
      g*=v.g;
      b*=v.b;
    }

  //! Colour multiplication by a scalar.
  void operator*=(float k)
    {
      r*=k;
      g*=k;
      b*=k;
    }

  //! Output method.
  std::ostream& write(std::ostream&) const;
};

//! Colour equality operator.
inline bool operator==(const FloatRGB& a,const FloatRGB& b)
{
  return (a.r==b.r && a.g==b.g && a.b==b.b);
}

//! Colour inequality operator.
inline bool operator!=(const FloatRGB& a,const FloatRGB& b)
{
  return (a.r!=b.r || a.g!=b.g || a.b!=b.b);
}

//! Colour addition operator.
inline FloatRGB operator+(const FloatRGB& a,const FloatRGB& b)
{
  return FloatRGB
    (
     a.r+b.r,
     a.g+b.g,
     a.b+b.b
     );
}

//! Colour negation operator.
inline FloatRGB operator-(const FloatRGB& c)
{
  return FloatRGB
    (
     -c.r,
     -c.g,
     -c.b
     );
}

//! Colour subtraction operator.
inline FloatRGB operator-(const FloatRGB& a,const FloatRGB& b)
{
  return FloatRGB
    (
     a.r-b.r,
     a.g-b.g,
     a.b-b.b
     );
}

//! Colour multiplication-by-scalar operator.
inline FloatRGB operator*(float k,const FloatRGB& c)
{
  return FloatRGB
    (
     k*c.r,
     k*c.g,
     k*c.b
     );
}

//! Colour multiplication-by-scalar operator.
inline FloatRGB operator*(const FloatRGB& c,float k)
{
  return FloatRGB
    (
     k*c.r,
     k*c.g,
     k*c.b
     );
}

//! Colour multiplication operator.
/*! Componentwise multiplication.
 */
inline FloatRGB operator*(const FloatRGB& a,const FloatRGB& b)
{
  return FloatRGB
    (
     a.r*b.r,
     a.g*b.g,
     a.b*b.b
     );
}

inline std::ostream& operator<<(std::ostream& out,const FloatRGB& c)
{
  return c.write(out);
}

//! Construct ByteRGB from FloatRGB
/*! Components in the range [0.0,1.0] are scaled to [0,255].
 */
inline ByteRGB::ByteRGB(const FloatRGB& c)
:r((uchar)(255.0*c.r)),g((uchar)(255.0*c.g)),b((uchar)(255.0*c.b))
{}

#endif



