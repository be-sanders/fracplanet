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
#include "triangle_mesh_viewer.h"


TriangleMeshViewer::TriangleMeshViewer(QWidget* parent,const ParametersRender* param,const TriangleMesh* mesh)
  :QGrid(2,Qt::Horizontal,parent)
{
  setSpacing(5);

  display=new TriangleMeshViewerDisplay(this,param,mesh);

  elevation_box=new QGroupBox(1,Qt::Horizontal,"Elevation",this);
  spinrate_box=new QGroupBox(1,Qt::Horizontal,"Spin Rate",this);

  elevation_slider=new QSlider(-80,80,10, 0,Qt::Vertical,elevation_box);
  spinrate_slider =new QSlider(-80,80,10, 0,Qt::Horizontal,spinrate_box);

  elevation_slider->setTickInterval(10);
  spinrate_slider->setTickInterval(10);

  elevation_slider->setTickmarks(QSlider::Both);
  spinrate_slider->setTickmarks(QSlider::Both);

  elevation_slider->setTracking(true);
  spinrate_slider->setTracking(true);

  connect(
	  elevation_slider,SIGNAL(valueChanged(int)),
	  display,SLOT(setElevation(int))
	  );
  connect(
	  spinrate_slider,SIGNAL(valueChanged(int)),
	  display,SLOT(setSpinRate(int))
	  );
}

void TriangleMeshViewer::set_mesh(const TriangleMesh* m)
{
  display->set_mesh(m);
}

