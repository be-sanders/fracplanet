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
#include "fracplanet_main.h"

#include <qapplication.h>
#include <qcursor.h>
#include <qfiledialog.h>
#include <qmessagebox.h>
#include <qeventloop.h>

#include <fstream>

#include <boost/scoped_ptr.hpp>

#include "image.h"

FracplanetMain::FracplanetMain(QWidget* parent,QApplication* app,const boost::program_options::variables_map& opts)
  :QHBox(parent)
   ,application(app)
   ,mesh_terrain(0)
   ,mesh_cloud(0)
   ,parameters_terrain()
   ,parameters_cloud()
   ,parameters_render(opts)
   ,parameters_save(&parameters_render)
   ,last_step(0)
   ,progress_was_stalled(false)
   ,startup(true)
{
  vbox=new QVBox(this);
  setStretchFactor(vbox,1);

  control_terrain=new ControlTerrain(this,this,&parameters_terrain,&parameters_cloud);
  control_save=new ControlSave(this,this,&parameters_save);
  control_render=new ControlRender(this,&parameters_render);
  control_about=new ControlAbout(this);

  tab=new QTabWidget(vbox);
  
  tab->addTab(control_terrain,"Create");
  tab->addTab(control_save,"Save");
  tab->addTab(control_render,"Render");
  tab->addTab(control_about,"About");

  viewer.reset(new TriangleMeshViewer(0,&parameters_render,std::vector<const TriangleMesh*>()));     // Viewer will be a top-level-window
  viewer->resize(512,512);
  viewer->move(384,64);  // Moves view away from controls on most window managers

  regenerate();

  raise();   // On app start-up the control panel is the most important thing (regenerate raises the viewer window).
  
  startup=false;
}

FracplanetMain::~FracplanetMain()
{}

void FracplanetMain::progress_start(uint target,const std::string& info)
{
  if (startup) return;

  if (!progress_dialog.get())
    {
      progress_dialog=std::auto_ptr<QProgressDialog>(new QProgressDialog("Progress","Cancel",100,this,0,true));
      progress_dialog->setCancelButton(0);  // Cancel not supported
      progress_dialog->setAutoClose(false); // Avoid it flashing on and off
      progress_dialog->setMinimumDuration(0);
    }

  progress_was_stalled=false;
  progress_info=info;
  progress_dialog->reset();
  progress_dialog->setTotalSteps(target+1);   // Not sure why, but  +1 seems to avoid the progress bar dropping back to the start on completion
  progress_dialog->setLabelText(progress_info.c_str());
  progress_dialog->show();

  last_step=static_cast<uint>(-1);
  
  QApplication::setOverrideCursor(Qt::WaitCursor);  

  application->processEvents();
}

void FracplanetMain::progress_stall(const std::string& reason)
{
  progress_was_stalled=true;
  progress_dialog->setLabelText(reason.c_str());
  application->processEvents();
}

void FracplanetMain::progress_step(uint step)
{
  if (startup) return;

  if (progress_was_stalled) 
    {
      progress_dialog->setLabelText(progress_info);
      progress_was_stalled=false;
      application->processEvents();
    }
      
  // We might be called lots of times with the same step.  Don't know if Qt handles this efficiently so check for it ourselves.
  if (step!=last_step)
    {
      progress_dialog->setProgress(step);
      last_step=step;
      application->processEvents();
    }

  // TODO: Probably better to base call to processEvents() on time since we last called it.
  // (but certainly calling it every step is a bad idea: really slows app down)
}

void FracplanetMain::progress_complete(const std::string& info)
{
  if (startup) return;

  progress_dialog->setLabelText(info.c_str());

  last_step=static_cast<uint>(-1);

  QApplication::restoreOverrideCursor();

  application->processEvents();
}

void FracplanetMain::regenerate()   //! \todo Should be able to retain ground or clouds
{
  viewer->hide();

  meshes.clear();
  viewer->set_mesh(meshes);

  mesh_terrain.reset();
  mesh_cloud.reset();

  const clock_t t0=clock();

  // There are some issues with type here:
  // We need to keep hold of a pointer to TriangleMeshTerrain so we can call its write_povray method
  // but the triangle viewer needs the TriangleMesh.
  // So we end up with code like this to avoid a premature downcast.
  switch (parameters_terrain.object_type)
    {
    case ParametersObject::ObjectTypePlanet:
      {
	std::auto_ptr<TriangleMeshTerrainPlanet> it(new TriangleMeshTerrainPlanet(parameters_terrain,this));
	meshes.push_back(it.get());
	mesh_terrain.reset(it.release());
	break;
      }
    default:
      {
	std::auto_ptr<TriangleMeshTerrainFlat> it(new TriangleMeshTerrainFlat(parameters_terrain,this));
	meshes.push_back(it.get());
	mesh_terrain.reset(it.release());
	break;
      }
    }

  if (parameters_cloud.enabled)
    {
      switch (parameters_cloud.object_type)
	{
	case ParametersObject::ObjectTypePlanet:
	  {
	    std::auto_ptr<TriangleMeshCloudPlanet> it(new TriangleMeshCloudPlanet(parameters_cloud,this));
	    meshes.push_back(it.get());
	    mesh_cloud.reset(it.release());
	    break;
	  }
	default:
	  {
	    std::auto_ptr<TriangleMeshCloudFlat> it(new TriangleMeshCloudFlat(parameters_cloud,this));
	    meshes.push_back(it.get());
	    mesh_cloud.reset(it.release());
	    break;
	  }
	}
    }

  const clock_t t1=clock();
  
  progress_dialog.reset(0);

  std::cerr << "Mesh build time was " << (t1-t0)/static_cast<double>(CLOCKS_PER_SEC) << "s" << std::endl;

  viewer->set_mesh(meshes);
  viewer->showNormal();
  viewer->raise();
}

void FracplanetMain::save_pov()
{
  QString selected_filename=QFileDialog::getSaveFileName(".","POV-Ray (*.pov *.inc)",this,"Save object","Fracplanet: a .pov AND .inc file will be written");
  if (selected_filename.isEmpty())
    {
      QMessageBox::critical(this,"Fracplanet","No file specified\nNothing saved");
    }
  else if (!(selected_filename.upper().endsWith(".POV") || selected_filename.upper().endsWith(".INC")))
    {
      QMessageBox::critical(this,"Fracplanet","File selected must have .pov or .inc suffix.");
    }
  else
    {
      viewer->hide();
      
      const std::string filename_base(selected_filename.left(selected_filename.length()-4).local8Bit());
      const std::string filename_pov=filename_base+".pov";
      const std::string filename_inc=filename_base+".inc";
      
      const uint last_separator=filename_inc.rfind('/');
      const std::string filename_inc_relative_to_pov=
	"./"
	+(
	  last_separator==std::string::npos
	  ?
	  filename_inc
	  :
	  filename_inc.substr(last_separator+1)
	  );
      
      std::ofstream out_pov(filename_pov.c_str());
      std::ofstream out_inc(filename_inc.c_str());
      
      // Boilerplate for renderer    
      out_pov << "camera {perspective location <0,1,-4.5> look_at <0,0,0> angle 45}\n";
      out_pov << "light_source {<100,100,-100> color rgb <1.0,1.0,1.0>}\n";
      out_pov << "#include \""+filename_inc_relative_to_pov+"\"\n";
      
      mesh_terrain->write_povray(out_inc,parameters_save,parameters_terrain);
      if (mesh_cloud) mesh_cloud->write_povray(out_inc,parameters_save,parameters_cloud);
      
      out_pov.close();
      out_inc.close();
      
      const bool ok=(out_pov && out_inc);
      
      progress_dialog.reset(0);
      
      viewer->showNormal();
      viewer->raise();
      
      if (!ok) 
	{
	  QMessageBox::critical(this,"Fracplanet","Errors ocurred while the files were being written.");
	}
    }
}

void FracplanetMain::save_blender()
{
  QString selected_filename=QFileDialog::getSaveFileName(".","Blender (*.py)",this,"Save object","Fracplanet");
  if (selected_filename.isEmpty())
    {
      QMessageBox::critical(this,"Fracplanet","No file specified\nNothing saved");
    }
  else
    {
      viewer->hide();

      const std::string filename(selected_filename.local8Bit());
      std::ofstream out(filename.c_str());
      
      // Boilerplate
      out << "#!BPY\n\n";
      out << "import Blender\n";
      out << "from Blender import NMesh, Material\n";
      out << "\n";
      out << "def v(mesh,x,y,z):\n";
      out << "\tmesh.verts.append(NMesh.Vert(x,y,z))\n";
      out << "\n";
      out << "def f(mesh,material,v0,v1,v2,c0,c1,c2):\n";
      out << "\tface=NMesh.Face()\n";
      out << "\tface.transp=NMesh.FaceTranspModes.ALPHA\n";
      out << "\tface.smooth=1\n";
      out << "\tface.mat=material\n";
      out << "\tface.v.append(mesh.verts[v0])\n";
      out << "\tface.v.append(mesh.verts[v1])\n";
      out << "\tface.v.append(mesh.verts[v2])\n";
      out << "\tface.col.append(NMesh.Col(c0[0],c0[1],c0[2],c0[3]))\n";
      out << "\tface.col.append(NMesh.Col(c1[0],c1[1],c1[2],c1[3]))\n";
      out << "\tface.col.append(NMesh.Col(c2[0],c2[1],c2[2],c2[3]))\n";
      out << "\tmesh.faces.append(face)\n";
      out << "\n";

      mesh_terrain->write_blender(out,parameters_save,parameters_terrain,"fracplanet");
      if (mesh_cloud) mesh_cloud->write_blender(out,parameters_save,parameters_cloud,"fracplanet");
	  
      out << "Blender.Redraw()\n";

      out.close();

      progress_dialog.reset(0);
      
      viewer->showNormal();
      viewer->raise();
	  
      if (!out) 
	{
	  QMessageBox::critical(this,"Fracplanet","Errors ocurred while the files were being written.");
	}
    }
}

void FracplanetMain::save_texture()
{
  const uint height=parameters_save.texture_height;
  const uint width=height*mesh_terrain->geometry().scan_convert_image_aspect_ratio();

  QString selected_filename=QFileDialog::getSaveFileName(".","Texture (*.ppm)",this,"Save texture","Fracplanet");
  if (selected_filename.isEmpty())
    {
      QMessageBox::critical(this,"Fracplanet","No file specified\nNothing saved");
    }
  else if (!(selected_filename.upper().endsWith(".PPM")))
    {
      QMessageBox::critical(this,"Fracplanet","File selected must have .ppm suffix.");
    }
  else
    {
      const std::string filename(selected_filename.local8Bit());
      const std::string filename_base(selected_filename.left(selected_filename.length()-4).local8Bit());
      	  
      viewer->hide();

      bool ok=true;
      {
	boost::scoped_ptr<Image<ByteRGBA> > terrain_image(new Image<ByteRGBA>(width,height));
	terrain_image->fill(ByteRGBA(0,0,0,0));
	boost::scoped_ptr<Image<ushort> > terrain_dem(new Image<ushort>(width,height));
	terrain_dem->fill(0);

	boost::scoped_ptr<Image<ByteRGBA> > terrain_normals(new Image<ByteRGBA>(width,height));
	terrain_normals->fill(ByteRGBA(128,128,128,0));

	mesh_terrain->render_texture
	  (
	   *terrain_image,
	   terrain_dem.get(),
	   terrain_normals.get(),
	   parameters_save.texture_shaded,
	   parameters_render.ambient,
	   parameters_render.illumination_direction()
	   );

	if (!terrain_image->write_ppmfile(filename,this)) ok=false;
	
	if (ok && terrain_dem)
	  {
	    if (!terrain_dem->write_pgmfile(filename_base+"_dem.pgm",this)) ok=false;
	  }

	if (ok && terrain_normals)
	  {
	    if (!terrain_normals->write_ppmfile(filename_base+"_norm.ppm",this)) ok=false;
	  }
      }

      if (ok && mesh_cloud)
	{
	  QMessageBox::warning(this,"Fracplanet","Texture save of cloud mesh not currently supported");
	  //boost::scoped_ptr<Image<uchar> > cloud_image(new Image<uchar>(width,height));
	  //mesh_cloud->render_texture(*cloud_image);
	  //if (!cloud_image->write_pgmfile(filename_base+"_cloud.png",this)) ok=false;
	}

      progress_dialog.reset(0);
      
      viewer->showNormal();
      viewer->raise();
	  
      if (!ok) 
	{
	  QMessageBox::critical(this,"Fracplanet","Errors ocurred while the files were being written.");
	}
    }
}
