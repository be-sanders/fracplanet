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
#include "triangle_mesh_terrain.h"

#include <sstream>

void TriangleMeshTerrain::do_sea_level(const ParametersTerrain& /*parameters*/)
{
  // Introduce sea level  
  const uint steps=vertices()+triangles();
  uint step=0;
  
  progress_start(100,"Sea level");
  {    
    std::vector<bool> sea_vertices(vertices());    // Flag vertices forced to sea level
    
    for (uint i=0;i<vertices();i++)
      {
	step++;
	progress_step((100*step)/steps);
	
	const float m=vertex_height(i);
	
	if (m<=0.0)
	  {
	    set_vertex_height(i,0.0);
	    sea_vertices[i]=true;
	  }
	else if (m>max_height)
	  max_height=m;
      }

    std::vector<Triangle> land_sea[2];
   
    for (uint i=0;i<triangles();i++)
      {
	step++;
	progress_step((100*step)/steps);

	const Triangle& t(triangle(i));
	land_sea[sea_vertices[t.vertex(0)] && sea_vertices[t.vertex(1)] && sea_vertices[t.vertex(2)]].push_back(t);
      }

    _triangle_switch_colour=land_sea[0].size();
    _triangle=land_sea[0];
    _triangle.insert(_triangle.end(),land_sea[1].begin(),land_sea[1].end());
  }
  progress_complete("Sea level completed");
}

void TriangleMeshTerrain::do_power_law(const ParametersTerrain& parameters)
{
  const uint steps=vertices();
  uint step=0;
  
  progress_start(100,"Power law");
  
  for (uint i=0;i<vertices();i++)
    {
      step++;
      progress_step((100*step)/steps);

      const float h=vertex_height(i);
      if (h>geometry().epsilon())
	set_vertex_height(i,max_height*pow(h/max_height,parameters.power_law));
    }

  progress_complete("Power law completed");
}

void TriangleMeshTerrain::do_rivers(const ParametersTerrain& parameters)
{
  if (parameters.rivers==0) return;

  Random01 r01(parameters.rivers_seed);
  
  std::vector<bool> is_sea_vertex(vertices());
  std::vector<std::set<uint> > vertex_neighbours(vertices());

  {
    progress_start(100,"River preparation");

    const uint steps=triangles_of_colour1()+triangles();
    uint step=0;

    for (uint i=triangles_of_colour0();i!=triangles();i++)
      {
	step++;
	progress_step((100*step)/steps);

	const Triangle& t=triangle(i);
	is_sea_vertex[t.vertex(0)]=true;
	is_sea_vertex[t.vertex(1)]=true;
	is_sea_vertex[t.vertex(2)]=true;
      }

    // Need to know each vertex's neighbours
    for (uint i=0;i<triangles();i++)
      {
	step++;
	progress_step((100*step)/steps);
	
	const Triangle& t=triangle(i);
	vertex_neighbours[t.vertex(0)].insert(t.vertex(1));
	vertex_neighbours[t.vertex(0)].insert(t.vertex(2));
	vertex_neighbours[t.vertex(1)].insert(t.vertex(0));
	vertex_neighbours[t.vertex(1)].insert(t.vertex(2));
	vertex_neighbours[t.vertex(2)].insert(t.vertex(0));
	vertex_neighbours[t.vertex(2)].insert(t.vertex(1));
      }
    progress_complete("River preparation completed");
  }

  {
    progress_start(100,"Rivers");

    const uint maximum_lake_size=(uint)(vertices()*parameters.lake_becomes_sea);

    const uint steps=parameters.rivers;
    uint step=0;

    for (uint r=0;r<parameters.rivers;r++)
      {
	step++;
	progress_step((100*step)/steps);
      
	std::set<uint> vertices_to_add;

	std::set<uint> current_vertices;
	float current_vertices_height;
      
	// Pick a random non sea triangle to start river
	// Would like to use __gnu_cxx::random_sample_n but can't get it to work
	uint source_vertex=(uint)(r01()*vertices());

	// Rivers starting in the sea are skipped to keep river density independent of land area, and to avoid lock-up on all-sea planets
	if (is_sea_vertex[source_vertex])
	  continue;

	current_vertices.insert(source_vertex);
	current_vertices_height=vertex_height(source_vertex);

	while(1)
	  {
	    std::multimap<float,uint> flow_candidates;
	    bool reached_sea=false;
	    for (std::set<uint>::const_iterator it=current_vertices.begin();it!=current_vertices.end();it++)
	      {
		vertices_to_add.insert(*it);
		reached_sea|=(is_sea_vertex[*it]);

		const std::set<uint>& neighbours=vertex_neighbours[*it];
	      
		for (std::set<uint>::const_iterator it_n=neighbours.begin();it_n!=neighbours.end();it_n++)
		  {
		    // Only vertices NOT in the current river section are of interest
		    if (current_vertices.find(*it_n)==current_vertices.end())
		      {
			flow_candidates.insert(std::multimap<float,uint>::value_type(vertex_height(*it_n),*it_n));
		      }
		  }
	      }

	    if (reached_sea)
	      {
		break;
	      }
	    else if (current_vertices.size()>=maximum_lake_size)
	      {
		break;
	      }
	    else
	      {
		// If there is any intersection between the current river hexes and completed rivers then we're done
		bool meets_existing=false;
		for (std::set<uint>::const_iterator it=current_vertices.begin();it!=current_vertices.end();it++)
		  {
		    if (river_vertices.find(*it)!=river_vertices.end())
		      {
			meets_existing=true;
			break;
		      }
		  }
		if (meets_existing)
		  {
		    break;
		  }
	      }

	    if ((*(flow_candidates.begin())).first<current_vertices_height-geometry().epsilon())  // Can flow downhill...
	      {
		// We just flow into ONE vertex, otherwise rivers will expand to inundate all lower terrain
		current_vertices.clear();
		current_vertices.insert((*(flow_candidates.begin())).second);
		current_vertices_height=(*(flow_candidates.begin())).first;
	      }
	    else if ((*(flow_candidates.begin())).first<current_vertices_height+geometry().epsilon())  // Or expand across flat...
	      {
		// But if there are multiple candidates on the same level, flow into all of them
		std::multimap<float,uint>::const_iterator flat_end=flow_candidates.upper_bound(current_vertices_height+geometry().epsilon());
		  
		for (std::multimap<float,uint>::const_iterator it_flat=flow_candidates.begin();it_flat!=flat_end;it_flat++)
		  {
		    current_vertices.insert((*(it_flat)).second);
		  }
	      }
	    else // Otherwise the water level must rise
	      {
		current_vertices.insert((*(flow_candidates.begin())).second);
		current_vertices_height=(*(flow_candidates.begin())).first;
	      
		for (std::set<uint>::const_iterator it=current_vertices.begin();it!=current_vertices.end();it++)
		  {
		    set_vertex_height(*it,current_vertices_height);
		  }
	      }
	  }
	river_vertices.insert(vertices_to_add.begin(),vertices_to_add.end());
      }

    // These can take a while to clean up, so make sure its done within a progress scope.
    vertex_neighbours.clear();
    
    progress_complete("Rivers completed");
  }
}

void TriangleMeshTerrain::do_colours(const ParametersTerrain& parameters)
{
  const uint steps=triangles_of_colour1()+vertices();
  uint step=0;

  progress_start(100,"Colouring");
  
  {
    // Colour any triangles which need colouring (ie just sea)
    for (uint i=triangles_of_colour0();i!=triangles();i++)
      {
	step++;
	progress_step((100*step)/steps);      

	vertex(triangle(i).vertex(0)).colour(1,parameters.colour_ocean);
	vertex(triangle(i).vertex(1)).colour(1,parameters.colour_ocean);
	vertex(triangle(i).vertex(2)).colour(1,parameters.colour_ocean);

	if (parameters.oceans_and_rivers_emissive>0.0f)
	  {
	    vertex(triangle(i).vertex(0)).emissive(1,true);
	    vertex(triangle(i).vertex(1)).emissive(1,true);
	    vertex(triangle(i).vertex(2)).emissive(1,true);
	  }

	// For debugging, set the colour0 of those triangles to red
	vertex(triangle(i).vertex(0)).colour(0,ByteRGB(255,0,0));
	vertex(triangle(i).vertex(1)).colour(0,ByteRGB(255,0,0));
	vertex(triangle(i).vertex(2)).colour(0,ByteRGB(255,0,0));
      }
    
    const float treeline=0.25;
    const float beachline=0.01;

    // Colour all vertices
    for (uint i=0;i<vertices();i++)
      {
	step++;
	progress_step((100*step)/steps);      

	bool is_river=(river_vertices.find(i)!=river_vertices.end());
      
	float average_slope=1.0-(geometry().up(vertex(i).position())%vertex(i).normal());

	const float normalised_height=vertex_height(i)/max_height;

	float snowline_here=
	      parameters.snowline_equator
	      +fabs(geometry().normalised_latitude(vertex(i).position()))*(parameters.snowline_pole-parameters.snowline_equator)
	      +parameters.snowline_slope_effect*average_slope
	  -(is_river ? parameters.snowline_glacier_effect : 0.0);
	
	if (snowline_here>0.0)
	  snowline_here=pow(snowline_here,parameters.snowline_power_law);

	if (normalised_height>snowline_here)
	  {
	    vertex(i).colour(0,parameters.colour_snow);
	  }
	else if (is_river)
	  {
	    vertex(i).colour(0,parameters.colour_river);
	    vertex(i).emissive(0,true);
	  }
	else if (normalised_height<beachline)
	  {
	    vertex(i).colour(0,parameters.colour_shoreline);
	  }
	else if (normalised_height<treeline)
	  {
	    const float blend=normalised_height/treeline;
	    vertex(i).colour(0,blend*parameters.colour_high+(1.0-blend)*parameters.colour_low);
	  }
	else
	  {
	    vertex(i).colour(0,parameters.colour_high);
	  }
      }
  }

  progress_complete("Colouring completed");
}

void TriangleMeshTerrain::do_terrain(const ParametersTerrain& parameters)
{
  do_sea_level(parameters);
  do_power_law(parameters);
  do_rivers(parameters);
  compute_vertex_normals();
  do_colours(parameters);
  set_emissive(parameters.oceans_and_rivers_emissive);
}

TriangleMeshTerrainPlanet::TriangleMeshTerrainPlanet(const ParametersTerrain& parameters,Progress* progress)
  :TriangleMesh(progress)
  ,TriangleMeshTerrain(progress)
  ,TriangleMeshSubdividedIcosahedron(1.0+parameters.variation.z*parameters.base_height,parameters.subdivisions,parameters.subdivisions_unperturbed,parameters.subdivisions_seed,parameters.variation,progress)
{
  do_terrain(parameters);
}

bool TriangleMeshTerrainPlanet::write_povray(const std::string& base_filename,const ParametersSave& param,const ParametersTerrain& parameters_terrain) const
{
  const bool save_pov_mode=POVMode::pov_mode();
  POVMode::pov_mode(true);

  std::stringstream header;
  
  if (param.sea_object)
    {
      header 
	<< "sphere {<0.0,0.0,0.0>,1.0 pigment{rgb "
	<< parameters_terrain.colour_ocean
	<< "} finish {ambient " 
	<< emissive() 
	<< " diffuse " 
	<< 1.0f-emissive() 
	<< "}}\n";
    }
  
  if (param.atmosphere)
    {
      header
	<< "sphere {<0.0,0.0,0.0>,1.025 hollow texture {pigment {color rgbf 1}} interior{media{scattering{1,color rgb <1.0,1.0,1.0> extinction 1}}}}\n";
      header
	<< "sphere {<0.0,0.0,0.0>,1.05  hollow texture {pigment {color rgbf 1}} interior{media{scattering{1,color rgb <0.0,0.0,1.0> extinction 1}}}}\n";
    }
  
  bool ret=TriangleMesh::write_povray(base_filename,header.str(),param.sea_object);

  POVMode::pov_mode(save_pov_mode);

  return ret;
}

TriangleMeshTerrainFlat::TriangleMeshTerrainFlat(const ParametersTerrain& parameters,Progress* progress)
  :TriangleMesh(progress)
  ,TriangleMeshTerrain(progress)
  ,TriangleMeshFlatTriangle(parameters.variation.z*parameters.base_height,parameters.subdivisions_seed,progress)
{
  subdivide(parameters.subdivisions,parameters.subdivisions_unperturbed,parameters.variation);

  do_terrain(parameters);
}

bool TriangleMeshTerrainFlat::write_povray(const std::string& base_filename,const ParametersSave& param,const ParametersTerrain& parameters_terrain) const
{
  const bool save_pov_mode=POVMode::pov_mode();
  POVMode::pov_mode(true);

  std::ostringstream header;
  
  if (param.sea_object)
    {
      header
	<< "plane {<0.0,1.0,0.0>,0.0 pigment{rgb "
	<< parameters_terrain.colour_ocean
	<< "} finish {ambient " 
	<< emissive() 
	<< " diffuse " 
	<< 1.0f-emissive() 
	<< "}}\n";
    }
  
  if (param.atmosphere)
    {
      header
	<< "plane {<0.0,1.0,0.0>,0.05 hollow texture {pigment {color rgbf 1}} interior{media{scattering{1,color rgb <1.0,1.0,1.0> extinction 1}}}}\n";
      header
	<< "plane {<0.0,1.0,0.0>,0.1  hollow texture {pigment {color rgbf 1}} interior{media{scattering{1,color rgb <0.0,0.0,1.0> extinction 1}}}}\n";
    }
  
  const bool ret=TriangleMesh::write_povray(base_filename,header.str(),param.sea_object);

  POVMode::pov_mode(save_pov_mode);

  return ret;
}

