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

#include <boost/optional.hpp>

/*! Common scan-converter code
 */
void Geometry::scan_convert_common
(
 const boost::array<XYZ,3>& v,
 const ScanConvertBackend& backend
 )
{
  // Sort vertices by increasing image y co-ordinate 
  boost::array<uint,3> sort={0,1,2};
  if (v[sort[0]].y>v[sort[1]].y) exchange(sort[0],sort[1]);
  if (v[sort[1]].y>v[sort[2]].y) exchange(sort[1],sort[2]);
  if (v[sort[0]].y>v[sort[1]].y) exchange(sort[0],sort[1]);

  // deltas
  const float x02=v[sort[2]].x-v[sort[0]].x;
  const float y02=v[sort[2]].y-v[sort[0]].y;
  const float x01=v[sort[1]].x-v[sort[0]].x;
  const float y01=v[sort[1]].y-v[sort[0]].y;
  const float x12=v[sort[2]].x-v[sort[1]].x;
  const float y12=v[sort[2]].y-v[sort[1]].y;
  
  boost::optional<float> ky02;
  boost::optional<float> ky01;
  boost::optional<float> ky12;
  
  if (y02==0.0f) return;
  
  ky02=1.0f/y02;
  if (y01!=0.0f) ky01=1.0f/y01;
  if (y12!=0.0f) ky12=1.0f/y12;
  
  // y range in image co-ordinates
  const int map_height=backend.height();
  const int iy_min=static_cast<int>(std::max(0.0f,ceilf(v[sort[0]].y-0.5f)));
  const int iy_mid=static_cast<int>(floorf(v[sort[1]].y-0.5f));
  const int iy_max=static_cast<int>(std::min(map_height-0.5f,floorf(v[sort[2]].y-0.5f)));
  
  if (ky01)
    for (int iy=iy_min;iy<=iy_mid;iy++)
      {
	const float yp02=(iy+0.5f-v[sort[0]].y)*ky02.get();
	const ScanEdge edge0(v[sort[0]].x+yp02*x02,sort[0],sort[2],yp02);
	
	const float yp01=(iy+0.5f-v[sort[0]].y)*ky01.get();
	const ScanEdge edge1(v[sort[0]].x+yp01*x01,sort[0],sort[1],yp01);
	if (edge0.x<=edge1.x)
	  backend.scan_convert_backend(iy,edge0,edge1);
	else
	  backend.scan_convert_backend(iy,edge1,edge0);
      }
  
  if (ky12)
    for (int iy=iy_mid+1;iy<=iy_max;iy++)
      {
	const float yp02=(iy+0.5f-v[sort[0]].y)*ky02.get();
	const ScanEdge edge0(v[sort[0]].x+yp02*x02,sort[0],sort[2],yp02);
	
	const float yp12=(iy+0.5f-v[sort[1]].y)*ky12.get();
	const ScanEdge edge1(v[sort[1]].x+yp12*x12,sort[1],sort[2],yp12);
	if (edge0.x<=edge1.x)
	  backend.scan_convert_backend(iy,edge0,edge1);
	else
	  backend.scan_convert_backend(iy,edge1,edge0);
      }
}

/*! Scan lines are throught the centre of pixels at y=0.5.
  This function doesn't care about quantization in x; that's for the backend.
 */
void GeometryFlat::scan_convert
(
 const boost::array<XYZ,3>& v,
 const ScanConvertBackend& backend
 ) const
{
  const boost::array<XYZ,3> vp
    ={
      XYZ(backend.width()*0.5f*(1.0f+v[0].x),backend.height()*0.5f*(1.0f-v[0].y),0.0f),
      XYZ(backend.width()*0.5f*(1.0f+v[1].x),backend.height()*0.5f*(1.0f-v[1].y),0.0f),
      XYZ(backend.width()*0.5f*(1.0f+v[2].x),backend.height()*0.5f*(1.0f-v[2].y),0.0f)
    };

  scan_convert_common(vp,backend);
}

/*!
  The problem with spherical geometry is that spans can go off one side of the map and come back on the other.
 */
void GeometrySpherical::scan_convert
(
 const boost::array<XYZ,3>& v,
 const ScanConvertBackend& backend
 ) const
{
  const float vx_min=std::min(v[0].x,std::min(v[1].x,v[2].x));
  const float vx_max=std::max(v[0].x,std::max(v[1].x,v[2].x));
  const float vy_min=std::min(v[0].y,std::min(v[1].y,v[2].y));
  const float vy_max=std::max(v[0].y,std::max(v[1].y,v[2].y));
  if (vx_min<=0.0f && 0.0f<=vx_max && vy_min<=0.0f && 0.0f<=vy_max)
    {
      // Triangle _may_ include the pole (z-axis), so consider subdividing
      const float dx=std::max(-vx_min,vx_max);
      const float dy=std::max(-vy_min,vy_max);
      if ((dx+dy)*backend.height()>0.25f)
	{
	  backend.subdivide(v,this);
	}
      else return;
    }

  boost::array<XYZ,3> vn={v[0].normalised(),v[1].normalised(),v[2].normalised()};
  boost::array<XYZ,3> vp;
  for (uint i=0;i<3;i++)
    {
      vp[i].x=backend.width()*0.5f*(1.0f+atan2f(vn[i].y,vn[i].x)*M_1_PI);
      if (i!=0)
	{
	  // Need to keep all the vertices in the same domain
	  if (vp[i].x-vp[0].x>0.5*backend.width()) vp[i].x-=backend.width();
	  else if (vp[i].x-vp[0].x<-0.5*backend.width()) vp[i].x+=backend.width();
	}
      vp[i].y=backend.height()*0.5f*(1.0f-asinf(vn[i].z)*M_2_PI);
      vp[i].z=0.0f;
    }
  for (float d=-1.0f;d<=1.0f;d+=1.0f)
    {
      // Easiest way to deal with triangles on the "date line" is to
      // render them with all possible placements and let the back end cull them.
      /*! \todo Might be better if span replication was done in backed rather than
	duplicating all the y-compute.
      */
      boost::array<XYZ,3> vpt;
      for (uint i=0;i<3;i++)
	{
	  vpt[i].x=vp[i].x+d*backend.width();
	  vpt[i].y=vp[i].y;
	  vpt[i].z=vp[i].z;
	}
      scan_convert_common(vpt,backend);
    }
}
