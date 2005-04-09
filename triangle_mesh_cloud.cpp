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

TriangleMeshCloudPlanet::TriangleMeshCloudPlanet(const ParametersTerrain& parameters,Progress* progress)
  :TriangleMesh(progress)
  ,TriangleMeshCloud(progress)
  ,TriangleMeshSubdividedIcosahedron(1.0+parameters.height,parameters.subdivisions,parameters.subdivisions,parameters.seed,progress)
{
}

TriangleMeshCloudFlat::TriangleMeshCloudFlat(const ParametersCloud& parameters,Progress* progress)
  :TriangleMesh(progress)
  ,TriangleMeshCloud(progress)
  ,TriangleMeshFlat(parameters.object_type,parameters.height,parameters.seed,progress)
{
  subdivide(parameters.subdivisions,parameters.subdivisions,parameters.variation);
}

