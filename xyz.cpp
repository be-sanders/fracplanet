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

#include "xyz.h"
#include "pov_mode.h"
#include <iostream>

/*! Usually just outputs whitespace-separated co-ordinates.
  If POV-Ray mode is enabled, then "<>" bracketed, comma-separated co-ordinates are output with the Y and Z co-ordinates exchanged
  to (assuming we have been working in a right-handed co-ordinate system with z up) obtain left-handed co-ordinates with y up. 
 */
std::ostream& XYZ::write(std::ostream& out) const
{
  if (POVMode::pov_mode())
    return out << "<" << x << "," << z << "," << y << ">";
  else
    return out << x << " " << y << " " << z;
}

RandomXYZInUnitCube::RandomXYZInUnitCube(Random01& rng)
:XYZ()
{
  x=rng();
  y=rng();
  z=rng();
}

RandomXYZInBox::RandomXYZInBox(Random01& rng,const XYZ& bounds)
:XYZ()
{
  x=-bounds.x+2.0*bounds.x*rng();
  y=-bounds.y+2.0*bounds.y*rng();
  z=-bounds.z+2.0*bounds.z*rng();
}

RandomXYZInSphere::RandomXYZInSphere(Random01& rng,float radius)
:XYZ()
{
  do
    {
      x=2.0*rng()-1.0;
      y=2.0*rng()-1.0;
      z=2.0*rng()-1.0;
    }
  while (magnitude2()>1.0);
  (*this)*=radius;
}

/*! Can handle case of individual axes being zero.
 */
RandomXYZInEllipsoid::RandomXYZInEllipsoid(Random01& rng,const XYZ& axes)
:XYZ()
{
  do
    {
      assign(RandomXYZInBox(rng,axes));
    }
  while (
	  (axes.x==0.0 ? 0.0 : sqr(x/axes.x))
	 +(axes.y==0.0 ? 0.0 : sqr(y/axes.y))
	 +(axes.z==0.0 ? 0.0 : sqr(z/axes.z))
	 >1.0
	 );
}

RandomXYZSphereNormal::RandomXYZSphereNormal(Random01& rng)
:XYZ(0.0,0.0,0.0)
{
  float m2;
  do
    {
      assign(RandomXYZInSphere(rng,1.0));
      m2=magnitude2();
    }
  while (m2==0.0);

  (*this)/=sqrtf(m2);
}
