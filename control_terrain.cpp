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
#include "control_terrain.h"

#include "fracplanet_main.h"
#include <qtabwidget.h>
#include <qgrid.h>

/*! Used when initialising colour-chooser buttons.
 */
QIconSet ControlTerrain::build_icon_of_colour(const FloatRGB& col)
{
  QPixmap pmap(16,16);

  const ByteRGB bcol(col);
  pmap.fill(QColor(bcol.r,bcol.g,bcol.b));
  return QIconSet(pmap);
}

/*! Lots of tedious code to instantiate controls and wire things up.
 */
ControlTerrain::ControlTerrain(QWidget* parent,FracplanetMain* tgt,ParametersTerrain* param)
  :QVBox(parent)
   ,parameters(param)
   ,regenerate_target(tgt)
{
  QTabWidget* tabs=new QTabWidget(this);
  tabs->setMinimumWidth(384);

  QVBox*const tab_terrain=new QVBox(tabs);
  tabs->addTab(tab_terrain,"Landscape");

  QVBox*const tab_snow=new QVBox(tabs);
  tabs->addTab(tab_snow,"Snow");

  QVBox*const tab_rivers=new QVBox(tabs);
  tabs->addTab(tab_rivers,"Rivers");

  QVBox*const tab_colours=new QVBox(tabs);
  tabs->addTab(tab_colours,"Colours");

  object_type_button_group=new QHButtonGroup(tab_terrain);
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
	  
  QGrid*const grid_terrain=new QGrid(2,Qt::Horizontal,tab_terrain);

  subdivisions_seed_label=new QLabel("Perturbation seed:",grid_terrain);
  subdivisions_seed_spinbox=new QSpinBox(0xffffffff,0x7fffffff,1,grid_terrain);
  subdivisions_seed_spinbox->setValue(parameters->subdivisions_seed);
  connect(
	  subdivisions_seed_spinbox,SIGNAL(valueChanged(int)),
	  this,SLOT(setSubdivisionsSeed(int))
	  );
  QToolTip::add(subdivisions_seed_spinbox,"The random seed for vertex perturbations.");

  subdivisions_label=new QLabel("Subdivisions:",grid_terrain);
  subdivisions_spinbox=new QSpinBox(0,16,1,grid_terrain);
  subdivisions_spinbox->setValue(parameters->subdivisions);
  connect(
	  subdivisions_spinbox,SIGNAL(valueChanged(int)),
	  this,SLOT(setSubdivisions(int))
	  );
  QToolTip::add(subdivisions_spinbox,"The number of times the initial structure will be subdivided.  WARNING: INCREASE SLOWLY!");

  subdivisions_unperturbed_label=new QLabel("Unperturbed:",grid_terrain);
  subdivisions_unperturbed_spinbox=new QSpinBox(0,16,1,grid_terrain);
  subdivisions_unperturbed_spinbox->setValue(parameters->subdivisions_unperturbed);
  connect(
	  subdivisions_unperturbed_spinbox,SIGNAL(valueChanged(int)),
	  this,SLOT(setSubdivisionsUnperturbed(int))
	  );
  QToolTip::add(subdivisions_unperturbed_spinbox,"The number of subdivisions which will be performed without perturbing vertices");


  variation_vertical_label=new QLabel("Variation (vertical) (/1024):",grid_terrain);
  variation_vertical_spinbox=new QSpinBox(0,256,1,grid_terrain);
  variation_vertical_spinbox->setValue((int)(1024*parameters->variation.z));
  connect(
	  variation_vertical_spinbox,SIGNAL(valueChanged(int)),
	  this,SLOT(setVariationVertical(int))
	  );
  QToolTip::add(variation_vertical_spinbox,"The magnitude of random height perturbations");

  variation_horizontal_label=new QLabel("Variation (horizontal) (/1024):",grid_terrain);
  variation_horizontal_spinbox=new QSpinBox(0,256,1,grid_terrain);
  variation_horizontal_spinbox->setValue((int)(1024*parameters->variation.x));
  connect(
	  variation_horizontal_spinbox,SIGNAL(valueChanged(int)),
	  this,SLOT(setVariationHorizontal(int))
	  );
  QToolTip::add(variation_horizontal_spinbox,"The magnitude of random horizontal perturbations");

  base_height_label=new QLabel("Base land height (%):",grid_terrain);
  base_height_spinbox=new QSpinBox(-100,100,10,grid_terrain);
  base_height_spinbox->setValue((uint)(100.0*parameters->base_height));
  connect(
	  base_height_spinbox,SIGNAL(valueChanged(int)),
	  this,SLOT(setBaseHeight(int))
	  );
  QToolTip::add(base_height_spinbox,"The initial height of land relative to sea-level");

  power_law_label=new QLabel("Power law:",grid_terrain);
  power_law_spinbox=new QSpinBox(1,10000,10,grid_terrain);
  power_law_spinbox->setValue((int)(100*parameters->power_law));
  connect(
	  power_law_spinbox,SIGNAL(valueChanged(int)),
	  this,SLOT(setPowerLaw(int))
	  );
  QToolTip::add(power_law_spinbox,"The power-law to be applied to elevations.");

  QGrid*const grid_snow=new QGrid(2,Qt::Horizontal,tab_snow);

  snowline_equator_label=new QLabel("Snowline at equator",grid_snow);
  snowline_equator_spinbox=new QSpinBox(-100,200,10,grid_snow);
  snowline_equator_spinbox->setValue((int)(100*parameters->snowline_equator));
  connect(
	  snowline_equator_spinbox,SIGNAL(valueChanged(int)),
	  this,SLOT(setSnowlineEquator(int))
	  );
  QToolTip::add(snowline_equator_spinbox,"Snowline on the equator (as a % of the maximum height)");

  snowline_pole_label=new QLabel("Snowline at pole",grid_snow);
  snowline_pole_spinbox=new QSpinBox(-100,200,10,grid_snow);
  snowline_pole_spinbox->setValue((int)(100*parameters->snowline_pole));
  connect(
	  snowline_pole_spinbox,SIGNAL(valueChanged(int)),
	  this,SLOT(setSnowlinePole(int))
	  );
  QToolTip::add(snowline_pole_spinbox,"Snowline at the poles (as a % of the maximum height)");

  snowline_power_law_label=new QLabel("Snowline power law",grid_snow);
  snowline_power_law_spinbox=new QSpinBox(1,1000,10,grid_snow);
  snowline_power_law_spinbox->setValue((int)(100*parameters->snowline_power_law));
  connect(
	  snowline_power_law_spinbox,SIGNAL(valueChanged(int)),
	  this,SLOT(setSnowlinePowerLaw(int))
	  );
  QToolTip::add(snowline_power_law_spinbox,"Power law applied to snowline elevation.");

  snowline_slope_effect_label=new QLabel("Snowline slope suppression",grid_snow);
  snowline_slope_effect_spinbox=new QSpinBox(0,10000,5,grid_snow);
  snowline_slope_effect_spinbox->setValue((int)(100*parameters->snowline_slope_effect));
  connect(
	  snowline_slope_effect_spinbox,SIGNAL(valueChanged(int)),
	  this,SLOT(setSnowlineSlopeEffect(int))
	  );
  QToolTip::add(snowline_slope_effect_spinbox,"Snow suppression on slopes.");

  snowline_glacier_effect_label=new QLabel("Snowline glacier effect",grid_snow);
  snowline_glacier_effect_spinbox=new QSpinBox(-1000,1000,5,grid_snow);
  snowline_glacier_effect_spinbox->setValue((int)(100*parameters->snowline_glacier_effect));
  connect(
	  snowline_glacier_effect_spinbox,SIGNAL(valueChanged(int)),
	  this,SLOT(setSnowlineGlacierEffect(int))
	  );
  QToolTip::add(snowline_glacier_effect_spinbox,"Converts rivers to glaciers.");

  QGrid*const grid_rivers=new QGrid(2,Qt::Horizontal,tab_rivers);

  rivers_label=new QLabel("Rivers:",grid_rivers);
  rivers_spinbox=new QSpinBox(0,1000000,100,grid_rivers);
  rivers_spinbox->setValue(parameters->rivers);
  connect(
	  rivers_spinbox,SIGNAL(valueChanged(int)),
	  this,SLOT(setRivers(int))
	  );
  QToolTip::add(rivers_spinbox,"The number of rivers to be attempted to be generated");

  rivers_seed_label=new QLabel("Rivers seed:",grid_rivers);
  rivers_seed_spinbox=new QSpinBox(0xffffffff,0x7fffffff,1,grid_rivers);
  rivers_seed_spinbox->setValue(parameters->rivers_seed);
  connect(
	  rivers_seed_spinbox,SIGNAL(valueChanged(int)),
	  this,SLOT(setRiversSeed(int))
	  );
  QToolTip::add(rivers_seed_spinbox,"The random seed for river generation.");

  lake_becomes_sea_label=new QLabel("Lake becomes sea:",grid_rivers);
  lake_becomes_sea_spinbox=new QSpinBox(1,100,1,grid_rivers);
  lake_becomes_sea_spinbox->setValue((uint)(100.0*parameters->lake_becomes_sea));
  connect(
	  lake_becomes_sea_spinbox,SIGNAL(valueChanged(int)),
	  this,SLOT(setLakeBecomesSea(int))
	  );
  QToolTip::add(lake_becomes_sea_spinbox,"The percentage of planetary surface which must be covered by a lake for it to be considered a sea");

  QGrid*const grid_colours=new QGrid(2,Qt::Horizontal,tab_colours);
  
  colour_ocean_button=new QPushButton(build_icon_of_colour(parameters->colour_ocean),"Ocean",grid_colours);
  connect(
	  colour_ocean_button,SIGNAL(clicked()),
	  this,SLOT(pickColourOcean())
	  );
  colour_shoreline_button=new QPushButton(build_icon_of_colour(parameters->colour_shoreline),"Shore",grid_colours);
  connect(
	  colour_shoreline_button,SIGNAL(clicked()),
	  this,SLOT(pickColourShoreline())
	  );
  colour_river_button=new QPushButton(build_icon_of_colour(parameters->colour_river),"River",grid_colours);
  connect(
	  colour_river_button,SIGNAL(clicked()),
	  this,SLOT(pickColourRiver())
	  );
  colour_low_button=new QPushButton(build_icon_of_colour(parameters->colour_low),"Low",grid_colours);
  connect(
	  colour_low_button,SIGNAL(clicked()),
	  this,SLOT(pickColourLow())
	  );
  colour_snow_button=new QPushButton(build_icon_of_colour(parameters->colour_snow),"Snow",grid_colours);
  connect(
	  colour_snow_button,SIGNAL(clicked()),
	  this,SLOT(pickColourSnow())
	  );
  colour_high_button=new QPushButton(build_icon_of_colour(parameters->colour_high),"High",grid_colours);
  connect(
	  colour_high_button,SIGNAL(clicked()),
	  this,SLOT(pickColourHigh())
	  );

  new QLabel("Oceans & rivers emissive",grid_colours);
  oceans_and_rivers_emissive_spinbox=new QSpinBox(0,100,10,grid_colours);
  oceans_and_rivers_emissive_spinbox->setValue((uint)(100.0*parameters->oceans_and_rivers_emissive));
  connect(
	  oceans_and_rivers_emissive_spinbox,SIGNAL(valueChanged(int)),
	  this,SLOT(setOceansAndRiversEmissive(int))
	  );
  QToolTip::add(oceans_and_rivers_emissive_spinbox,"Percentage of ocean and river colour which is emissive");
 
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
