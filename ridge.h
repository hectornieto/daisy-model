// ridge.h
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


#ifndef RIDGE_H
#define RIDGE_H

#include <vector>

class Frame;
class FrameSubmodel;
class Log;
class Geometry1D;
class Soil;
class SoilWater;

class Ridge
{ 
  struct Implementation;
  Implementation& impl;

  // Simulation.
public:
  void tick (const Geometry1D& geo,
             const Soil&, const SoilWater&, 
	     double external_ponding /* [mm] */, double dt /* [h] */);
  void update_water (const Geometry1D& geo,
                     const Soil&, const std::vector<double>& S_,
		     std::vector<double>& h_, std::vector<double>& Theta_,
		     std::vector<double>& q, const std::vector<double>& q_p, 
                     const double dt);
  void output (Log&) const;
  int last_cell () const;
  double h () const;		// [cm]
  double exfiltration () const;	// [mm]

  // Create & Destroy.
public:
  static void load_syntax (Frame&);
  void initialize (const Geometry1D& geo,
                   const Soil&, const SoilWater&);
  Ridge (const FrameSubmodel& al);
  ~Ridge ();
};

#endif // RIDGE_H
