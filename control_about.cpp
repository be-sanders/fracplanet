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
#include "control_about.h"
#include "license.h"

ControlAbout::ControlAbout(QWidget* parent)
  :QVBox(parent)
{
  label=new QLabel("\nFracplanet 0.0.1\n\nAuthor\ntimday@timday.com\n\nHome page\nhttp://fracplanet.sourceforge.net\n\nProject page\nhttp://sourceforge.net/projects/fracplanet\n\nLicense:",this);
  //label=new QLabel("Fracplanet 0.0.1",this);
  label->setAlignment(Qt::AlignHCenter|label->alignment());

  license=new QTextEdit(this);
  license->setReadOnly(true);
  license->setTextFormat(PlainText);
  license->setText(license_string);
}
