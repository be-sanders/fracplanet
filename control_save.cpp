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
  atmosphere_checkbox=new QCheckBox("Add atmosphere",this);
  atmosphere_checkbox->setChecked(parameters->atmosphere);
  QToolTip::add(atmosphere_checkbox,"Select to add an atmosphere");
  connect(
	  atmosphere_checkbox,SIGNAL(stateChanged(int)),
	  this,SLOT(setAtmosphere(int))
	  );

  sea_object_checkbox=new QCheckBox("Sea object",this);
  sea_object_checkbox->setChecked(parameters->sea_object);  
  QToolTip::add(sea_object_checkbox,"Select to emit a single object (instead of multiple triangles) for the sea surface");
  connect(
	  sea_object_checkbox,SIGNAL(stateChanged(int)),
	  this,SLOT(setSeaSphere(int))
	  );

  save=new QPushButton("Save (POV-Ray)",this);
  QToolTip::add(save,"Press to save object for POV-Ray");
  connect(
	  save,SIGNAL(clicked()),
	  save_target,SLOT(save())
	  );

  padding=new QVBox(this);
  setStretchFactor(padding,1);
}
