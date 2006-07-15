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

/*! \file
  \brief Interface for class ControlRender.
*/

#ifndef _control_render_h_
#define _control_render_h_

#include <qhbox.h>
#include <qlabel.h>
#include <qgroupbox.h>
#include <qcheckbox.h>
#include <qpushbutton.h>
#include <qprogressbar.h>
#include <qlineedit.h>
#include <qspinbox.h>
#include <qgrid.h>
#include <qtooltip.h>
#include <qradiobutton.h>
#include <qhbuttongroup.h>
#include <qslider.h>

#include "useful.h"
#include "control.h"
#include "notifiable.h"
#include "parameters_render.h"

//! Encapsulates GUI elements for controlling OpenGL rendering.
class ControlRender : public Control, public Notifiable
{
 private:
  Q_OBJECT;
 protected:
  //! The parameter set being controlled.
  ParametersRender*const parameters;

  QCheckBox* wireframe;
  QCheckBox* display_list;
  QCheckBox* joystick_mouse;
  QSlider* ambient;
  QSlider* illumination_azimuth;
  QSlider* illumination_elevation;

  QLabel* status;

  QVBox* padding;

 public:

  ControlRender(QWidget* parent,ParametersRender* param);
  ~ControlRender();

  virtual void notify(const std::string& message);
  
  public slots:
    
  void setWireframe(int v);
  void setDisplayList(int v);
  void setJoystickMouse(int v);
  void setAmbient(int v);
  void setIlluminationAzimuth(int v);
  void setIlluminationElevation(int v);

  void pickBackgroundColourLow();
  void pickBackgroundColourHigh();

 private:

  QPushButton* background_colour_low_button;
  QPushButton* background_colour_high_button;
};

#endif
