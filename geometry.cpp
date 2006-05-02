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
#include "geometry.h"

void GeometryFlat::scan_convert
(
 const Vertex* vertex0,
 const Vertex* vertex1,
 const Vertex* vertex2,
 uint map_width,
 uint map_height,
 std::vector<ScanLine>& scan_lines
 ) const
{
  // Sort vertices by y co-ordinate
  if (vertex0->position().y>vertex1->position().y) exchange(vertex0,vertex1);
  if (vertex1->position().y>vertex2->position().y) exchange(vertex1,vertex2);
  if (vertex0->position().y>vertex1->position().y) exchange(vertex0,vertex1);

  const int ymin=std::max(0,static_cast<int>(ceilf(map_height*(vertex0->position().y))));
  const float ymid=map_height*(vertex1->position().y);
  const float ymax=std::min(static_cast<float>(map_height),map_height*(vertex2->position().y));

  const float y02=vertex2->position().y-vertex0->position().y;
  const float x02=vertex2->position().x-vertex0->position().x;
  const float y01=vertex1->position().y-vertex0->position().y;
  const float x01=vertex1->position().x-vertex0->position().x;
  const float y12=vertex2->position().y-vertex1->position().y;
  const float x12=vertex2->position().x-vertex1->position().x;
  const float iy02=1.0f/y02;
  const float iy01=1.0f/y01;
  const float iy12=1.0f/y12;
  const float ky=1.0f/map_height;

  for (int y=ymin;y<ymax;++y)
    {
      const float yp02=(y*ky-vertex0->position().y)*iy02;
      scan_lines.push_back(ScanLine(y));

      ScanSpan span;
      span.edge[0]=ScanEdge(map_width*(vertex0->position().x+yp02*x02),vertex0,vertex2,yp02);

      if (y<ymid)
	{
	  const float yp01=(y-vertex0->position().y)*iy01;
	  span.edge[1]=ScanEdge(map_width*(vertex0->position().x+yp01*x01),vertex0,vertex1,yp01);
	}
      else
	{
	  const float yp12=(y-vertex1->position().y)*iy12;
	  span.edge[1]=ScanEdge(map_width*(vertex1->position().x+yp12*x12),vertex1,vertex2,yp12);
	}
      if (span.edge[0].x>span.edge[1].x)
	{
	  exchange(span.edge[0],span.edge[1]);
	}
      
      scan_lines.back().spans.push_back(span);
    }
}

void GeometrySpherical::scan_convert
(
 const Vertex* vertex0,
 const Vertex* vertex1,
 const Vertex* vertex2,
 uint map_width,
 uint map_height,
 std::vector<ScanLine>& scan_lines
 ) const
{
}
