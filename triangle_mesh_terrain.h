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
  \brief Interface for class TriangleMeshTerrain and derived classes.
*/

#ifndef _triangle_mesh_terrain_h_
#define _triangle_mesh_terrain_h_

#include "triangle_mesh.h"

//! This class holds all the terrain-related methods.  
/*! It's intended to be used as a "mix-in", adding terrain generating functionality to terrain objects subclassed from simpler geometries.
  \todo Multiple inheritance pretty yucky.  Switch to a "Factory" pattern.
 */
class TriangleMeshTerrain : virtual TriangleMesh
{
 protected:
  //! Indices of the set of triangle with all vertices at sea-level
  std::set<uint> sea_triangles;  

  //! Indices of the set of vertices comprising the river network
  std::set<uint> river_vertices; 

  //! Maximum height of terrain (used to scale to/from "normalised" height).
  float max_height;

  //! Impose a sea level (raise lower vertices, and note sea triangles).
  void do_sea_level(const ParametersTerrain& parameters);

  //! Apply power law
  void do_power_law(const ParametersTerrain& parameters);

  //! Generate river network.
  void do_rivers(const ParametersTerrain& parameters);

  //!Final colouration pass.
  void do_colours(const ParametersTerrain& parameters);

 public:
  //! Constructor.
  TriangleMeshTerrain(Progress* progress)
    :TriangleMesh(progress)
    ,max_height(0.0)
    {}

  //! Destructor.
  virtual ~TriangleMeshTerrain()
    {}

  //! Apply rules for sea-level through final colouring to a pre-subdivided triangle mesh.
  void do_terrain(const ParametersTerrain& parameters);

  //! Dump the model as a POV scene.
  /*! Virtual method because spherical and flat terrains need e.g different sea-level planes and atmosphere layers.
   */
  virtual void write_povray(const ParametersSave& param,const ParametersTerrain& parameters_terrain) const
    =0;
};

//! Class constructing specific case of a planetary terrain.
class TriangleMeshTerrainPlanet : public TriangleMeshSubdividedIcosahedron, virtual public TriangleMeshTerrain
{
 protected:
 public:
  //! Constructor.
  TriangleMeshTerrainPlanet(const ParametersTerrain& param,Progress* progress);

  //! Destructor.
  virtual ~TriangleMeshTerrainPlanet()
    {}

  //! Specifc dump-to-povray for planet terrain.
  virtual void write_povray(const ParametersSave& param,const ParametersTerrain& parameters_terrain) const;
};

//! Class constructing specific case of a flat-base terrain area.
class TriangleMeshTerrainFlat : public TriangleMeshFlatTriangle, virtual public TriangleMeshTerrain
{
 public:
  //! Constructor.
  TriangleMeshTerrainFlat(const ParametersTerrain& parameters,Progress* progress);

  //! Destructor.
  virtual ~TriangleMeshTerrainFlat()
    {}

  //! Specifc dump-to-povray for flat terrain area.
  virtual void write_povray(const ParametersSave& param,const ParametersTerrain& parameters_terrain) const;
};




#endif
