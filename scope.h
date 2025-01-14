// scope.h -- A name -> value map.
// 
// Copyright 2004 Per Abrahamsen and KVL.
//
// This file is part of Daisy.
// 
// Daisy is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser Public License as published by
// the Free Software Foundation; either version 2.1 of the License, or
// (at your option) any later version.
// 
// Daisy is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser Public License for more details.
// 
// You should have received a copy of the GNU Lesser Public License
// along with Daisy; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA


#ifndef SCOPE_H
#define SCOPE_H

#include "attribute.h"
#include "symbol.h"
#include <set>
#include <boost/noncopyable.hpp>

#ifdef __unix
#define EXPORT /* Nothing */
#elif defined (BUILD_DLL)
/* DLL export */
#define EXPORT __declspec(dllexport)
#else
/* EXE import */
#define EXPORT __declspec(dllimport)
#endif

class EXPORT Scope : private boost::noncopyable
{
  // For selecting with the 'scopesel' model.
public:
  virtual symbol title () const;

  // Type.
public:
  virtual void entries (std::set<symbol>&) const = 0;
  virtual Attribute::type lookup (symbol) const = 0;
  virtual bool can_extract_as (symbol, Attribute::type) const;
  virtual symbol dimension (symbol) const = 0;
  virtual symbol description (symbol) const = 0;
  virtual int type_size (symbol) const;

  // Value.
public:
  virtual bool check (symbol) const = 0;
  virtual double number (symbol) const = 0;
  virtual int value_size (symbol tag) const;
  virtual symbol name (symbol) const;
  virtual int integer (symbol) const;

  // Create and Destroy.
public:
  static Scope& null ();
protected:
  Scope ();
  virtual ~Scope ();
};

class WScope : public Scope     // Writable scope.
{
  // Use.
public:
  virtual void set (symbol, double) = 0;
};

#endif // SCOPE_H
