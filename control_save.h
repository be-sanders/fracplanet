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
  \brief Interface for class ControlSave.
*/
#ifndef _control_save_h_
#define _control_save_h_

#include <qhbox.h>
#include <qvbox.h>
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
#include "parameters_save.h"

class FracplanetMain;

//! Encapsulates GUI elements for controlling save.
class ControlSave : public QVBox
{
 private:
  Q_OBJECT
 
 protected:
  //! Ther parameters set we control
  ParametersSave*const parameters;
  
  QCheckBox* atmosphere_checkbox;
  QCheckBox* sea_object_checkbox;

  QPushButton* save;

  QVBox* padding;

 public:
  ControlSave(QWidget* parent,FracplanetMain* save_target,ParametersSave* param);
  virtual ~ControlSave()
    {}

  public slots:
    void setAtmosphere(int v)
    {
      parameters->atmosphere=(v==2);
    }
  void setSeaSphere(int v)
    {
      parameters->sea_object=(v==2);
    }
};



#endif
