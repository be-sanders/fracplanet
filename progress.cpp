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
#include "progress.h"

Progress::~Progress()
{}

ProgressScope::ProgressScope(uint steps,const std::string& info,Progress* tgt)
  :_steps(steps)
   ,_info(info)
   ,_target(tgt)
   ,_step(0)
{
  if (_target) _target->progress_start(_steps,_info);
}

ProgressScope::~ProgressScope()
{
  if (_target) _target->progress_complete(_info+" completed");
}

void ProgressScope::step()
{
  _step++;
  if (_target) _target->progress_step(_step);
}
