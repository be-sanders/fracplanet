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
  \brief Interface for class TriangleMeshViewerDisplay.
*/

#ifndef _triangle_mesh_viewer_display_h_
#define _triangle_mesh_viewer_display_h_

#include <qwidget.h>
#include <qtimer.h>
#include <qgl.h>
#include <qdatetime.h>

#include <deque>

#include "useful.h"
#include "random.h"

#include "triangle_mesh.h"
#include "parameters_render.h"

//! Contains the actual rendering functionality of a TriangleMeshViewer.
class TriangleMeshViewerDisplay : public QGLWidget
{
 private:
  Q_OBJECT;
  
 protected:
  //! The mesh being displayed.
  const TriangleMesh* mesh;

  //! Pointer to the rendering parameters.
  const ParametersRender* parameters;

  //! GL display list index
  uint gl_display_list_index;

  //! Timer for driving frames.
  QTimer* timer;

  //! Frame count.
  uint frame;
  
  //! Display area width.
  uint width;

  //! Display area height.
  uint height;

  //! Time frames for FPS measurement.
  QTime frame_time;
  
  //! Time since FPS last reported.
  QTime frame_time_reported;

  //! Queue of frame times to average.
  std::deque<uint> frame_times;

  //@{
  //! Parameter of camera position.
  float camera_elevation;
  float camera_spin_rate;
  float camera_azimuth;
  float camera_distance;
  //@}

 public:
  //! Constructor.
  TriangleMeshViewerDisplay(QWidget* parent,const ParametersRender* param,const TriangleMesh* m=0);

  //! Set the mesh being rendered.
  void set_mesh(const TriangleMesh* m);

  //! Called to repaint GL area.  (Driven by updateGL in tick() slot).
  virtual void paintGL();

  //! Set up OpenGL.
  virtual void initializeGL();

  //! Deal with resize.
  virtual void resizeGL(int w,int h);
  
  public slots:

  //! Signalled from elevation slider.
  void setElevation(int);

  //! Signalled from spinrate slider.
  void setSpinRate(int);
  
  private slots:
  
  //! Driven by timer.
  void tick();  
};

#endif
