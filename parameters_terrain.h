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
#ifndef _parameters_terrain_h_
#define _parameters_terrain_h_

#include "useful.h"
#include "xyz.h"

//! Aggregates controllable parameters for all things related to terrain generation.
/*! \todo Add these to ParametersTerrain (and ControlTerrain):  float treeline;  float beachline;
*/

class ParametersTerrain
{
public:
  
  //! What kind of object will be generated.
  typedef enum {ObjectTypePlanet,ObjectTypeTerrain} ObjectType;
  ObjectType object_type;
  
  //! Random seed for subdivision perturbations.
  uint subdivisions_seed;

  //! Number of subdivisions.
  uint subdivisions;

  //! Numer of subdivisions (at the top level) which will be unperturbed
  uint subdivisions_unperturbed;
  
  //! Maximum size of perturbations (z in vertical direction, x & y horizontally).
  XYZ variation;

  //! Initial height of unsubdivided, unperturbed terrain, expressed as a proportion of variation.z
  float base_height;

  //! Power law applied to terrain heights.
  /*! When >1, flattens low areas.  When <1, flattens highland areas and creates gorges.
   */
  float power_law;

  //! Normalised height of snowline at the equator.
  float snowline_equator;  

  //! Normalised height of snowline at the pole.
  float snowline_pole;  

  //! Power law for snowline.
  float snowline_power_law;  

  //! Supresses snow on slopes.
  float snowline_slope_effect;  

  //! When positive, rivers become glaciers.  When negative, rivers remain blue.
  float snowline_glacier_effect;

  //! Number of rivers to generate.
  uint rivers;

  //! Random seed for river generation.
  uint rivers_seed;
  
  //! Limit on lake size as a proportion of available surface.
  /*! When lakes (produced during the river-growing step) cover this fraction of the available terrain they become seas and no-longer need to find a drain.
   */
  float lake_becomes_sea;

  //! Constructor sets up some hopefully sensible defaults.
  /*! \todo: Might be better (more portable) to use QTime::currentTime () for random seeds.
   */
  ParametersTerrain()
    :object_type(ObjectTypePlanet)
    ,subdivisions_seed(time(0))
    ,subdivisions(5)
    ,subdivisions_unperturbed(1)
    ,variation(0.0,0.0,0.125)
    ,base_height(0)
    ,power_law(1.5)
    ,snowline_equator(0.8)
    ,snowline_pole(-0.1)
    ,snowline_power_law(1.0)
    ,snowline_slope_effect(1.0)
    ,snowline_glacier_effect(0.1)
    ,rivers(0)
    ,rivers_seed(time(0))
    ,lake_becomes_sea(5)
  {}
};




#endif
