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
#ifndef _triangle_mesh_terrain_h_
#define _triangle_mesh_terrain_h_

#include "triangle_mesh.h"

//! This class holds all the terrain-related methods.  
/*! It's intended to be used as a "mix-in", adding terrain generating functionality to terrain objects subclassed from simpler geometries.
 */
class TriangleMeshTerrain : virtual TriangleMesh
{
 protected:
  std::set<uint> sea_triangles;  // The set of triangles with all vertices at sea-level
  std::set<uint> river_vertices; // The set of vertices comprising the river network

  float max_height;

  void do_sea_level(const ParametersTerrain& parameters);
  void do_power_law(const ParametersTerrain& parameters);
  void do_rivers(const ParametersTerrain& parameters);
  void do_colours(const ParametersTerrain& parameters);

 public:
  TriangleMeshTerrain(Progress* progress)
    :TriangleMesh(progress)
    ,max_height(0.0)
    {}
  virtual ~TriangleMeshTerrain()
    {}

  void do_terrain(const ParametersTerrain& parameters);

  virtual void write_povray(const ParametersSave& param) const
    =0;
};

class TriangleMeshTerrainPlanet : public TriangleMeshSubdividedIcosahedron, virtual public TriangleMeshTerrain
{
 protected:
 public:
  TriangleMeshTerrainPlanet(const ParametersTerrain& param,Progress* progress);
  virtual ~TriangleMeshTerrainPlanet()
    {}
  virtual void write_povray(const ParametersSave& param) const;
};

class TriangleMeshTerrainFlat : public TriangleMeshFlatTriangle, virtual public TriangleMeshTerrain
{
 public:
  TriangleMeshTerrainFlat(const ParametersTerrain& parameters,Progress* progress);
  virtual ~TriangleMeshTerrainFlat()
    {}

  virtual void write_povray(const ParametersSave& param) const;
};




#endif
