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
#include "triangle_mesh_viewer.h"


TriangleMeshViewer::TriangleMeshViewer(QWidget* parent,const ParametersRender* param,const TriangleMesh* mesh)
  :QGrid(2,Qt::Horizontal,parent)
  ,parameters(param)
  ,camera_position(-4.5f,0.0f,0.0f)
  ,camera_forward(1.0f,0.0f,0.0f)
  ,camera_up(0.0f,0.0f,1.0f)
  ,camera_velocity(0.0f)
  ,object_tilt(0.0f)
  ,object_rotation(0.0f)
  ,object_spinrate(0.0f)
  ,fly_mode(false)
{
  setSpacing(5);

  display=new TriangleMeshViewerDisplay(this,param,mesh);

  tilt_box=new QGroupBox(1,Qt::Horizontal,"Tilt",this);
  spinrate_box=new QGroupBox(1,Qt::Horizontal,"Spin Rate",this);

  tilt_slider=new QSlider(-80,80,10, 0,Qt::Vertical,tilt_box);
  spinrate_slider =new QSlider(-80,80,10, 0,Qt::Horizontal,spinrate_box);

  fly_button=new QPushButton("Fly",this);

  tilt_slider->setTickInterval(10);
  spinrate_slider->setTickInterval(10);

  tilt_slider->setTickmarks(QSlider::Both);
  spinrate_slider->setTickmarks(QSlider::Both);

  tilt_slider->setTracking(true);
  spinrate_slider->setTracking(true);

  connect(
	  tilt_slider,SIGNAL(valueChanged(int)),
	  this,SLOT(set_tilt(int))
	  );
  connect(
	  spinrate_slider,SIGNAL(valueChanged(int)),
	  this,SLOT(set_spinrate(int))
	  );
  connect(
	  fly_button,SIGNAL(clicked()),
	  this,SLOT(fly())
	  );

  timer=new QTimer(this);
  
  connect(timer,SIGNAL(timeout()),this,SLOT(tick()));
  
  timer->start(static_cast<int>(ceil(1000.0f/parameters->fps_target)));
  
  setMouseTracking(true);  // To get moves regardless of button state
  display->setMouseTracking(true);
}

TriangleMeshViewer::~TriangleMeshViewer()
{}

void TriangleMeshViewer::keyPressEvent(QKeyEvent* e)
{
  if (fly_mode && e->key()==Qt::Key_Escape)
    {
      unfly();
    }
  else
    {
      e->ignore();
    }
}

void TriangleMeshViewer::mouseMoveEvent(QMouseEvent* e)
{
  if (fly_mode)
    {
      const float kt=1.0f/parameters->fps_target;
      
      const float nx=2.0*(e->pos().x()/static_cast<float>(size().width())-0.5f);
      const float ny=2.0*(0.5f-e->pos().y()/static_cast<float>(size().height()));

      //! \todo: Build proper rotation matrix, then apply to all vectors and re-normalize/orthognalize
      const XYZ camera_right((camera_forward*camera_up).normalised());
      camera_forward=(camera_forward+kt*nx*camera_right+kt*ny*camera_up).normalised();
      camera_up=(camera_right*camera_forward).normalised();
    }
  else
    {
      e->ignore();
    }
}


void TriangleMeshViewer::set_mesh(const TriangleMesh* m)
{
  if (fly_mode) unfly();
  display->set_mesh(m);
}

void TriangleMeshViewer::fly()
{
  fly_mode=true;
  camera_velocity=0.1f;
  spinrate_slider->setValue(0);
  tilt_box->hide();
  spinrate_box->hide();
  fly_button->hide();
  display->updateGeometry();
}

void TriangleMeshViewer::unfly()
{
  fly_mode=false;
  camera_position=XYZ(-4.5f,0.0f,0.0f);
  camera_forward=XYZ(1.0f,0.0f,0.0f);
  camera_up=XYZ(0.0f,0.0f,1.0f);
  camera_velocity=0.0f;
  tilt_box->show();
  spinrate_box->show();
  fly_button->show();
  display->updateGeometry();
}

void TriangleMeshViewer::tick()
{
  const float kt=1.0f/parameters->fps_target;

  camera_position+=(kt*camera_velocity)*camera_forward;
  object_rotation+=object_spinrate*kt;

  display->draw_frame(camera_position,camera_position+camera_forward,camera_up,object_rotation,object_tilt);
}

void TriangleMeshViewer::set_tilt(int v)
{
  object_tilt=v*(M_PI/180.0);
}

void TriangleMeshViewer::set_spinrate(int v)
{
  object_spinrate=v*(M_PI/180.0);
}
