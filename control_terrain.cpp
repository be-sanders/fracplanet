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

#include <qtabwidget.h>
#include <qgrid.h>

#include "control_terrain.h"
#include "fracplanet_main.h"

/*! Lots of tedious code to instantiate controls and wire things up.
 */
ControlTerrain::ControlTerrain(QWidget* parent,FracplanetMain* tgt,ParametersTerrain* param_terrain,ParametersCloud* param_cloud)
  :Control(parent)
  ,parameters_terrain(param_terrain)
  ,parameters_cloud(param_cloud)
  ,regenerate_target(tgt)
{
  QTabWidget* tabs=new QTabWidget(this);      // Top level tab widget
  tabs->setMinimumWidth(512);

  QTabWidget* tab_terrain=new QTabWidget(this);  
  tabs->addTab(tab_terrain,"Terrain");

  //QVBox*const tab_terrain=new QVBox(tabs);
  //tabs->addTab(tab_terrain,"Landscape");

  QVBox*const tab_terrain_basics=new QVBox(tab_terrain);
  tab_terrain->addTab(tab_terrain_basics,"Basics");

  QVBox*const tab_terrain_subdivision=new QVBox(tab_terrain);
  tab_terrain->addTab(tab_terrain_subdivision,"Subdivision");

  QVBox*const tab_terrain_noise=new QVBox(tab_terrain);
  tab_terrain->addTab(tab_terrain_noise,"Noise");

  QVBox*const tab_snow=new QVBox(tabs);
  tabs->addTab(tab_snow,"Snow");

  QVBox*const tab_rivers=new QVBox(tabs);
  tabs->addTab(tab_rivers,"Rivers");

  QVBox*const tab_colours=new QVBox(tabs);
  tabs->addTab(tab_colours,"Colours");

  QVBox* tab_clouds=new QVBox(tabs);
  tabs->addTab(tab_clouds,"Clouds");

  QComboBox* object_type_combo_box=new QComboBox(false,tab_terrain_basics);
  object_type_combo_box->insertItem("Generate Planet",  Parameters::ObjectTypePlanet);
  object_type_combo_box->insertItem("Generate Hexagonal Area", Parameters::ObjectTypeFlatHexagon);
  object_type_combo_box->insertItem("Generate Triangular Area",Parameters::ObjectTypeFlatTriangle);
  object_type_combo_box->insertItem("Generate Square Area",  Parameters::ObjectTypeFlatSquare);

  object_type_combo_box->setCurrentItem(parameters_terrain->object_type);

  connect(
	  object_type_combo_box,SIGNAL(activated(int)),
	  this,SLOT(setObjectType(int))
	  );
  
  QGrid*const grid_terrain_basics=new QGrid(2,Qt::Horizontal,tab_terrain_basics);
  QGrid*const grid_terrain_subdivision=new QGrid(2,Qt::Horizontal,tab_terrain_subdivision);
  QGrid*const grid_terrain_noise=new QGrid(2,Qt::Horizontal,tab_terrain_noise);

  base_height_label=new QLabel("Base land height (%):",grid_terrain_basics);
  base_height_spinbox=new QSpinBox(-100,100,10,grid_terrain_basics);
  base_height_spinbox->setValue((uint)(100.0*parameters_terrain->base_height));
  connect(
	  base_height_spinbox,SIGNAL(valueChanged(int)),
	  this,SLOT(setBaseHeight(int))
	  );
  QToolTip::add(base_height_spinbox,"The initial height of land relative to sea-level");

  terrain_seed_label=new QLabel("Terrain random seed:",grid_terrain_basics);
  terrain_seed_spinbox=new QSpinBox(0xffffffff,0x7fffffff,1,grid_terrain_basics);
  terrain_seed_spinbox->setValue(parameters_terrain->seed);
  connect(
	  terrain_seed_spinbox,SIGNAL(valueChanged(int)),
	  this,SLOT(setTerrainSeed(int))
	  );
  QToolTip::add(terrain_seed_spinbox,"The random seed for subdivision and noise.");

  subdivisions_label=new QLabel("Subdivisions:",grid_terrain_subdivision);
  subdivisions_spinbox=new QSpinBox(0,16,1,grid_terrain_subdivision);
  subdivisions_spinbox->setValue(parameters_terrain->subdivisions);
  connect(
	  subdivisions_spinbox,SIGNAL(valueChanged(int)),
	  this,SLOT(setTerrainSubdivisions(int))
	  );
  QToolTip::add(subdivisions_spinbox,"The number of times the initial structure will be subdivided.\nWARNING: EACH STEP QUADRUPLES THE MEMORY REQUIREMENT!");

  subdivisions_unperturbed_label=new QLabel("Unperturbed subdivisions:",grid_terrain_subdivision);
  subdivisions_unperturbed_spinbox=new QSpinBox(0,16,1,grid_terrain_subdivision);
  subdivisions_unperturbed_spinbox->setValue(parameters_terrain->subdivisions_unperturbed);
  connect(
	  subdivisions_unperturbed_spinbox,SIGNAL(valueChanged(int)),
	  this,SLOT(setSubdivisionsUnperturbed(int))
	  );
  QToolTip::add(subdivisions_unperturbed_spinbox,"The number of subdivisions which will be performed without perturbing vertices");

  variation_vertical_label=new QLabel("Vertical perturbation :",grid_terrain_subdivision);
  variation_vertical_spinbox=new QSpinBox(0,50,1,grid_terrain_subdivision);
  variation_vertical_spinbox->setValue((int)(100*parameters_terrain->variation.z));
  connect(
	  variation_vertical_spinbox,SIGNAL(valueChanged(int)),
	  this,SLOT(setVariationVertical(int))
	  );
  QToolTip::add(variation_vertical_spinbox,"The magnitude of random height perturbations");

  variation_horizontal_label=new QLabel("Horizontal perturbation:",grid_terrain_subdivision);
  variation_horizontal_spinbox=new QSpinBox(0,25,1,grid_terrain_subdivision);
  variation_horizontal_spinbox->setValue((int)(100*parameters_terrain->variation.x));
  connect(
	  variation_horizontal_spinbox,SIGNAL(valueChanged(int)),
	  this,SLOT(setVariationHorizontal(int))
	  );
  QToolTip::add(variation_horizontal_spinbox,"The magnitude of random horizontal perturbations");

  noise_terms_label=new QLabel("Noise terms",grid_terrain_noise);
  noise_terms_spinbox=new QSpinBox(0,10,1,grid_terrain_noise);
  noise_terms_spinbox->setValue(parameters_terrain->noise_terms);
  connect(
	  noise_terms_spinbox,SIGNAL(valueChanged(int)),
	  this,SLOT(setNoiseTerms(int))
	  );
  QToolTip::add(noise_terms_spinbox,"Number of terms in added Perlin noise");

  noise_frequency_label=new QLabel("Noise frequency",grid_terrain_noise);
  noise_frequency_spinbox=new QSpinBox(0,10000,10,grid_terrain_noise);
  noise_frequency_spinbox->setValue(static_cast<int>(100*parameters_terrain->noise_frequency));
  connect(
	  noise_frequency_spinbox,SIGNAL(valueChanged(int)),
	  this,SLOT(setNoiseFrequency(int))
	  );
  QToolTip::add(noise_frequency_spinbox,"Frequency for Perlin noise 1st term");

  noise_amplitude_label=new QLabel("Noise amplitude",grid_terrain_noise);
  noise_amplitude_spinbox=new QSpinBox(0,100,1,grid_terrain_noise);
  noise_amplitude_spinbox->setValue(static_cast<int>(100*parameters_terrain->noise_amplitude));  
  connect(
	  noise_amplitude_spinbox,SIGNAL(valueChanged(int)),
	  this,SLOT(setNoiseAmplitude(int))
	  );
  QToolTip::add(noise_amplitude_spinbox,"Amplitude for Perlin noise 1st term");

  noise_amplitude_decay_label=new QLabel("Noise amplitude decay rate",grid_terrain_noise);
  noise_amplitude_decay_spinbox=new QSpinBox(0,100,10,grid_terrain_noise);
  noise_amplitude_decay_spinbox->setValue(static_cast<int>(100*parameters_terrain->noise_amplitude_decay));  
  connect(
	  noise_amplitude_decay_spinbox,SIGNAL(valueChanged(int)),
	  this,SLOT(setNoiseAmplitudeDecay(int))
	  );
  QToolTip::add(noise_amplitude_decay_spinbox,"Amplitude decay rate for subsequent Perlin noise terms\n(subsequent terms are this percentage amplitude of the previous term)");

  power_law_label=new QLabel("Power law:",grid_terrain_basics);
  power_law_spinbox=new QSpinBox(1,10000,10,grid_terrain_basics);
  power_law_spinbox->setValue((int)(100*parameters_terrain->power_law));
  connect(
	  power_law_spinbox,SIGNAL(valueChanged(int)),
	  this,SLOT(setPowerLaw(int))
	  );
  QToolTip::add(power_law_spinbox,"The power-law to be applied to elevations.");

  QGrid*const grid_snow=new QGrid(2,Qt::Horizontal,tab_snow);

  snowline_equator_label=new QLabel("Snowline at equator",grid_snow);
  snowline_equator_spinbox=new QSpinBox(-100,200,10,grid_snow);
  snowline_equator_spinbox->setValue((int)(100*parameters_terrain->snowline_equator));
  connect(
	  snowline_equator_spinbox,SIGNAL(valueChanged(int)),
	  this,SLOT(setSnowlineEquator(int))
	  );
  QToolTip::add(snowline_equator_spinbox,"Snowline on the equator (as a % of the maximum height)");

  snowline_pole_label=new QLabel("Snowline at pole",grid_snow);
  snowline_pole_spinbox=new QSpinBox(-100,200,10,grid_snow);
  snowline_pole_spinbox->setValue((int)(100*parameters_terrain->snowline_pole));
  connect(
	  snowline_pole_spinbox,SIGNAL(valueChanged(int)),
	  this,SLOT(setSnowlinePole(int))
	  );
  QToolTip::add(snowline_pole_spinbox,"Snowline at the poles (as a % of the maximum height)");

  snowline_power_law_label=new QLabel("Snowline power law",grid_snow);
  snowline_power_law_spinbox=new QSpinBox(1,1000,10,grid_snow);
  snowline_power_law_spinbox->setValue((int)(100*parameters_terrain->snowline_power_law));
  connect(
	  snowline_power_law_spinbox,SIGNAL(valueChanged(int)),
	  this,SLOT(setSnowlinePowerLaw(int))
	  );
  QToolTip::add(snowline_power_law_spinbox,"Power law applied to snowline elevation.");

  snowline_slope_effect_label=new QLabel("Snowline slope suppression",grid_snow);
  snowline_slope_effect_spinbox=new QSpinBox(0,10000,5,grid_snow);
  snowline_slope_effect_spinbox->setValue((int)(100*parameters_terrain->snowline_slope_effect));
  connect(
	  snowline_slope_effect_spinbox,SIGNAL(valueChanged(int)),
	  this,SLOT(setSnowlineSlopeEffect(int))
	  );
  QToolTip::add(snowline_slope_effect_spinbox,"Snow suppression on slopes.");

  snowline_glacier_effect_label=new QLabel("Snowline glacier effect",grid_snow);
  snowline_glacier_effect_spinbox=new QSpinBox(-1000,1000,5,grid_snow);
  snowline_glacier_effect_spinbox->setValue((int)(100*parameters_terrain->snowline_glacier_effect));
  connect(
	  snowline_glacier_effect_spinbox,SIGNAL(valueChanged(int)),
	  this,SLOT(setSnowlineGlacierEffect(int))
	  );
  QToolTip::add(snowline_glacier_effect_spinbox,"Converts rivers to glaciers.");

  QGrid*const grid_rivers=new QGrid(2,Qt::Horizontal,tab_rivers);

  rivers_label=new QLabel("Rivers:",grid_rivers);
  rivers_spinbox=new QSpinBox(0,1000000,100,grid_rivers);
  rivers_spinbox->setValue(parameters_terrain->rivers);
  connect(
	  rivers_spinbox,SIGNAL(valueChanged(int)),
	  this,SLOT(setRivers(int))
	  );
  QToolTip::add(rivers_spinbox,"The number of rivers to be attempted to be generated");

  rivers_seed_label=new QLabel("Rivers seed:",grid_rivers);
  rivers_seed_spinbox=new QSpinBox(0xffffffff,0x7fffffff,1,grid_rivers);
  rivers_seed_spinbox->setValue(parameters_terrain->rivers_seed);
  connect(
	  rivers_seed_spinbox,SIGNAL(valueChanged(int)),
	  this,SLOT(setRiversSeed(int))
	  );
  QToolTip::add(rivers_seed_spinbox,"The random seed for river generation.");

  lake_becomes_sea_label=new QLabel("Lake becomes sea:",grid_rivers);
  lake_becomes_sea_spinbox=new QSpinBox(1,100,1,grid_rivers);
  lake_becomes_sea_spinbox->setValue((uint)(100.0*parameters_terrain->lake_becomes_sea));
  connect(
	  lake_becomes_sea_spinbox,SIGNAL(valueChanged(int)),
	  this,SLOT(setLakeBecomesSea(int))
	  );
  QToolTip::add(lake_becomes_sea_spinbox,"The percentage of planetary surface which must be covered by a lake for it to be considered a sea");

  QGrid*const grid_colours=new QGrid(2,Qt::Horizontal,tab_colours);
  
  colour_ocean_button=new QPushButton(build_icon_of_colour(parameters_terrain->colour_ocean),"Ocean",grid_colours);
  connect(
	  colour_ocean_button,SIGNAL(clicked()),
	  this,SLOT(pickColourOcean())
	  );
  colour_shoreline_button=new QPushButton(build_icon_of_colour(parameters_terrain->colour_shoreline),"Shore",grid_colours);
  connect(
	  colour_shoreline_button,SIGNAL(clicked()),
	  this,SLOT(pickColourShoreline())
	  );
  colour_river_button=new QPushButton(build_icon_of_colour(parameters_terrain->colour_river),"River",grid_colours);
  connect(
	  colour_river_button,SIGNAL(clicked()),
	  this,SLOT(pickColourRiver())
	  );
  colour_low_button=new QPushButton(build_icon_of_colour(parameters_terrain->colour_low),"Low",grid_colours);
  connect(
	  colour_low_button,SIGNAL(clicked()),
	  this,SLOT(pickColourLow())
	  );
  colour_snow_button=new QPushButton(build_icon_of_colour(parameters_terrain->colour_snow),"Snow",grid_colours);
  connect(
	  colour_snow_button,SIGNAL(clicked()),
	  this,SLOT(pickColourSnow())
	  );
  colour_high_button=new QPushButton(build_icon_of_colour(parameters_terrain->colour_high),"High",grid_colours);
  connect(
	  colour_high_button,SIGNAL(clicked()),
	  this,SLOT(pickColourHigh())
	  );

  new QLabel("Oceans & rivers emissive",grid_colours);
  oceans_and_rivers_emissive_spinbox=new QSpinBox(0,100,10,grid_colours);
  oceans_and_rivers_emissive_spinbox->setValue((uint)(100.0*parameters_terrain->oceans_and_rivers_emissive));
  connect(
	  oceans_and_rivers_emissive_spinbox,SIGNAL(valueChanged(int)),
	  this,SLOT(setOceansAndRiversEmissive(int))
	  );
  QToolTip::add(oceans_and_rivers_emissive_spinbox,"Percentage of ocean and river colour which is emissive");

  QCheckBox*const clouds_checkbox=new QCheckBox("Clouds enabled",tab_clouds);
  QToolTip::add(clouds_checkbox,"Switch on cloud generation");
  clouds_checkbox->setChecked(parameters_cloud->enabled);
  connect(
	  clouds_checkbox,SIGNAL(toggled(bool)),
	  this,SLOT(setCloudsEnabled(bool))
	  );

  QGrid*const grid_clouds=new QGrid(2,Qt::Horizontal,tab_clouds);

  clouds_subdivisions_unlock_checkbox=new QCheckBox("Subdivisions ",grid_clouds);
  clouds_subdivisions_unlock_checkbox->setChecked(false);
  QToolTip::add(clouds_subdivisions_unlock_checkbox,"Enable explicit control of cloud subdivisons.\nWhen disabled, the cloud mesh will be subdivided by the same amount as the terrain");
  connect(clouds_subdivisions_unlock_checkbox,SIGNAL(toggled(bool)),
	  this,SLOT(setCloudsSubdivisionsUnlocked(bool))
	  );

  clouds_subdivisions_spinbox=new QSpinBox(0,16,1,grid_clouds);
  clouds_subdivisions_spinbox->setValue(parameters_cloud->subdivisions);
  connect(
	  clouds_subdivisions_spinbox,SIGNAL(valueChanged(int)),
	  this,SLOT(setCloudSubdivisions(int))
	  );
  QToolTip::add(clouds_subdivisions_spinbox,"The number of times the initial structure will be subdivided.\nWARNING: EACH STEP QUADRUPLES THE MEMORY REQUIREMENT!");

  clouds_subdivisions_spinbox->setEnabled(clouds_subdivisions_unlock_checkbox->isChecked());
  connect(
	  clouds_subdivisions_unlock_checkbox,SIGNAL(toggled(bool)),
	  clouds_subdivisions_spinbox,SLOT(setEnabled(bool))
	  );
  setCloudsSubdivisionsUnlocked(clouds_subdivisions_unlock_checkbox->isChecked());

  clouds_seed_label=new QLabel("Clouds seed:",grid_clouds);
  clouds_seed_spinbox=new QSpinBox(0xffffffff,0x7fffffff,1,grid_clouds);
  clouds_seed_spinbox->setValue(parameters_cloud->seed);
  QToolTip::add(clouds_seed_spinbox,"Random seed for cloud generation");

  new QLabel("Cloud height",grid_clouds);
  QSpinBox* clouds_height_spinbox=new QSpinBox(1,100,1,grid_clouds);
  clouds_height_spinbox->setValue(static_cast<int>(parameters_cloud->cloudbase*100.0f));
  QToolTip::add(clouds_height_spinbox,"Altitude of cloud layer");
  connect(
	  clouds_height_spinbox,SIGNAL(valueChanged(int)),
	  this,SLOT(setCloudbase(int))
	  );

  colour_cloud_button=new QPushButton(build_icon_of_colour(parameters_cloud->colour),"Cloud colour",tab_clouds);
  connect(
	  colour_cloud_button,SIGNAL(clicked()),
	  this,SLOT(pickColourCloud())
	  );

  // Wiring to disable all cloud controls 
  grid_clouds->setEnabled(parameters_cloud->enabled);
  connect(
	  clouds_checkbox,SIGNAL(toggled(bool)),
	  grid_clouds,SLOT(setEnabled(bool))
	  );
  colour_cloud_button->setEnabled(parameters_cloud->enabled);
  connect(
	  clouds_checkbox,SIGNAL(toggled(bool)),
	  colour_cloud_button,SLOT(setEnabled(bool))
	  );
  
  setStretchFactor(new QVBox(this),1);
 
  regenerate_button=new QPushButton("Regenerate",this);
  connect(
	  regenerate_button,SIGNAL(clicked()),
	  regenerate_target,SLOT(regenerate())
	  );

  regenerate_with_new_terrain_seed_button=new QPushButton("...with new terrain seed",this);
  connect(
	  regenerate_with_new_terrain_seed_button,SIGNAL(clicked()),
	  this,SLOT(regenerate_with_new_terrain_seed())
	  );

  regenerate_with_new_rivers_seed_button=new QPushButton("...with new rivers seed",this);
  connect(
	  regenerate_with_new_rivers_seed_button,SIGNAL(clicked()),
	  this,SLOT(regenerate_with_new_rivers_seed())
	  );

  regenerate_with_new_clouds_seed_button=new QPushButton("...with new clouds seed",this);
  connect(
	  regenerate_with_new_clouds_seed_button,SIGNAL(clicked()),
	  this,SLOT(regenerate_with_new_clouds_seed())
	  );

  regenerate_with_new_clouds_seed_button->setEnabled(parameters_cloud->enabled);
  connect(
	  clouds_checkbox,SIGNAL(toggled(bool)),
	  regenerate_with_new_clouds_seed_button,SLOT(setEnabled(bool))
	  );

  setStretchFactor(new QVBox(this),1);
}

void ControlTerrain::regenerate_with_new_terrain_seed()
{
  parameters_terrain->seed++;
  terrain_seed_spinbox->setValue(parameters_terrain->seed);

  regenerate_target->regenerate();
}

void ControlTerrain::regenerate_with_new_rivers_seed()
{
  parameters_terrain->rivers_seed++;
  rivers_seed_spinbox->setValue(parameters_terrain->rivers_seed);

  regenerate_target->regenerate();
}

void ControlTerrain::regenerate_with_new_clouds_seed()
{
  parameters_cloud->seed++;
  clouds_seed_spinbox->setValue(parameters_cloud->seed);

  regenerate_target->regenerate();
}

