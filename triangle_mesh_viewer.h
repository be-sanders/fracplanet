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
  \brief Interface for class TriangleMeshViewer.
*/

#ifndef _triangle_mesh_viewer_h_
#define _triangle_mesh_viewer_h_

#include <qwidget.h>
#include <qvbox.h>
#include <qhbox.h>
#include <qslider.h>
#include <qgroupbox.h>
#include <qgrid.h>
#include <qpushbutton.h>
#include <qdatetime.h>
#include <qtimer.h>
#include <qlabel.h>

#include <vector>

#include "useful.h"
#include "random.h"

#include "triangle_mesh.h"
#include "parameters_render.h"

#include "triangle_mesh_viewer_display.h"

//! A class to display a triangle mesh.
/*! Wraps a TriangleMeshViewerDisplay with some controls.
  \todo Add better controls.
*/
class TriangleMeshViewer : public QGrid
{
 private:
  Q_OBJECT;

 protected:
  //! Pointer to the rendering parameters.
  const ParametersRender* parameters;

  //! The actual rendering area.
  TriangleMeshViewerDisplay* display;

  //! Timer for driving animation.
  QTimer* timer;

  //! Time for animation progress
  QTime* clock;

  //! Label and box around the elevation slider.
  QGroupBox* tilt_box;
  
  //! Elevation slider.
  QSlider* tilt_slider;

  //! Fly button
  QPushButton* fly_button;
  
  //! Label and box arond the spin-rate slider.
  QGroupBox* spinrate_box;

  //! Spin rate slider.
  QSlider* spinrate_slider;

  //! Display speed etc
  QLabel* fly_info;

  //@{
  //! Parameter of camera position.
  XYZ camera_position;
  XYZ camera_forward;
  XYZ camera_up;
  float camera_velocity;
  float camera_yaw_rate;
  float camera_pitch_rate;
  float camera_roll_rate;
  //@}

  //@{
  //! Parameters of object
  float object_tilt;
  float object_rotation;
  float object_spinrate;
  //@}
  
  //@{
  //! Key state
  bool keypressed_arrow_left;
  bool keypressed_arrow_right;
  bool keypressed_arrow_up;
  bool keypressed_arrow_down;
  bool keypressed_mouse_left;
  bool keypressed_mouse_right;
  //@}

  //! Whether in fly mode
  bool fly_mode;

 public:
  //! Constructor.
  TriangleMeshViewer(QWidget* parent,const ParametersRender* param,const std::vector<const TriangleMesh*>& m);

  virtual ~TriangleMeshViewer();

  //! Sets the TriangleMesh to be displayed.
  void set_mesh(const std::vector<const TriangleMesh*>& m);

 protected:
  //! Interested in some key presses
  void keyPressEvent(QKeyEvent* e);

  //! Interested in some key state
  void keyReleaseEvent(QKeyEvent* e);

  //! Interested in mouse clicks for steering
  void mousePressEvent(QMouseEvent* e);

  //! Interested in some button state
  void mouseReleaseEvent(QMouseEvent* e);
  
  //! Interested in mouse position for steering
  void mouseMoveEvent(QMouseEvent* e);

  //! Interested in wheel for speed
  void wheelEvent(QWheelEvent* e);

 public slots:
  void fly();
  void unfly();

  void set_tilt(int v);
  void set_spinrate(int v);
  
 private slots:
  void tick();
};

#endif



