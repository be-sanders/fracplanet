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
#include "triangle_mesh_cloud.h"
#include "noise.h"

TriangleMeshCloud::TriangleMeshCloud(Progress* progress)
  :TriangleMesh(progress)
{}

TriangleMeshCloud::~TriangleMeshCloud()
{}


TriangleMeshCloudPlanet::TriangleMeshCloudPlanet(const ParametersCloud& parameters,Progress* progress)
  :TriangleMesh(progress)
  ,TriangleMeshCloud(progress)
  ,TriangleMeshSubdividedIcosahedron(1.0+parameters.cloudbase,parameters.subdivisions,parameters.subdivisions,parameters.seed,XYZ(0.0,0.0,0.0),progress)
{
  _triangle_switch_colour=triangles();
  compute_vertex_normals();
  for (uint v=0;v<vertices();v++)
    vertex(v).colour(0,ByteRGBA(255,255,255,192));
}

TriangleMeshCloudFlat::TriangleMeshCloudFlat(const ParametersCloud& parameters,Progress* progress)
  :TriangleMesh(progress)
  ,TriangleMeshCloud(progress)
  ,TriangleMeshFlat(parameters.object_type,parameters.cloudbase,parameters.seed,progress)
{
  subdivide(parameters.subdivisions,parameters.subdivisions,XYZ(0.0,0.0,0.0));
  compute_vertex_normals();
  _triangle_switch_colour=triangles();
  for (uint v=0;v<vertices();v++)
    vertex(v).colour(0,ByteRGBA(255,255,255,192));
}

