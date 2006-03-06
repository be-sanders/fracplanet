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

#include "control.h"

#include <qcolordialog.h>

Control::Control(QWidget* parent)
  :QVBox(parent)
{}

Control::~Control()
{}

void Control::pickColour(QPushButton* button,FloatRGBA& colour)
{
  const ByteRGBA col_old(colour);
  QColor qcol_old(col_old.r,col_old.g,col_old.b);
  QColor qcol_new=QColorDialog::getColor(qcol_old,this);
  if (qcol_new.isValid())
    {
      colour=FloatRGBA(ByteRGBA(qcol_new.red(),qcol_new.green(),qcol_new.blue(),255));
      
      QPixmap pmap(16,16);
      pmap.fill(qcol_new);
      button->setIconSet(QIconSet(pmap));
    }
}

/*! Used when initialising colour-chooser buttons.
 */
QIconSet Control::build_icon_of_colour(const FloatRGBA& col)
{
  QPixmap pmap(16,16);

  const ByteRGBA bcol(col);
  pmap.fill(QColor(bcol.r,bcol.g,bcol.b));
  return QIconSet(pmap);
}

