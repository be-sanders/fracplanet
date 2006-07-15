// Source file for fracplanet
// Copyright (C) 2006 Tim Day
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
  :Control(parent)
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

  joystick_mouse=new QCheckBox("Joystick mouse-Y (fly mode)",this);
  joystick_mouse->setChecked(parameters->joystick_mouse);
  QToolTip::add(joystick_mouse,"Mouse y-axis functions as joystick in fly mode:\nmouse moved down/pulled-back pitches up.");
  connect(
	  joystick_mouse,SIGNAL(stateChanged(int)),
	  this,SLOT(setJoystickMouse(int))
	  );

  display_list=new QCheckBox("Display list",this);
  display_list->setChecked(parameters->display_list);
  QToolTip::add(display_list,"Use OpenGL display lists; CAUTION: unstable on many platforms");
  connect(
	  display_list,SIGNAL(stateChanged(int)),
	  this,SLOT(setDisplayList(int))
	  );

  background_colour_low_button=new QPushButton(build_icon_of_colour(parameters->background_colour_low),"Background colour (low altitude)",this);
  QToolTip::add(background_colour_low_button,"Colour used in display area when the camera is low.");
  connect(background_colour_low_button,SIGNAL(clicked()),
	  this,SLOT(pickBackgroundColourLow())
	  );

  background_colour_high_button=new QPushButton(build_icon_of_colour(parameters->background_colour_high),"Background colour (high altitude)",this);
  QToolTip::add(background_colour_high_button,"Colour used in display area when the camera is high.");
  connect(background_colour_high_button,SIGNAL(clicked()),
	  this,SLOT(pickBackgroundColourHigh())
	  );
	  
  QGroupBox* ambient_box=new QGroupBox(3,Qt::Horizontal,"Ambient",this);
  new QLabel("0.0",ambient_box);
  ambient=new QSlider(0,100,10,10,Qt::Horizontal,ambient_box);
  ambient->setTickmarks(QSlider::Both);
  ambient->setTickInterval(10);
  ambient->setTracking(true);
  new QLabel("1.0",ambient_box);
  connect(
	  ambient,SIGNAL(valueChanged(int)),
	  this,SLOT(setAmbient(int))
	  );

  QGroupBox* illumination_box=new QGroupBox(3,Qt::Horizontal,"Illumination azimuth/elevation",this);
  new QLabel("-180",illumination_box);
  illumination_azimuth=new QSlider(-180,180,10,static_cast<int>(parameters->illumination_azimuth*180/M_PI),Qt::Horizontal,illumination_box);
  illumination_azimuth->setTickmarks(QSlider::Both);
  illumination_azimuth->setTickInterval(10);
  illumination_azimuth->setTracking(true);
  new QLabel("180",illumination_box);
  new QLabel("-90",illumination_box);
  illumination_elevation=new QSlider(-90,90,10,static_cast<int>(parameters->illumination_elevation*180/M_PI),Qt::Horizontal,illumination_box);
  illumination_elevation->setTickmarks(QSlider::Both);
  illumination_elevation->setTickInterval(10);
  illumination_elevation->setTracking(true);
  new QLabel("90",illumination_box);

  connect(
	  illumination_azimuth,SIGNAL(valueChanged(int)),
	  this,SLOT(setIlluminationAzimuth(int))
	  );
  connect(
	  illumination_elevation,SIGNAL(valueChanged(int)),
	  this,SLOT(setIlluminationElevation(int))
	  );

  new QLabel("Status:",this);
  status=new QLabel("",this);

  padding=new QVBox(this);
  setStretchFactor(padding,1);
}

ControlRender::~ControlRender()
{}

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

void ControlRender::setIlluminationAzimuth(int v)
{
  parameters->illumination_azimuth=v*M_PI/180;
}

void ControlRender::setIlluminationElevation(int v)
{
  parameters->illumination_elevation=v*M_PI/180;
}

void ControlRender::pickBackgroundColourLow()
{
  emit pickColour(background_colour_low_button,parameters->background_colour_low);
}

void ControlRender::pickBackgroundColourHigh()
{
  emit pickColour(background_colour_high_button,parameters->background_colour_high);
}
