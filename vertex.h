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

/*! \file
  \brief Interface for class Vertex.
*/

#ifndef _vertex_h_
#define _vertex_h_

#include "useful.h"
#include "xyz.h"
#include "rgb.h"

//! Class to store vertex state information
/*! There is no direct access to members.
  Should probably be a protected member class of TriangleMesh.
  sizeof(Vertex) should ideally be 3*4+3*4+2*3=30 but sizeof(ByteRGB) is 4 not 3 so reports 32.
 */
class Vertex
{
 protected:
  //! Position of vertex.
  XYZ _position;

  //! Normal at vertex (for smooth shading).
  XYZ _normal;

  //! Colours at vertex (could be a different colour in different triangles).
  ByteRGB _colour[2];

  //! Flag that this vertex should use emissive shading (could be different in different triangles).
  bool _emissive[2];

 public:

  //! Constructor.  NB Almost no default values set.
  Vertex()
    {
      _emissive[0]=false;
      _emissive[1]=false;
    }

  //! Copy constructor.
  Vertex(const Vertex& v)
    :_position(v._position)
    ,_normal(v._normal)
    {
      _colour[0]=v._colour[0];
      _colour[1]=v._colour[1];
      _emissive[0]=v._emissive[0];
      _emissive[1]=v._emissive[1];
    }

  //! Construct from position only.
  explicit Vertex(const XYZ& p)
    :_position(p)
    ,_normal(0.0,0.0,0.0)
    {
      _colour[0]=ByteRGB(0,0,0);
      _colour[1]=ByteRGB(0,0,0);
      _emissive[0]=false;
      _emissive[1]=false;
    }

  //! Accessor.
  const XYZ& position() const
    {
      return _position;
    }

  //! Accessor.
  const XYZ& normal() const
    {
      return _normal;
    }

  //! Accessor.
  const ByteRGB& colour(uint c) const
    {
      assert(c<2);
      return _colour[c];
    }

  //! Accessor.
  const bool emissive(uint c) const
    {
      assert(c<2);
      return _emissive[c];
    }

  //! Accessor.
  void position(const XYZ& p)
    {
      _position=p;
    }

  //! Accessor.
  void normal(const XYZ& n)
    {
      _normal=n;
    }

  //! Accessor.
  void colour(uint c,const ByteRGB& col)
    {
      assert(c<2);
      _colour[c]=col;
    }

  //! Accessor.
  void colour(uint c,const FloatRGB& col)
    {
      assert(c<2);
      _colour[c]=ByteRGB(col);
    }

  //! Accessor.
  void emissive(uint c,bool e)
    {
      assert(c<2);
      _emissive[c]=e;
    }
};


#endif
