/***********************************************************************
 * The rDock program was developed from 1998 - 2006 by the software team
 * at RiboTargets (subsequently Vernalis (R&D) Ltd).
 * In 2006, the software was licensed to the University of York for
 * maintenance and distribution.
 * In 2012, Vernalis and the University of York agreed to release the
 * program as Open Source software.
 * This version is licensed under GNU-LGPL version 3.0 with support from
 * the University of Barcelona.
 * http://rdock.sourceforge.net/
 ***********************************************************************/

// GPFFCrossDock Class. GPFFCrossDock represents a fitness function
// valid for the CGP. It is used to train with the CrossDock set.

#ifndef _RBT_GPFFCROSSDOCK_H_
#define _RBT_GPFFCROSSDOCK_H_

#include "RbtGPFitnessFunction.h"
#include "RbtGPGenome.h"
#include "RbtGPTypes.h"
#include "RbtRand.h"
// #include <cmath>

class RbtGPFFCrossDock : public RbtGPFitnessFunction {
public:
  static std::string _CT;

  void ReadTables(std::istream &, RbtReturnTypeArray &, RbtReturnTypeArray &);
  double CalculateFitness(RbtGPGenomePtr, RbtReturnTypeArray &,
                          RbtReturnTypeArray &, double, bool);
  double CalculateFitness(RbtGPGenomePtr, RbtReturnTypeArray &,
                          RbtReturnTypeArray &, bool);

private:
  void CreateRandomCtes(int);
  std::vector<double> ctes;
};

// Useful typedefs
typedef SmartPtr<RbtGPFFCrossDock> RbtGPFFCrossDockPtr; // Smart pointer
typedef std::vector<RbtGPFFCrossDockPtr>
    RbtGPFFCrossDockList; // Vector of smart pointers
typedef RbtGPFFCrossDockList::iterator RbtGPFFCrossDockListIter;
typedef RbtGPFFCrossDockList::const_iterator RbtGPFFCrossDockListConstIter;
#endif //_RbtGPFFCrossDock
