// soil_water1d.h
// 
// Copyright 1996-2001 Per Abrahamsen and S�ren Hansen
// Copyright 2000-2001 KVL.
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


#ifndef SOIL_WATER1D_H
#define SOIL_WATER1D_H

#include "soil_water.h"
#include "macro.h"		// Must be initialized.

class Geometry1D;

class SoilWater1D : public SoilWater
{
  // Content.
  struct Implementation;
  std::auto_ptr<Implementation> impl;

  // Sink.
public:
  void clear (const Geometry&);
  void root_uptake (const std::vector<double>&);
  void drain (const std::vector<double>&);
  void freeze (const Soil&, const std::vector<double>&);
  
  // Queries
public:
  double h (size_t i) const;
  double pF (size_t i) const;
  double Theta (size_t i) const;
  double Theta_left (size_t i) const;
  double Theta_old (size_t i) const;
  double content (const Geometry&, double from, double to) const; // [cm]
#ifndef NEWMOVE
  double q (size_t i) const;
  double q_p (size_t i) const;
#endif // OLDMOVE
  double S_sum (size_t i) const;
  double S_root (size_t i) const;
  double S_drain (size_t i) const;
  double S_ice (size_t i) const;
  double S_p (size_t i) const;
  double h_ice (size_t i) const;
  double X_ice (size_t i) const;
  double X_ice_total (size_t i) const;

  size_t first_groundwater_node () const;
    
  // Ice modified lookups.
  double Theta (const Soil&, size_t i, double h) const;
 
  // Simulation.
public:
  void macro_tick (const Geometry1D&, const Soil&, Surface&, Treelog&);
  void tick (const Geometry1D& geo,
             const Soil&, const SoilHeat&, Surface&, Groundwater&, Treelog&);
  void set_external_source (const Geometry&, 
			    double amount, double from, double to);
  void incorporate (const Geometry&, double amount, double from, double to);
  void mix (const Geometry& geo,
            const Soil&, double from, double to);
  void swap (Treelog&, const Geometry& geo,
             const Soil&, double from, double middle, double to);
  void set_Theta (const Soil& soil, 
		  size_t from, size_t to, double Theta);
  bool check (size_t n, Treelog& err) const;
  void output (Log&) const;


  // Communication with surface.
  double MaxExfiltration (const Geometry& geo,
                          const Soil&, double T) const;

  // Communication with external model.
  void put_h (const Soil& soil, const std::vector<double>& v); // [cm]
  void get_sink (std::vector<double>& v) const; // [cm^3/cm^3/h]

  // Creation.
  static void load_syntax (Syntax&, AttributeList&);
  SoilWater1D (Block&);
  void initialize (const AttributeList&, 
		   const Geometry1D& geo,
                   const Soil& soil, const Groundwater& groundwater,
		   Treelog&);
  ~SoilWater1D ();
};

#endif // SOIL_WATER1D_H