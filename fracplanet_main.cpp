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
#include "fracplanet_main.h"

#include <qapplication.h>
#include <qcursor.h>

FracplanetMain::FracplanetMain(QWidget* parent)
  :QHBox(parent)
   ,mesh(0)
   ,last_step(0)
{
  vbox=new QVBox(this);
  setStretchFactor(vbox,1);

  control_terrain=new ControlTerrain(this,this,&parameters_terrain);
  control_save=new ControlSave(this,this,&parameters_save);
  control_render=new ControlRender(this,&parameters_render);
  control_about=new ControlAbout(this);

  tab=new QTabWidget(vbox);
  
  tab->addTab(control_terrain,"Terrain");
  tab->addTab(control_save,"Save");
  tab->addTab(control_render,"Render");
  tab->addTab(control_about,"About");

  progress_box=new QGroupBox(2,Qt::Vertical,"Progress",vbox);
  progress_label=new QLabel(progress_box);
  progress_bar=new QProgressBar(progress_box);

  viewer=new TriangleMeshViewer(this,&parameters_render);    
  setStretchFactor(viewer,2);

  regenerate();  
}

void FracplanetMain::progress_start(uint target,const std::string& info)
{
  progress_label->setText(info.c_str());
  progress_label->repaint();
  progress_bar->setTotalSteps(target);
  progress_bar->reset();
  last_step=(uint)-1;
  
  QApplication::setOverrideCursor(Qt::WaitCursor);  
}

void FracplanetMain::progress_step(uint step)
{
  // We might be called lots of times with the same step.  Don't know if Qt handles this efficiently so check for it ourselves.
  if (step!=last_step)
    {
      progress_bar->setProgress(step);
      last_step=step;
    }
}

void FracplanetMain::progress_complete(const std::string& info)
{
  progress_label->setText(info.c_str());
  progress_label->repaint();
  progress_bar->reset();
  last_step=(uint)-1;

  QApplication::restoreOverrideCursor();
}

void FracplanetMain::regenerate()
{
  delete mesh;

  // There are some issues with type here:
  // We need to keep hold of a pointer to TriangleMeshTerrain so we can call its write_povray method
  //  but the triangle viewer needs the TriangleMesh.
  // So we end up with code like this to avoid a premature downcast.
  switch (parameters_terrain.object_type)
    {
    case ParametersTerrain::ObjectTypePlanet:
      {
	TriangleMeshTerrainPlanet* it=new TriangleMeshTerrainPlanet(parameters_terrain,this);
	mesh=it;
	viewer->set_mesh(it);
	break;
      }
    case ParametersTerrain::ObjectTypeTerrain:
      {
	TriangleMeshTerrainFlat* it=new TriangleMeshTerrainFlat(parameters_terrain,this);
	mesh=it;
	viewer->set_mesh(it);
	break;
      }
    }
}

void FracplanetMain::save()
{
  mesh->write_povray(parameters_save);
}
