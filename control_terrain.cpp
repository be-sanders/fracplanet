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
#include "control_terrain.h"

#include "fracplanet_main.h"

/*! Lots of tedious code to instantiate controls and wire things up.
 */
ControlTerrain::ControlTerrain(QWidget* parent,FracplanetMain* tgt,ParametersTerrain*const param)
  :QVBox(parent)
   ,parameters(param)
   ,regenerate_target(tgt)
{
  object_type_button_group=new QHButtonGroup(this);
  object_type_planet_button=new QRadioButton("Planet",object_type_button_group);
  object_type_terrain_button=new QRadioButton("Terrain",object_type_button_group);

  switch (parameters->object_type)
    {
    case ParametersTerrain::ObjectTypePlanet:
      object_type_planet_button->setChecked(true);
      break;
    case ParametersTerrain::ObjectTypeTerrain:
      object_type_terrain_button->setChecked(true);
      break;
    }

  connect(
	  object_type_button_group,SIGNAL(pressed(int)),
	  this,SLOT(setObjectType(int))
	  );
	  
  grid=new QGrid(2,Qt::Horizontal,this);

  subdivisions_seed_label=new QLabel("Perturbation seed:",grid);
  subdivisions_seed_spinbox=new QSpinBox(0xffffffff,0x7fffffff,1,grid);
  subdivisions_seed_spinbox->setValue(parameters->subdivisions_seed);
  connect(
	  subdivisions_seed_spinbox,SIGNAL(valueChanged(int)),
	  this,SLOT(setSubdivisionsSeed(int))
	  );
  QToolTip::add(subdivisions_seed_spinbox,"The random seed for vertex perturbations.");

  subdivisions_label=new QLabel("Subdivisions:",grid);
  subdivisions_spinbox=new QSpinBox(0,16,1,grid);
  subdivisions_spinbox->setValue(parameters->subdivisions);
  connect(
	  subdivisions_spinbox,SIGNAL(valueChanged(int)),
	  this,SLOT(setSubdivisions(int))
	  );
  QToolTip::add(subdivisions_spinbox,"The number of times the initial structure will be subdivided.  WARNING: INCREASE SLOWLY!");

  subdivisions_unperturbed_label=new QLabel("Unperturbed:",grid);
  subdivisions_unperturbed_spinbox=new QSpinBox(0,16,1,grid);
  subdivisions_unperturbed_spinbox->setValue(parameters->subdivisions_unperturbed);
  connect(
	  subdivisions_unperturbed_spinbox,SIGNAL(valueChanged(int)),
	  this,SLOT(setSubdivisionsUnperturbed(int))
	  );
  QToolTip::add(subdivisions_unperturbed_spinbox,"The number of subdivisions which will be performed without perturbing vertices");


  variation_vertical_label=new QLabel("Variation (vertical) (/1024):",grid);
  variation_vertical_spinbox=new QSpinBox(0,256,1,grid);
  variation_vertical_spinbox->setValue((int)(1024*parameters->variation.z));
  connect(
	  variation_vertical_spinbox,SIGNAL(valueChanged(int)),
	  this,SLOT(setVariationVertical(int))
	  );
  QToolTip::add(variation_vertical_spinbox,"The magnitude of random height perturbations");

  variation_horizontal_label=new QLabel("Variation (horizontal) (/1024):",grid);
  variation_horizontal_spinbox=new QSpinBox(0,256,1,grid);
  variation_horizontal_spinbox->setValue((int)(1024*parameters->variation.x));
  connect(
	  variation_horizontal_spinbox,SIGNAL(valueChanged(int)),
	  this,SLOT(setVariationHorizontal(int))
	  );
  QToolTip::add(variation_horizontal_spinbox,"The magnitude of random height perturbations");

  base_height_label=new QLabel("Base land height (%):",grid);
  base_height_spinbox=new QSpinBox(-100,100,10,grid);
  base_height_spinbox->setValue((uint)(100.0*parameters->base_height));
  connect(
	  base_height_spinbox,SIGNAL(valueChanged(int)),
	  this,SLOT(setBaseHeight(int))
	  );
  QToolTip::add(base_height_spinbox,"The initial height of land relative to sea-level");

  power_law_label=new QLabel("Power law:",grid);
  power_law_spinbox=new QSpinBox(1,10000,10,grid);
  power_law_spinbox->setValue((int)(100*parameters->power_law));
  connect(
	  power_law_spinbox,SIGNAL(valueChanged(int)),
	  this,SLOT(setPowerLaw(int))
	  );
  QToolTip::add(power_law_spinbox,"The power-law to be applied to elevations.");


  snowline_equator_label=new QLabel("Snowline at equator",grid);
  snowline_equator_spinbox=new QSpinBox(-100,200,10,grid);
  snowline_equator_spinbox->setValue((int)(100*parameters->snowline_equator));
  connect(
	  snowline_equator_spinbox,SIGNAL(valueChanged(int)),
	  this,SLOT(setSnowlineEquator(int))
	  );
  QToolTip::add(snowline_equator_spinbox,"Snowline on the equator (as a % of the maximum height)");

  snowline_pole_label=new QLabel("Snowline at pole",grid);
  snowline_pole_spinbox=new QSpinBox(-100,200,10,grid);
  snowline_pole_spinbox->setValue((int)(100*parameters->snowline_pole));
  connect(
	  snowline_pole_spinbox,SIGNAL(valueChanged(int)),
	  this,SLOT(setSnowlinePole(int))
	  );
  QToolTip::add(snowline_pole_spinbox,"Snowline at the poles (as a % of the maximum height)");

  snowline_power_law_label=new QLabel("Snowline power law",grid);
  snowline_power_law_spinbox=new QSpinBox(1,1000,10,grid);
  snowline_power_law_spinbox->setValue((int)(100*parameters->snowline_power_law));
  connect(
	  snowline_power_law_spinbox,SIGNAL(valueChanged(int)),
	  this,SLOT(setSnowlinePowerLaw(int))
	  );
  QToolTip::add(snowline_power_law_spinbox,"Power law applied to snowline elevation.");

  snowline_slope_effect_label=new QLabel("Snowline slope suppression",grid);
  snowline_slope_effect_spinbox=new QSpinBox(0,10000,5,grid);
  snowline_slope_effect_spinbox->setValue((int)(100*parameters->snowline_slope_effect));
  connect(
	  snowline_slope_effect_spinbox,SIGNAL(valueChanged(int)),
	  this,SLOT(setSnowlineSlopeEffect(int))
	  );
  QToolTip::add(snowline_slope_effect_spinbox,"Snow suppression on slopes.");

  snowline_glacier_effect_label=new QLabel("Snowline glacier effect",grid);
  snowline_glacier_effect_spinbox=new QSpinBox(-1000,1000,5,grid);
  snowline_glacier_effect_spinbox->setValue((int)(100*parameters->snowline_glacier_effect));
  connect(
	  snowline_glacier_effect_spinbox,SIGNAL(valueChanged(int)),
	  this,SLOT(setSnowlineGlacierEffect(int))
	  );
  QToolTip::add(snowline_glacier_effect_spinbox,"Converts rivers to glaciers.");

  rivers_label=new QLabel("Rivers:",grid);
  rivers_spinbox=new QSpinBox(0,1000000,100,grid);
  rivers_spinbox->setValue(parameters->rivers);
  connect(
	  rivers_spinbox,SIGNAL(valueChanged(int)),
	  this,SLOT(setRivers(int))
	  );
  QToolTip::add(rivers_spinbox,"The number of rivers to be attempted to be generated");

  rivers_seed_label=new QLabel("Rivers seed:",grid);
  rivers_seed_spinbox=new QSpinBox(0xffffffff,0x7fffffff,1,grid);
  rivers_seed_spinbox->setValue(parameters->rivers_seed);
  connect(
	  rivers_seed_spinbox,SIGNAL(valueChanged(int)),
	  this,SLOT(setRiversSeed(int))
	  );
  QToolTip::add(rivers_seed_spinbox,"The random seed for river generation.");

  lake_becomes_sea_label=new QLabel("Lake becomes sea:",grid);
  lake_becomes_sea_spinbox=new QSpinBox(1,100,1,grid);
  lake_becomes_sea_spinbox->setValue((uint)(100.0*parameters->lake_becomes_sea));
  connect(
	  lake_becomes_sea_spinbox,SIGNAL(valueChanged(int)),
	  this,SLOT(setLakeBecomesSea(int))
	  );
  QToolTip::add(lake_becomes_sea_spinbox,"The percentage of planetary surface which must be covered by a lake for it to be considered a sea");

  regenerate_button=new QPushButton("Regenerate",this);
  connect(
	  regenerate_button,SIGNAL(clicked()),
	  regenerate_target,SLOT(regenerate())
	  );

  regenerate_rivers_with_new_seed_button=new QPushButton("...with new rivers seed",this);
  connect(
	  regenerate_rivers_with_new_seed_button,SIGNAL(clicked()),
	  this,SLOT(regenerate_rivers_with_new_seed())
	  );

  regenerate_with_new_seed_button=new QPushButton("...with new perturbations seed",this);
  connect(
	  regenerate_with_new_seed_button,SIGNAL(clicked()),
	  this,SLOT(regenerate_with_new_seed())
	  );
	  
  padding=new QVBox(this);
  setStretchFactor(padding,1);
}

void ControlTerrain::regenerate_rivers_with_new_seed()
{
  parameters->rivers_seed++;
  rivers_seed_spinbox->setValue(parameters->rivers_seed);

  regenerate_target->regenerate();
}

void ControlTerrain::regenerate_with_new_seed()
{
  parameters->subdivisions_seed++;
  subdivisions_seed_spinbox->setValue(parameters->subdivisions_seed);

  regenerate_target->regenerate();
}
