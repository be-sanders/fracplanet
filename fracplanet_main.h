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
  \brief Interface for class FracplanetMain.
*/

#ifndef _fracplanet_main_h_
#define _fracplanet_main_h_

extern "C"
{
#include <time.h>
}

#include <qtabwidget.h>
#include <qhbox.h>
#include <qvbox.h>
#include <qlabel.h>

#include <iostream>

#include "useful.h"
#include "random.h"

#include "parameters_terrain.h"
#include "parameters_save.h"
#include "parameters_render.h"
#include "control_terrain.h"
#include "control_save.h"
#include "control_render.h"
#include "control_about.h"

#include "triangle_mesh_terrain.h"
#include "triangle_mesh_viewer.h"

//! Top level GUI component for fracplanet application: contains parameter controls and viewing area
class FracplanetMain : public QHBox , public Progress
{
 private:
  Q_OBJECT
protected:
  //! The mesh being rendered.
  TriangleMeshTerrain* mesh;

  ParametersTerrain parameters_terrain;
  ParametersSave parameters_save;
  ParametersRender parameters_render;

  QVBox* vbox;

  ControlRender* control_render;
  ControlSave* control_save;
  ControlTerrain* control_terrain;
  ControlAbout* control_about;

  TriangleMeshViewer* viewer;
  QTabWidget* tab;

  uint last_step;
  QGroupBox* progress_box;
  QLabel* progress_label;
  QProgressBar* progress_bar;


 public:
  FracplanetMain(QWidget* parent);

  virtual void progress_start(uint target,const std::string&);
  virtual void progress_step(uint step);
  virtual void progress_complete(const std::string&);
  
  public slots:

  //! Invoked by ControlTerrain to generate a new TriangleMesh.
  void regenerate();

  //! Invoked by ControlSave to save to file.
  void save();
};

#endif
