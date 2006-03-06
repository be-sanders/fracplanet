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

/*! \file
  \brief Interface for class Control.
*/

#ifndef _control_h_
#define _control_h_

#include <qiconset.h>
#include <qpushbutton.h>
#include <qvbox.h>

#include "useful.h"
#include "rgb.h"

//! Base class for other controls; useful for shared stuff.
class Control : public QVBox
{
 public:

  Control(QWidget* parent);
  virtual ~Control();

  //! Use Qt's colour-picking dialog to replace the referenced colour
  void pickColour(QPushButton* button,FloatRGBA& colour);

 protected:

  //! Utility function to build a small Qt icon of the specified colour.
  static QIconSet build_icon_of_colour(const FloatRGBA& col);
};

#endif
