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
/* Copyright (C) 1998,1999 T Day */

/*! \file
  \brief Interface for class ByteRGB and class FloatRGB.
*/

#ifndef _image_rgb_h_
#define _image_rgb_h_

#include "useful.h"

class FloatRGBA;

//! Class to represent red-green-blue colours stored with 8-bit resolution.
/*! Direct access to class members is permitted.
    This is intended as a minimal class for efficient storage of colours.
    (However, padding means there is a wasted byte if we just want RGB so we
    might as well have an alpha channel).
    For computations FloatRGBA should be used.
 */
class ByteRGBA
{
 public:
  //@{
  //! Colour component.
  uchar r;
  uchar g;
  uchar b;
  uchar a;
  //@}

  //! Null constructor.
  /* NB There are no default values.
   */
  ByteRGBA()
    {}

  //! Copy constructor.
  ByteRGBA(const ByteRGBA& c)
    :r(c.r)
    ,g(c.g)
    ,b(c.b)
    ,a(c.a)
    {}

  //! Initialise from separate components.
  ByteRGBA(uchar vr,uchar vg,uchar vb,uchar va)
    :r(vr)
    ,g(vg)
    ,b(vb)
    ,a(va)
    {}

  //! Convert from FloatRGB.
  explicit ByteRGBA(const FloatRGBA&);

  // Don't bother with destructor for inline things.
  //~ByteRGBA()
  //  {}

  //! Colour addition.
  void operator+=(const ByteRGBA& v)
    {
      r+=v.r;
      g+=v.g;
      b+=v.b;
      a+=v.a;
    }
  
  //! Colour subtraction.
  void operator-=(const ByteRGBA& v)
    {
      r-=v.r;
      g-=v.g;
      b-=v.b;
      a-=v.a;
    }
  std::ostream& write(std::ostream&) const;
  const std::string format_comma() const;
}; 

//! Class to represent red-green-blue colours stored to floating point accuracy.
/*! Direct access to class members is permitted.
 */
class FloatRGBA
{
public:
  //@{
  //! Colour component
  float r;
  float g;
  float b;
  float a;
  //@}

  //! Null constructor.
  /* NB There are no default values.
   */
  FloatRGBA()
    {}

  //! Copy constructor.
  FloatRGBA(const FloatRGBA& c)
    :r(c.r),g(c.g),b(c.b),a(c.a)
    {}

  //! Initialise from separate components.
  FloatRGBA(float vr,float vg,float vb,float va)
    :r(vr),g(vg),b(vb),a(va){}

  //! Initialise from ByteRGBA.
  /*! Byte values [0,255] are normalised to [0.0,1.0]
   */
  FloatRGBA(const ByteRGBA& c)
    :r(c.r/255.0)
    ,g(c.g/255.0)
    ,b(c.b/255.0)
    ,a(c.a/255.0)
    {}

  ~FloatRGBA()
    {}

  //! Colour addition.
  void operator+=(const FloatRGBA& v)
    {
      r+=v.r;
      g+=v.g;
      b+=v.b;
      a+=v.a;
    }

  //! Colour subtraction.
  void operator-=(const FloatRGBA& v)
    {
      r-=v.r;
      g-=v.g;
      b-=v.b;
      a-=v.a;
    }

  //! Colour multiplication by another colour (componentwise).
  void operator*=(const FloatRGBA& v)
    {
      r*=v.r;
      g*=v.g;
      b*=v.b;
      a*=v.a;
    }

  //! Colour multiplication by a scalar.
  void operator*=(float k)
    {
      r*=k;
      g*=k;
      b*=k;
      a*=k;
    }

  //! Output method.
  std::ostream& write(std::ostream&) const;

  const std::string format_pov_rgb() const;
  const std::string format_pov_rgbf() const;
};

//! Colour equality operator.
inline bool operator==(const FloatRGBA& a,const FloatRGBA& b)
{
  return (a.r==b.r && a.g==b.g && a.b==b.b && a.a==b.a);
}

//! Colour inequality operator.
inline bool operator!=(const FloatRGBA& a,const FloatRGBA& b)
{
  return (a.r!=b.r || a.g!=b.g || a.b!=b.b || a.a!=b.a);
}

//! Colour addition operator.
inline FloatRGBA operator+(const FloatRGBA& a,const FloatRGBA& b)
{
  return FloatRGBA
    (
     a.r+b.r,
     a.g+b.g,
     a.b+b.b,
     a.a+b.a
     );
}

//! Colour negation operator.
inline FloatRGBA operator-(const FloatRGBA& c)
{
  return FloatRGBA
    (
     -c.r,
     -c.g,
     -c.b,
     -c.a
     );
}

//! Colour subtraction operator.
inline FloatRGBA operator-(const FloatRGBA& a,const FloatRGBA& b)
{
  return FloatRGBA
    (
     a.r-b.r,
     a.g-b.g,
     a.b-b.b,
     a.a-b.a
     );
}

//! Colour multiplication-by-scalar operator.
inline FloatRGBA operator*(float k,const FloatRGBA& c)
{
  return FloatRGBA
    (
     k*c.r,
     k*c.g,
     k*c.b,
     k*c.a
     );
}

//! Colour multiplication-by-scalar operator.
inline FloatRGBA operator*(const FloatRGBA& c,float k)
{
  return FloatRGBA
    (
     k*c.r,
     k*c.g,
     k*c.b,
     k*c.a
     );
}

//! Colour multiplication operator.
/*! Componentwise multiplication.
 */
inline FloatRGBA operator*(const FloatRGBA& a,const FloatRGBA& b)
{
  return FloatRGBA
    (
     a.r*b.r,
     a.g*b.g,
     a.b*b.b,
     a.a*b.a
     );
}

inline std::ostream& operator<<(std::ostream& out,const FloatRGBA& c)
{
  return c.write(out);
}

//! Construct ByteRGBA from FloatRGBA
/*! Components in the range [0.0,1.0] are scaled to [0,255].
 */
inline ByteRGBA::ByteRGBA(const FloatRGBA& c)
  :r((uchar)(255.0*c.r))
  ,g((uchar)(255.0*c.g))
  ,b((uchar)(255.0*c.b))
  ,a((uchar)(255.0*c.a))
{}

#endif



