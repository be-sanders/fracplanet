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
   ,camera_position(-3.0f,0.0f,0.0f)
   ,camera_lookat(0.0f,0.0f,0.0f)
   ,camera_up(0.0f,0.0f,1.0f)
   ,object_tilt(-30.0f*M_PI/180.0f)
   ,object_rotation(0.0f)
{
  setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding,1,1));

  frame_time.start();
  frame_time_reported.start();

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

  const float a=parameters->ambient;

  GLfloat global_ambient[]={a,a,a,1.0};
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT,global_ambient);

  GLfloat light_diffuse[]={1.0-a,1.0-a,1.0-a,1.0};
  glLightfv(GL_LIGHT0,GL_DIFFUSE,light_diffuse);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  gluLookAt(
	    camera_position.x,camera_position.y,camera_position.z,
	    camera_lookat.x  ,camera_lookat.y  ,camera_lookat.z,
	    camera_up.x      ,camera_up.y      ,camera_up.z
	    );

  GLfloat light_position[]={-2.0,-3.0,1.0,0.0};  
  glLightfv(GL_LIGHT0,GL_POSITION,light_position);

  glRotatef((180.0/M_PI)*object_tilt,0.0,1.0,0.0);
  glRotatef((180.0/M_PI)*object_rotation,0.0,0.0,1.0);

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

  // Basic lighting stuff (set ambient globally rather than in light)
  GLfloat black_light[]={0.0,0.0,0.0,1.0};
  glLightfv(GL_LIGHT0,GL_AMBIENT,black_light);
  glLightfv(GL_LIGHT0,GL_SPECULAR,black_light);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHTING);

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
  const float view_angle_degrees=minimum(90.0,width>height ? 45.0 : 45.0*height/width);

  gluPerspective(view_angle_degrees,(float)width/(float)height,0.01,10.0);  // Was 0.1 (too far); 0.001 gives artefacts

  glMatrixMode(GL_MODELVIEW);

  updateGL();
}

void TriangleMeshViewerDisplay::draw_frame(const XYZ& p,const XYZ& l,const XYZ& u,float r,float t)
{
  frame++;

  camera_position=p;
  camera_lookat=l;
  camera_up=u;
  object_rotation=r;
  object_tilt=t;

  updateGL();
}


