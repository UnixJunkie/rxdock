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

// Class for providing Charmm force field data

#ifndef _RBTCHARMMDATASOURCE_H_
#define _RBTCHARMMDATASOURCE_H_

#include "RbtAtom.h"
#include "RbtConfig.h"

// Added by DM, 8 Dec 1998 for HybridState lookup method
typedef std::map<std::string, RbtAtom::eHybridState> RbtStringHybridStateMap;
typedef RbtStringHybridStateMap::iterator RbtStringHybridStateMapIter;
typedef RbtStringHybridStateMap::const_iterator
    RbtStringHybridStateMapConstIter;

class RbtCharmmDataSource {
public:
  ////////////////////////////////////////
  // Constructors/destructors

  // DM 30 Apr 1999 - pass in masses.rtf file name as parameter
  RbtCharmmDataSource(const std::string &strMassesFile =
                          Rbt::GetRbtFileName("data", "masses.rtf"));

  ~RbtCharmmDataSource(); // Default destructor

  ////////////////////////////////////////
  // Public methods
  ////////////////
  std::string AtomTypeString(int nAtomType);
  int ImplicitHydrogens(const std::string &strFFType);
  int AtomicNumber(const std::string &strFFType);
  int FormalCharge(const std::string &strFFType); // DM 24 Mar 1999 - changed
                                                  // from double to int
  RbtAtom::eHybridState
  HybridState(const std::string &strFFType); // DM 8 Dec 1998

protected:
  ////////////////////////////////////////
  // Protected methods
  ///////////////////

private:
  ////////////////////////////////////////
  // Private methods
  /////////////////

  RbtCharmmDataSource(
      const RbtCharmmDataSource &); // Copy constructor disabled by default

  RbtCharmmDataSource &
  operator=(const RbtCharmmDataSource &); // Copy assignment disabled by default

  // DM 8 Dec 1998 Searches for Hybridisation state string in the masses.rtf
  // comment field Valid strings are (RBT::SP), (RBT::SP2), (RBT::SP3),
  // (RBT::AROM), (RBT::TRI) (brackets are important)
  RbtAtom::eHybridState
  ConvertCommentStringToHybridState(const std::string &strComment);

protected:
  ////////////////////////////////////////
  // Protected data
  ////////////////

private:
  ////////////////////////////////////////
  // Private data
  //////////////
  std::map<int, std::string> m_atomTypes;
  std::map<std::string, int> m_implicitHydrogens;
  std::map<std::string, int> m_atomicNumber;
  std::map<std::string, int> m_formalCharge;
  RbtStringHybridStateMap m_hybridState; // DM 8 Dec 1998
};

// useful typedefs
typedef SmartPtr<RbtCharmmDataSource> RbtCharmmDataSourcePtr; // Smart pointer

#endif //_RBTCHARMMDATASOURCE_H_
