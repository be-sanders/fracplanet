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
#ifndef _control_about_h_
#define _control_about_h_

#include <qhbox.h>
#include <qvbox.h>
#include <qlabel.h>
#include <qtextedit.h>

#include "useful.h"

//! Displays "About" type info
class ControlAbout : public QVBox
{
 private:
  Q_OBJECT;

 protected:
  QLabel* label;
  QTextEdit* license;

 public:
  ControlAbout(QWidget* parent);
};

#endif
