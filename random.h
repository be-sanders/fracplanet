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

/*! \file
  \brief Interface for class Random and derived classes.
*/

#ifndef _random_h_
#define _random_h_

#include "useful.h"

//! Abstract base class for random number generation
class Random
{
protected:
  //! Current state of random number generator.
  uint current;
public:

  //! Constructor.  Argument is seed for random number generator.
  Random(uint s=0)
    :current(s)
    {}

  //! Destructor.
  virtual ~Random()
    {}
  
  //! Set seed value.
  void seed(uint n);

  //! Return a random number.
  /*! \warning Returns double instead of float because suspect NegExp can return Inf otherwise.
   */
  virtual double operator()()
    =0;
};

//! Generates random numbers in the range [0,1).
class Random01 : public Random   
{
protected:
  //!
  /* Code straight out of Stroustrup C++ 3rd ed.
   */
  uint next()
    {current=current*1103515245+12345;return (current&0x7fffffff);}

public:
  // Constructor.  Argument is seed value.
  Random01(uint s=0)
    :Random(s){}

  //! Destructor.
  virtual ~Random01()
    {}
  
  // Return random number in the range [0,1).
  virtual double operator()()
    {return ((1.0/2147483648.0)*next());}
};

//! Return negative-exponentially distributed random numbers.
/*! \todo Since RandomNegExp can't be considered IS-A Random01, maybe it shouldn't be derived from it.
 */
class RandomNegExp : public Random01
{
protected:
  double mean;
public:
  
  //! Construct generator of numbers with mean value m, seed value s.
  RandomNegExp(double m,uint s=0)
    :Random01(s),mean(m){}

  //! Destructor.
  virtual ~RandomNegExp()
    {}
  
  //! Return negative-ecponentially distributed random number.
  virtual double operator()()
    {return -mean*log(1.0-Random01::operator()());}  
};

#endif






