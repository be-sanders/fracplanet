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

/*! \file
  \brief Interface for class TriangleMeshCloud and derived classes.
*/

#ifndef _triangle_mesh_cloud_h_
#define _triangle_mesh_cloud_h_

#include "image.h"
#include "parameters_cloud.h"
#include "triangle_mesh.h"

//! This class holds all the cloud-related methods.  
/*! It's intended to be used as a "mix-in", adding cloud generating 
  functionality to cloud objects subclassed from simpler geometries.
  \todo Ugh!!!  This is really yucky use of multiple inheritance.  Better for these terrain types to have-a TriangleMesh.
 */
class TriangleMeshCloud : virtual public TriangleMesh
{
 protected:
  void do_cloud(const ParametersCloud& parameters);

 public:
  
  //! Constructor.
  TriangleMeshCloud(Progress* progress);

  //! Destructor.
  virtual ~TriangleMeshCloud();

  //! Dump mesh to file for POV-Ray
  virtual void write_povray(std::ofstream& out,const ParametersSave&,const ParametersCloud&) const;

  //! Dump mesh to file for Blender
  virtual void write_blender(std::ofstream& out,const ParametersSave&,const ParametersCloud&,const std::string& mesh_name) const;

  //! Render the mesh onto a raster image.
  /*! The only interesting thing with clouds is their alpha, so render a greyscale.
  */
  virtual void render_texture(Raster<uchar>&) const;
};

//! Class constructing specific case of a planetary cloud.
class TriangleMeshCloudPlanet : public TriangleMeshSubdividedIcosahedron, virtual public TriangleMeshCloud
{
 protected:
 public:
  //! Constructor.
  TriangleMeshCloudPlanet(const ParametersCloud& param,Progress* progress);

  //! Destructor.
  virtual ~TriangleMeshCloudPlanet()
    {}
};

//! Class constructing specific case of a flat-base terrain area.
class TriangleMeshCloudFlat : public TriangleMeshFlat, virtual public TriangleMeshCloud
{
 public:
  //! Constructor.
  TriangleMeshCloudFlat(const ParametersCloud& parameters,Progress* progress);

  //! Destructor.
  virtual ~TriangleMeshCloudFlat()
    {}
};



#endif
