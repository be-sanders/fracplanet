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
#ifndef _triangle_mesh_viewer_display_h_
#define _triangle_mesh_viewer_display_h_

#include <qwidget.h>
#include <qtimer.h>
#include <qgl.h>

#include "useful.h"
#include "random.h"

#include "triangle_mesh.h"
#include "parameters_render.h"

class TriangleMeshViewerDisplay : public QGLWidget
{
 private:
  Q_OBJECT;
  
 protected:
  const TriangleMesh* mesh;
  const ParametersRender* parameters;

  QTimer* timer;
  uint frame;
  
  uint width;
  uint height;

  Random01 r01;

  float camera_elevation;
  float camera_spin_rate;
  float camera_azimuth;
  float camera_distance;

 public:
  TriangleMeshViewerDisplay(QWidget* parent,const ParametersRender* param,const TriangleMesh* m=0);

  void set_mesh(const TriangleMesh* m);

  virtual void paintGL();
  virtual void initializeGL();
  virtual void resizeGL(int w,int h);
  
  public slots:
    void setElevation(int);
  void setSpinRate(int);
  
  private slots:
    
    void tick();  
};

#endif
