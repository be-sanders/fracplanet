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
#include "fracplanet_main.h"

#include <qapplication.h>
#include <qcursor.h>
#include <qfiledialog.h>
#include <qmessagebox.h>

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

  viewer=new TriangleMeshViewer(0,&parameters_render);     // Viewer will be a top-level-window
  viewer->resize(512,512);
  viewer->move(384,64);  // Moves view away from controls on most window managers

  regenerate(); 

  raise();   // On app start-up the control panel is the most important thing (regenerate raises the viewer window).
}

FracplanetMain::~FracplanetMain()
{
  delete viewer;
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
  viewer->hide();

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

  viewer->showNormal();
  viewer->raise();
}

void FracplanetMain::save()
{
  
  QString selected_filename=QFileDialog::getSaveFileName(".","POV-Ray (*.pov *.inc)",this,"Save object","Fracplanet: a .pov AND .inc file will be written");
  if (selected_filename.isEmpty())
    {
      QMessageBox::critical(this,"Fracplanet","No file specified\nNothing saved");
    }
  else
    {
      if (selected_filename.upper().endsWith(".POV") || selected_filename.upper().endsWith(".INC"))
	{
	  const std::string base_filename(selected_filename.left(selected_filename.length()-4).local8Bit());
	  const bool ok=mesh->write_povray(base_filename,parameters_save,parameters_terrain);
	  if (!ok) 
	    {
	      QMessageBox::critical(this,"Fracplanet","Errors ocurred while the files were being written.");
	    }
	}
      else
	{
	  QMessageBox::critical(this,"Fracplanet","File selected must have .pov or .inc suffix.");
	}
    }
}
