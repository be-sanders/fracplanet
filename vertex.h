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
#ifndef _vertex_h_
#define _vertex_h_

#include "useful.h"
#include "xyz.h"
#include "rgb.h"

//! Class to store vertex state information
/*! There is no direct access to members.
  Should probably be a protected member class of TriangleMesh.
 */
class Vertex
{
 protected:
  XYZ _position;
  XYZ _normal;
  ByteRGB _colour[2];
 public:
  Vertex()
    {}

  Vertex(const Vertex& v)
    :_position(v._position)
    ,_normal(v._normal)
    {
      _colour[0]=v._colour[0];
      _colour[1]=v._colour[1];
    }

  explicit Vertex(const XYZ& p)
    :_position(p)
    ,_normal(0.0,0.0,0.0)
    {
      _colour[0]=ByteRGB(0,0,0);
      _colour[1]=ByteRGB(0,0,0);
    }

  const XYZ& position() const
    {
      return _position;
    }
  const XYZ& normal() const
    {
      return _normal;
    }
  const ByteRGB& colour(uint c) const
    {
      assert(c<2);
      return _colour[c];
    }

  void position(const XYZ& p)
    {
      _position=p;
    }
  void normal(const XYZ& n)
    {
      _normal=n;
    }
  void colour(uint c,const ByteRGB& col)
    {
      assert(c<2);
      _colour[c]=col;
    }
};


#endif
