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
/* Copyright (C) 1998,1999,2000 T Day */

/*! \file
  \brief All our favourite little helpers.
*/

#ifndef _useful_h_
#define _useful_h_

extern "C"
{
#include <stdlib.h>
#include <assert.h>
}

#include <math.h>
#include <iosfwd>
#include <string>
#include <memory>

typedef unsigned int uint;
typedef unsigned short ushort;
typedef unsigned char uchar;

template <class T> inline const T maximum(T a,T b) {return (a>b ? a : b);}
template <class T> inline const T minimum(T a,T b) {return (a<b ? a : b);}

template <class T> inline const T maximum(T a,T b,T c) {return maximum(a,maximum(b,c));}
template <class T> inline const T minimum(T a,T b,T c) {return minimum(a,minimum(b,c));}

template <class T> inline const T maximum(T a,T b,T c,T d) {return maximum(maximum(a,b),maximum(c,d));}
template <class T> inline const T minimum(T a,T b,T c,T d) {return minimum(minimum(a,b),minimum(c,d));}

template <class T> inline const T sqr(T a) {return a*a;}

template <class T> inline const T clamped(T v,T lo,T hi) {return (v<lo ? lo : (v>hi ? hi : v));}

template <class T> inline void clamp(T& v,T lo,T hi) {v=(v<lo ? lo : (v>hi ? hi : v));}

template <class T> inline void exchange(T& a,T& b) {const T x(a);a=b;b=x;}

extern void fatal_error(const char*);

inline void fatal_error(const std::string& s)
{
  fatal_error(s.c_str());
}

extern void fatal_internal_error(const char* src_file,uint src_line);

extern void constraint_violation(const char* test,const char* src_file,uint src_line);
#define constraint(TEST) {if (!TEST) {constraint_violation(#TEST,__FILE__,__LINE__);}}


#endif


