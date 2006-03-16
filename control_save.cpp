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
#include "control_save.h"

#include "fracplanet_main.h"

ControlSave::ControlSave(QWidget* parent,FracplanetMain* save_target,ParametersSave* param)
  :QVBox(parent)
  ,parameters(param)
{
  QTabWidget*const tabs=new QTabWidget(this);
  QVBox*const tab_pov=new QVBox(this);
  tabs->addTab(tab_pov,"POV-Ray");

  QCheckBox*const atmosphere_checkbox=new QCheckBox("Add atmosphere",tab_pov);
  atmosphere_checkbox->setChecked(parameters->pov_atmosphere);
  QToolTip::add(atmosphere_checkbox,"Select to add an atmosphere");
  connect(
	  atmosphere_checkbox,SIGNAL(stateChanged(int)),
	  this,SLOT(setAtmosphere(int))
	  );

  QCheckBox*const sea_object_checkbox=new QCheckBox("Sea as single object",tab_pov);
  sea_object_checkbox->setChecked(parameters->pov_sea_object);  
  QToolTip::add(sea_object_checkbox,"Select to emit a single object (instead of multiple triangles) for the sea surface");
  connect(
	  sea_object_checkbox,SIGNAL(stateChanged(int)),
	  this,SLOT(setSeaSphere(int))
	  );

  setStretchFactor(new QVBox(tab_pov),1);

  QPushButton*const save_pov=new QPushButton("Save (POV-Ray)",tab_pov);
  QToolTip::add(save_pov,"Press to save object for POV-Ray");
  connect(
	  save_pov,SIGNAL(clicked()),
	  save_target,SLOT(save_pov())
	  );

  setStretchFactor(new QVBox(tab_pov),1);

  QVBox*const tab_blender=new QVBox(this);
  tabs->addTab(tab_blender,"Blender");

  setStretchFactor(new QVBox(tab_blender),1);

  QCheckBox*const per_vertex_alpha=new QCheckBox("Use per-vertex alpha (for clouds)",tab_blender);
  per_vertex_alpha->setChecked(parameters->blender_per_vertex_alpha);
  QToolTip::add(per_vertex_alpha,"Unfortunately Blender seems to ignore alpha components supplied with per-vertex\ncolours so a workround is normally used.\nCheck this box to save as if per-vertex alpha worked.");
  connect(
	  per_vertex_alpha,SIGNAL(stateChanged(int)),
	  this,SLOT(setPerVertexAlpha(int))
	  );

  setStretchFactor(new QVBox(tab_blender),1);

  QPushButton*const save_blender=new QPushButton("Save (Blender)",tab_blender);
  QToolTip::add(save_blender,"Press to save object for Blender");
  connect(
	  save_blender,SIGNAL(clicked()),
	  save_target,SLOT(save_blender())
	  );

  setStretchFactor(new QVBox(tab_blender),1);
}

ControlSave::~ControlSave()
{}

void ControlSave::setAtmosphere(int v)
{
  parameters->pov_atmosphere=(v==2);
}

void ControlSave::setSeaSphere(int v)
{
  parameters->pov_sea_object=(v==2);
}

void ControlSave::setPerVertexAlpha(int v)
{
  parameters->blender_per_vertex_alpha=(v==2);
}
