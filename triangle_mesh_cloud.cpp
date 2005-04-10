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

void TriangleMeshCloud::do_cloud(const ParametersCloud& parameters)
{
  compute_vertex_normals();

  progress_start(100,"Cloud colouring");

  //! \todo Wire up terms, decay and base fequency and thresholds
  MultiscaleNoise noise(parameters.seed,6,0.5);
  for (uint i=0;i<vertices();i++)
    {
      progress_step((100*i)/vertices());

      const float v=0.5+0.5*noise(4.0f*vertex(i).position());
      const float v_min=0.5f;
      const float v_max=0.6f;
      const float v_k=1.0f/(v_max-v_min);
      const float vs=std::min(1.0f,std::max(0.0f,(v-v_min)*v_k));

      vertex(i).colour(0,ByteRGBA(255,255,255,static_cast<uint>(255.0*vs)));

      // Set other colour (unused) to red for debug
      vertex(i).colour(1,ByteRGBA(255,0,0,255));
    }
  progress_complete("Cloud colouring completed");

  _triangle_switch_colour=triangles();
}

TriangleMeshCloudPlanet::TriangleMeshCloudPlanet(const ParametersCloud& parameters,Progress* progress)
  :TriangleMesh(progress)
  ,TriangleMeshCloud(progress)
  ,TriangleMeshSubdividedIcosahedron(1.0+parameters.cloudbase,parameters.subdivisions,parameters.subdivisions,parameters.seed,XYZ(0.0,0.0,0.0),progress)
{
  do_cloud(parameters);
}

TriangleMeshCloudFlat::TriangleMeshCloudFlat(const ParametersCloud& parameters,Progress* progress)
  :TriangleMesh(progress)
  ,TriangleMeshCloud(progress)
  ,TriangleMeshFlat(parameters.object_type,parameters.cloudbase,parameters.seed,progress)
{
  subdivide(parameters.subdivisions,parameters.subdivisions,XYZ(0.0,0.0,0.0));
  do_cloud(parameters);
}

