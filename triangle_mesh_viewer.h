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
 protected:
  //! The actual rendering area.
  TriangleMeshViewerDisplay* display;

  //! Label and box around the elevation slider.
  QGroupBox* elevation_box;
  
  //! Elevation slider.
  QSlider* elevation_slider;
  
  //! Label and box arond the spin-rate slider.
  QGroupBox* spinrate_box;

  //! Spin rate slider.
  QSlider* spinrate_slider;
  
 public:
  //! Constructor.
  TriangleMeshViewer(QWidget* parent,const ParametersRender* param,const TriangleMesh* m=0);

  //! Sets the TriangleMesh to be displayed.
  void set_mesh(const TriangleMesh* m);
};

#endif



