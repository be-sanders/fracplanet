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
  \brief Interface for class TriangleMeshViewerDisplay.
*/

#ifndef _triangle_mesh_viewer_display_h_
#define _triangle_mesh_viewer_display_h_

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>

#include <qwidget.h>
#include <qgl.h>
#include <qdatetime.h>

#include <deque>
#include <vector>

#include "useful.h"
#include "random.h"

#include "triangle_mesh.h"
#include "parameters_render.h"

//! Contains the actual rendering functionality of a TriangleMeshViewer.
class TriangleMeshViewerDisplay : public QGLWidget
{
 private:
  Q_OBJECT;

  void check_for_gl_errors(const char*) const;
  
 protected:
  //! The meshes being displayed.
  /*! NB NOT owned here
   */
  std::vector<const TriangleMesh*> mesh;

  //! Pointer to the rendering parameters.
  const ParametersRender* parameters;

  //! GL display list index
  uint gl_display_list_index;

  //! Frame count.
  uint frame_number;
  
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
  XYZ camera_position;
  XYZ camera_lookat;
  XYZ camera_up;
  //@}

  //@{
  //! Parameters of object
  float object_tilt;
  float object_rotation;
  //@}

  //! Compute background colour from render parameters and camera height
  const FloatRGBA background_colour() const;

 public:
  //! Constructor.
  TriangleMeshViewerDisplay(QWidget* parent,const ParametersRender* param,const std::vector<const TriangleMesh*>& m);

  //! Set the mesh being rendered.
  void set_mesh(const std::vector<const TriangleMesh*>& m);

  //! Called to repaint GL area.
  virtual void paintGL();

  //! Set up OpenGL.
  virtual void initializeGL();

  //! Deal with resize.
  virtual void resizeGL(int w,int h);
  
  public slots:
  
  //! Called to redisplay scene
  void draw_frame(const XYZ& p,const XYZ& l,const XYZ& u,float r,float t);
};

#endif
