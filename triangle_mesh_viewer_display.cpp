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
#include "triangle_mesh_viewer_display.h"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <numeric>

TriangleMeshViewerDisplay::TriangleMeshViewerDisplay(QWidget* parent,const ParametersRender* param,const TriangleMesh* m)
  :QGLWidget(parent)
   ,mesh(m)
   ,parameters(param)
   ,frame(0)
   ,width(0)
   ,height(0)
   ,frame_time()
   ,camera_elevation(0.0)
   ,camera_spin_rate(0.0)
   ,camera_azimuth(0.0)
   ,camera_distance(4.5)
{
  timer=new QTimer(this);
  
  frame_time.start();
  frame_time_reported.start();

  connect(timer,SIGNAL(timeout()),this,SLOT(tick()));
  
  timer->start(static_cast<int>(ceil(1000.0f/parameters->fps_target)));

  // Zero is not a valid value according to red book, so use it to designate unset
  gl_display_list_index=0;
}

void TriangleMeshViewerDisplay::set_mesh(const TriangleMesh* m)
{
  mesh=m;

  // If there is a display list allocated for the current mesh, delete it.
  if (gl_display_list_index!=0)
    {
      glDeleteLists(gl_display_list_index,1);
      gl_display_list_index=0;
    }  
}

void TriangleMeshViewerDisplay::paintGL()
{
  glClearColor(0.0,0.0,0.0,1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glLoadIdentity();
  gluLookAt(0.0,-camera_distance*cos(camera_elevation),camera_distance*sin(camera_elevation), 0.0,0.0,0.0, 0.0,0.0,1.0);

  glMatrixMode(GL_MODELVIEW);
  glRotatef((180.0/M_PI)*camera_azimuth,0.0,0.0,1.0);

  glPolygonMode(GL_FRONT,(parameters->wireframe ? GL_LINE : GL_FILL));
  
  if (parameters->display_list && gl_display_list_index!=0)
    {
      glCallList(gl_display_list_index);
    }
  else
    {
      bool building_display_list=(parameters->display_list && gl_display_list_index==0);

      if (building_display_list)
	{
	  gl_display_list_index=glGenLists(1);
	  glNewList(gl_display_list_index,GL_COMPILE_AND_EXECUTE);
	  std::cerr << "Building display list...\n";
	}

      GLfloat default_material_white[3]={1.0f,1.0f,1.0f};
      GLfloat default_material_black[3]={0.0f,0.0f,0.0f};
      glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,default_material_white);
      glMaterialfv(GL_FRONT,GL_EMISSION,default_material_black);

      if (mesh->emissive()==0.0f)
	{
	  // Use "Color Material" mode 'cos everything is the same material.... just change the colour
	  glEnable(GL_COLOR_MATERIAL);
	  glColorMaterial(GL_FRONT,GL_AMBIENT_AND_DIFFUSE);

	  // Point GL at arrays of data
	  glVertexPointer(3,GL_FLOAT,sizeof(Vertex),&(mesh->vertex(0).position().x));
	  glNormalPointer(GL_FLOAT,sizeof(Vertex),&(mesh->vertex(0).normal().x));
	  glColorPointer(3,GL_UNSIGNED_BYTE,sizeof(Vertex),&(mesh->vertex(0).colour(0).r));

	  // Draw the colour-zero triangles
	  glDrawElements(GL_TRIANGLES,3*mesh->triangles_of_colour0(),GL_UNSIGNED_INT,&(mesh->triangle(0).vertex(0)));

	  // Switch to alternate colour and draw the colour-one triangles
	  glColorPointer(3,GL_UNSIGNED_BYTE,sizeof(Vertex),&(mesh->vertex(0).colour(1).r));
	  glDrawElements(GL_TRIANGLES,3*mesh->triangles_of_colour1(),GL_UNSIGNED_INT,&(mesh->triangle(mesh->triangles_of_colour0()).vertex(0)));

	  glDisable(GL_COLOR_MATERIAL);
	}
      else
	{
	  // If there could be emissive vertices, we need to do things the hard way.	  

	  const float k=1.0f/255.0f;
	  const float em=k*(     mesh->emissive());
	  const float ad=k*(1.0f-mesh->emissive());

	  glBegin(GL_TRIANGLES);
	  for (unsigned int t=0;t<mesh->triangles();t++)
	    {
	      const uint c=(t<mesh->triangles_of_colour0() ? 0 : 1);

	      for (uint i=0;i<3;i++)
		{
		  const uint vn=mesh->triangle(t).vertex(i);
		  const Vertex& v=mesh->vertex(vn);
		  
		  GLfloat c_ad[3];
		  GLfloat c_em[3];
		  if (v.emissive(c))
		    {
		      c_ad[0]=v.colour(c).r*ad;
		      c_ad[1]=v.colour(c).g*ad;
		      c_ad[2]=v.colour(c).b*ad;
		      c_em[0]=v.colour(c).r*em;
		      c_em[1]=v.colour(c).g*em;
		      c_em[2]=v.colour(c).b*em;
		    }
		  else
		    {
		      c_ad[0]=v.colour(c).r*k;
		      c_ad[1]=v.colour(c).g*k;
		      c_ad[2]=v.colour(c).b*k;
		      c_em[0]=0.0f;
		      c_em[1]=0.0f;
		      c_em[2]=0.0f;
		    }

		  glNormal3fv(&(v.normal().x));
		  glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,c_ad);
		  glMaterialfv(GL_FRONT,GL_EMISSION,c_em);
		  glVertex3fv(&(v.position().x));
		}
	    }
	  glEnd();
	}

      if (building_display_list)
	{
	  glEndList();
	  std::cerr << "...built display list\n";
	}
    }

  glFlush();

  // Get time taken since last frame
  const uint dt=frame_time.restart();

  // Save it in the queue
  frame_times.push_back(dt);

  // Keep last 10 frame times
  while (frame_times.size()>10) frame_times.pop_front();

  // Only update frame time a couple of times a second to reduce flashing
  if (parameters->notify && frame_time_reported.elapsed()>500)
  {    
    const float average_time=std::accumulate(frame_times.begin(),frame_times.end(),0)/static_cast<float>(frame_times.size());
   
    const float fps=1000.0/average_time;

    std::ostringstream report;
    report.setf(std::ios::fixed);
    report.precision(1);
    
    report << "Triangles: " << mesh->triangles() << "\n";
    report << "Vertices : " << mesh->vertices() << "\n";
    report << "FPS (av) : " << fps << "\n";
    
    parameters->notify->notify(report.str());
    frame_time_reported.restart();
  }
}

void TriangleMeshViewerDisplay::initializeGL()
{
  glClearColor(0.0,0.0,0.0,1.0);

  std::cerr << "Double buffering " << (doubleBuffer() ? "ON" : "OFF") << "\n";
  std::cerr << "Auto Buffer Swap " << (autoBufferSwap() ? "ON" : "OFF") << "\n";

  // Switch depth-buffering on
  glEnable(GL_DEPTH_TEST);

  // Set up simple lighting
  GLfloat white_light[]={1.0,1.0,1.0,1.0};
  GLfloat light_position[]={2.0,1.0,3.0,0.0};  // Seems to be relative to the camera
  
  glShadeModel(GL_FLAT);
  glLightfv(GL_LIGHT0,GL_POSITION,light_position);
  glLightfv(GL_LIGHT0,GL_DIFFUSE,white_light);

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

  // Do smooth shading 'cos colours are specified at vertices
  glShadeModel(GL_SMOOTH);

  // Don't waste time on back-facers
  glFrontFace(GL_CCW);
  glCullFace(GL_BACK);
  glEnable(GL_CULL_FACE);

  // Use arrays of data
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
}

void TriangleMeshViewerDisplay::resizeGL(int w,int h)
{
  width=w;
  height=h;

  glViewport(0,0,(GLint)w,(GLint)h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  // View angle is specified in vertical direction, but we need to exaggerate it if image is taller than wide.
  const float view_angle_degrees=minimum(90.0,width>height ? 30.0 : 30.0*height/width);

  gluPerspective(view_angle_degrees,(float)width/(float)height,0.1,10.0);

  glMatrixMode(GL_MODELVIEW);

  updateGL();
}

void TriangleMeshViewerDisplay::tick()
{
  frame++;
  camera_azimuth+=0.01*camera_spin_rate;
  updateGL();
}

void TriangleMeshViewerDisplay::setElevation(int e)
{
  camera_elevation=e*(-M_PI/180.0);
}

void TriangleMeshViewerDisplay::setSpinRate(int s)
{
  camera_spin_rate=s*(-M_PI/180.0);
}
