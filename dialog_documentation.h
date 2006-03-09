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
  \brief Interface for class DialogDocumentation.
*/

#ifndef _dialog_documentation_h_
#define _dialog_documentation_h_

#include <qdialog.h>
#include <qvbox.h>
#include <qtextbrowser.h>
#include <qpushbutton.h>

#include "useful.h"

//! Provides a dialog box with some user documentation.
class DialogDocumentation : public QDialog
{
 private:
  Q_OBJECT

 public:
  //! Constructor.
  DialogDocumentation(QWidget* parent);

  //! Destructor.
  virtual ~DialogDocumentation();

 protected:
  //! Vertical layout.
  QVBox* vbox;

  //! Text area.
  QTextBrowser* browser;

  //! Button to close dialog.
  QPushButton* ok;

  //! Need to pass resizes on to vbox or things just get chopped.
  virtual void resizeEvent(QResizeEvent*);
};

#endif
