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
#include <sstream>
#include <qcursor.h>

TriangleMeshViewer::TriangleMeshViewer(QWidget* parent,const ParametersRender* param,const TriangleMesh* mesh)
  :QGrid(2,Qt::Horizontal,parent)
  ,parameters(param)
  ,camera_position(-3.0f,0.0f,0.0f)
  ,camera_forward(1.0f,0.0f,0.0f)
  ,camera_up(0.0f,0.0f,1.0f)
  ,camera_velocity(0.0f)
  ,camera_yaw_rate(0.0f)
  ,camera_pitch_rate(0.0f)
  ,camera_roll_rate(0.0f)
  ,object_tilt(-30.0f*M_PI/180.0f)
  ,object_rotation(0.0f)
  ,object_spinrate(0.0f)
  ,keypressed_arrow_left(false)
  ,keypressed_arrow_right(false)
  ,keypressed_arrow_up(false)
  ,keypressed_arrow_down(false)
  ,keypressed_mouse_left(false)
  ,keypressed_mouse_right(false)
  ,fly_mode(false)
{
  setSpacing(5);

  display=new TriangleMeshViewerDisplay(this,param,mesh);

  tilt_box=new QGroupBox(1,Qt::Horizontal,"Tilt",this);
  spinrate_box=new QGroupBox(1,Qt::Horizontal,"Spin Rate",this);

  tilt_slider=new QSlider(-80,80,10,-30,Qt::Vertical,tilt_box);
  spinrate_slider =new QSlider(-80,80,10, 0,Qt::Horizontal,spinrate_box);

  fly_button=new QPushButton("Fly",this);

  tilt_slider->setTickInterval(10);
  spinrate_slider->setTickInterval(10);

  tilt_slider->setTickmarks(QSlider::Both);
  spinrate_slider->setTickmarks(QSlider::Both);

  tilt_slider->setTracking(true);
  spinrate_slider->setTracking(true);

  fly_info=new QLabel("-",this);
  fly_info->hide();

  //Calling setFocus() when switching to fly mode seems to avoid need for this, but do it anyway.
  tilt_slider->setFocusPolicy(QWidget::NoFocus);
  spinrate_slider->setFocusPolicy(QWidget::NoFocus);

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

  clock=new QTime();
  clock->start();

  timer=new QTimer(this);  
  connect(timer,SIGNAL(timeout()),this,SLOT(tick()));
  timer->start(static_cast<int>(ceil(1000.0f/parameters->fps_target)));
  
  setMouseTracking(true);  // To get moves regardless of button state
  display->setMouseTracking(true);
  setFocus();
}

TriangleMeshViewer::~TriangleMeshViewer()
{
  delete clock;
}

void TriangleMeshViewer::keyPressEvent(QKeyEvent* e)
{
  if (fly_mode)
    {
      if (e->key()==Qt::Key_Escape) unfly();
      else if (e->key()==Qt::Key_Left) keypressed_arrow_left=true;
      else if (e->key()==Qt::Key_Right) keypressed_arrow_right=true;
      else if (e->key()==Qt::Key_Up) keypressed_arrow_up=true;
      else if (e->key()==Qt::Key_Down) keypressed_arrow_down=true;
      else e->ignore();
    }
  else
    {
      e->ignore();
    }
}

void TriangleMeshViewer::keyReleaseEvent(QKeyEvent* e)
{
  if (fly_mode)
    {
      if (e->key()==Qt::Key_Left) keypressed_arrow_left=false;
      else if (e->key()==Qt::Key_Right) keypressed_arrow_right=false;
      else if (e->key()==Qt::Key_Up) keypressed_arrow_up=false;
      else if (e->key()==Qt::Key_Down) keypressed_arrow_down=false;
      else e->ignore();
    }
  else
    {
      e->ignore();
    }
}

void TriangleMeshViewer::mousePressEvent(QMouseEvent* e)
{
  if (fly_mode)
    {
      if (e->button()==Qt::LeftButton) keypressed_mouse_left=true;
      else if (e->button()==Qt::RightButton) keypressed_mouse_right=true;
      else if (e->button()==Qt::MidButton) camera_velocity=0.0f;
      else e->ignore();
    }
  else
    {
      e->ignore();
    }
}

void TriangleMeshViewer::mouseReleaseEvent(QMouseEvent* e)
{
  if (fly_mode)
    {
      if (e->button()==Qt::LeftButton) keypressed_mouse_left=false;
      else if (e->button()==Qt::RightButton) keypressed_mouse_right=false;
      else e->ignore();
    }
  else
    {
      e->ignore();
    }
}


void TriangleMeshViewer::wheelEvent(QWheelEvent* e)
{
  if (fly_mode)
    {
      camera_velocity+=e->delta()*(0.03125f/480.0f);
    }
  else
    {
      e->ignore();
    }
}

inline float signedsquare(float v) {return (v<0.0f ? -v*v : v*v);}

void TriangleMeshViewer::mouseMoveEvent(QMouseEvent* e)
{
  if (fly_mode)
    {
      camera_yaw_rate=4.0f*signedsquare(e->pos().x()/static_cast<float>(size().width())-0.5f);
      camera_pitch_rate=(parameters->joystick_mouse ? 1.0f : -1.0f)*4.0f*signedsquare(e->pos().y()/static_cast<float>(size().height())-0.5f);
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
  camera_velocity=0.125f;
  spinrate_slider->setValue(0);
  tilt_box->hide();
  spinrate_box->hide();
  fly_button->hide();
  fly_info->show();
  display->updateGeometry();
  setFocus();
  QCursor::setPos(mapToGlobal(QPoint(width()/2,height()/2)));
}

void TriangleMeshViewer::unfly()
{
  fly_mode=false;
  camera_position=XYZ(-3.0f,0.0f,0.0f);
  camera_forward=XYZ(1.0f,0.0f,0.0f);
  camera_up=XYZ(0.0f,0.0f,1.0f);
  camera_velocity=0.0f;
  camera_yaw_rate=0.0f;
  camera_pitch_rate=0.0f;
  camera_roll_rate=0.0f;
  fly_info->hide();
  tilt_box->show();
  spinrate_box->show();
  fly_button->show();
  display->updateGeometry();
}

void TriangleMeshViewer::tick()
{
  const float dt=0.001f*clock->restart();

  camera_roll_rate=0.0f;
  if (keypressed_arrow_left || keypressed_mouse_left) camera_roll_rate+=0.5f;
  if (keypressed_arrow_right || keypressed_mouse_right) camera_roll_rate-=0.5f;

  if (keypressed_arrow_up) camera_velocity+=120.0f*(0.03125f/480.0f);
  if (keypressed_arrow_down) camera_velocity-=120.0f*(0.03125f/480.0f);
  
  //! \todo Replace cheesy rotation hacks with proper rotation matrices

  XYZ camera_right=(camera_forward*camera_up).normalised();

  const XYZ camera_right_rolled=camera_right+(dt*camera_roll_rate)*camera_up;
  const XYZ camera_up_rolled=camera_up-(dt*camera_roll_rate)*camera_right;

  camera_right=camera_right_rolled.normalised();
  camera_up=camera_up_rolled.normalised();

  camera_forward=(camera_forward+dt*camera_yaw_rate*camera_right+dt*camera_pitch_rate*camera_up).normalised();
  camera_up=(camera_right*camera_forward).normalised();

  camera_position+=(dt*camera_velocity)*camera_forward;
  object_rotation+=object_spinrate*dt;

  if (display->isVisible())
    {
      display->draw_frame(camera_position,camera_position+camera_forward,camera_up,object_rotation,object_tilt);
    }

  std::ostringstream msg;
  msg << "Velocity: " << camera_velocity << "  Roll rate:" << camera_roll_rate << "\n";
  msg << "Exit flight: Esc  Speed: up/down keys or mouse wheel  Roll: left/right keys or mouse buttons";
  fly_info->setText(msg.str().c_str());
}

void TriangleMeshViewer::set_tilt(int v)
{
  object_tilt=v*(M_PI/180.0);
}

void TriangleMeshViewer::set_spinrate(int v)
{
  object_spinrate=v*(M_PI/180.0);
}
