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
#include "control_render.h"

#include <iostream>

ControlRender::ControlRender(QWidget* parent,ParametersRender* param)
  :QVBox(parent)
   ,parameters(param)
{
  parameters->notify=this;

  wireframe=new QCheckBox("Wireframe",this);
  wireframe->setChecked(parameters->wireframe);
  QToolTip::add(wireframe,"Selects wireframe OpenGL rendering");
  connect(
	  wireframe,SIGNAL(stateChanged(int)),
	  this,SLOT(setWireframe(int))
	  );

  display_list=new QCheckBox("Display list",this);
  display_list->setChecked(parameters->display_list);
  QToolTip::add(display_list,"Selects OpenGL rendering via display_list");
  connect(
	  display_list,SIGNAL(stateChanged(int)),
	  this,SLOT(setDisplayList(int))
	  );

  joystick_mouse=new QCheckBox("Joystick mouse-Y (fly mode)",this);
  joystick_mouse->setChecked(parameters->joystick_mouse);
  QToolTip::add(joystick_mouse,"Mouse y-axis functions as joystick in fly mode:\nmouse moved down/pulled-back pitches up.");
  connect(
	  joystick_mouse,SIGNAL(stateChanged(int)),
	  this,SLOT(setJoystickMouse(int))
	  );

  QGroupBox* ambient_box=new QGroupBox(3,Qt::Horizontal,"Ambient",this);
  new QLabel("0.0",ambient_box);
  ambient=new QSlider(0,100,10,10,Qt::Horizontal,ambient_box);
  ambient->setTracking(true);
  new QLabel("1.0",ambient_box);
  connect(
	  ambient,SIGNAL(valueChanged(int)),
	  this,SLOT(setAmbient(int))
	  );

  new QLabel("Status:",this);
  status=new QLabel("",this);

  padding=new QVBox(this);
  setStretchFactor(padding,1);
}

void ControlRender::notify(const std::string& message)
{
  status->setText(message.c_str());
}

void ControlRender::setWireframe(int v)
{
  parameters->wireframe=(v==2);
}

void ControlRender::setDisplayList(int v)
{
  parameters->display_list=(v==2);
}

void ControlRender::setJoystickMouse(int v)
{
  parameters->joystick_mouse=(v==2);
}

void ControlRender::setAmbient(int v)
{
  parameters->ambient=v/100.0f;
}
