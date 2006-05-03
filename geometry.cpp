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
 const Vertex*const vertex[3],
 uint map_width,
 uint map_height,
 std::vector<ScanLine>& scan_lines
 ) const
{
  const boost::array<XYZ,3> v
    ={
      XYZ(map_width*0.5f*(1.0f+vertex[0]->position().x),map_height*0.5f*(1.0f-vertex[0]->position().y),0.0f),
      XYZ(map_width*0.5f*(1.0f+vertex[1]->position().x),map_height*0.5f*(1.0f-vertex[1]->position().y),0.0f),
      XYZ(map_width*0.5f*(1.0f+vertex[2]->position().x),map_height*0.5f*(1.0f-vertex[2]->position().y),0.0f)
    };

  boost::array<uint,3> sort={0,1,2};

  // Sort vertices by increasing image y co-ordinate 
  if (v[sort[0]].y>v[sort[1]].y) exchange(sort[0],sort[1]);
  if (v[sort[1]].y>v[sort[2]].y) exchange(sort[1],sort[2]);
  if (v[sort[0]].y>v[sort[1]].y) exchange(sort[0],sort[1]);

  // y range on image 
  const float y_min=std::max(0.0f,ceilf(v[sort[0]].y));
  const float y_mid=v[sort[1]].y;
  const float y_max=std::min(static_cast<float>(map_height-1),floorf(v[sort[2]].y));

  // deltas
  const float x02=v[sort[2]].x-v[sort[0]].x;
  const float y02=v[sort[2]].y-v[sort[0]].y;
  const float x01=v[sort[1]].x-v[sort[0]].x;
  const float y01=v[sort[1]].y-v[sort[0]].y;
  const float x12=v[sort[2]].x-v[sort[1]].x;
  const float y12=v[sort[2]].y-v[sort[1]].y;

  const float ky02=1.0f/y02;
  const float ky01=1.0f/y01;
  const float ky12=1.0f/y12;

  for (int y=static_cast<int>(y_min);y<=static_cast<int>(y_max);y++)
    {
      scan_lines.push_back(ScanLine(y));
      ScanSpan span;
      
      // yp02 is proportion along edge 02
      const float yp02=(y-v[sort[0]].y)*ky02;
      span.edge[0]=ScanEdge(v[sort[0]].x+yp02*x02,vertex[sort[0]],vertex[sort[2]],yp02);

      if (y<y_mid)
	{
	  const float yp01=(y-v[sort[0]].y)*ky01;
	  span.edge[1]=ScanEdge(v[sort[0]].x+yp01*x01,vertex[sort[0]],vertex[sort[1]],yp01);
	}
      else
	{
	  const float yp12=(y-v[sort[1]].y)*ky12;
	  span.edge[1]=ScanEdge(v[sort[1]].x+yp12*x12,vertex[sort[1]],vertex[sort[2]],yp12);
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
 const Vertex*const vertex[3],
 uint map_width,
 uint map_height,
 std::vector<ScanLine>& scan_lines
 ) const
{
}
