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

#include "matrix34.h"

Matrix34RotateAboutAxisThrough::Matrix34RotateAboutAxisThrough(const XYZ& axis,float angle,const XYZ& pt)
{
  assign
    ( 
     Matrix34Translate(pt)
     *Matrix34(Matrix33RotateAboutAxis(axis,angle),XYZ(0.0f,0.0f,0.0f))
     *Matrix34Translate(-pt)
     );
}

Matrix34RotateAboutAxisThrough::Matrix34RotateAboutAxisThrough(const XYZ& axis,const XYZ& pt)
{
  const float axis_magnitude=axis.magnitude();

  if (axis_magnitude==0.0f)
    {
      assign(Matrix34Identity());
    }
  else
    {
      assign(Matrix34RotateAboutAxisThrough(axis/axis_magnitude,axis_magnitude,pt));
    }
}

