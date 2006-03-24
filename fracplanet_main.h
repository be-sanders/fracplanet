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
#include <qprogressdialog.h>

#include <iostream>
#include <vector>

#include <boost/scoped_ptr.hpp>

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
#include "triangle_mesh_cloud.h"
#include "triangle_mesh_viewer.h"

//! Top level GUI component for fracplanet application: contains parameter controls and viewing area
class FracplanetMain : public QHBox , public Progress
{
 private:
  Q_OBJECT
protected:
  QApplication*const application;
  
  //! Owned terrain.
  boost::scoped_ptr<const TriangleMeshTerrain> mesh_terrain;

  //! Owned clouds, if any.
  boost::scoped_ptr<const TriangleMeshCloud> mesh_cloud;

  //! Downcast version for use by mesh viewer.
  std::vector<const TriangleMesh*> meshes;

  ParametersTerrain parameters_terrain;
  ParametersCloud parameters_cloud;
  ParametersSave parameters_save;
  ParametersRender parameters_render;
  
  QVBox* vbox;
  
  ControlRender* control_render;
  ControlSave* control_save;
  ControlTerrain* control_terrain;
  ControlAbout* control_about;

  boost::scoped_ptr<TriangleMeshViewer> viewer;
  QTabWidget* tab;

  uint last_step;

  std::auto_ptr<QProgressDialog> progress_dialog;
  std::string progress_info;
  bool progress_was_stalled;

  bool startup;

 public:
  FracplanetMain(QWidget* parent,QApplication* app);
  virtual ~FracplanetMain();
  
  virtual void progress_start(uint target,const std::string&);
  virtual void progress_stall(const std::string& reason);
  virtual void progress_step(uint step);
  virtual void progress_complete(const std::string&);
  
 public slots:
    
  //! Invoked by ControlTerrain to generate new TriangleMesh.
  void regenerate();
  
  //! Invoked by ControlSave to save to file (POV-Ray format).
  void save_pov();

  //! Invoked by ControlSave to save to file (Blender format).
  void save_blender();
};

#endif
