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
#include "control_about.h"
#include "license.h"
#include "dialog_documentation.h"

#include <qlabel.h>
#include <qtextedit.h>
#include <qapplication.h>
#include <qfont.h>

ControlAbout::ControlAbout(QWidget* parent)
  :QVBox(parent)
{
  setSpacing(10);

  QLabel* label0=new QLabel("\nFracplanet - version "+QString(FRACPLANET_VERSION),this);
  label0->setAlignment(Qt::AlignHCenter|label0->alignment());
  QFont label0_font(QApplication::font());
  label0_font.setBold(true);
  label0->setFont(label0_font);

  QLabel* label1=new QLabel("by timday@timday.com\nhttp://fracplanet.sourceforge.net",this);
  label1->setAlignment(Qt::AlignHCenter|label0->alignment());
  QFont label1_font(QApplication::font());
  label1_font.setPointSize(std::max(2,label1_font.pointSize()-4));
  label1->setFont(label1_font);

  DialogDocumentation* dialog_docs=new DialogDocumentation(this);

  QPushButton* button_docs=new QPushButton("Show documentation", this);
  connect(button_docs,SIGNAL(clicked()),dialog_docs,SLOT(show()));

  QLabel* label2=new QLabel("License:",this);
  label2->setAlignment(Qt::AlignHCenter|label0->alignment());

  QTextEdit* license;
  license=new QTextEdit(this);
  license->setReadOnly(true);
  license->setTextFormat(PlainText);
  license->setText(license_string);
}
