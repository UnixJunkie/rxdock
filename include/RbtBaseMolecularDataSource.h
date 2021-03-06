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

// Abstract base class (interface) for molecular data sources

#ifndef _RBTBASEMOLECULARDATASOURCE_H_
#define _RBTBASEMOLECULARDATASOURCE_H_

#include "RbtAtom.h"
#include "RbtBond.h"
#include "RbtConfig.h"
#include "RbtVariant.h"

class RbtBaseMolecularDataSource {
public:
  ////////////////////////////////////////
  // Constructors/destructors

  // Parameterised constructor
  // strName is a descriptive text label for the type of data source
  // Derived classes should use this constructor to set a suitable name
  RbtBaseMolecularDataSource(std::string strName) : m_strName(strName) {}

  virtual ~RbtBaseMolecularDataSource() {} // Default destructor

  ////////////////////////////////////////
  // Public methods
  ////////////////
  std::string GetName() {
    return m_strName;
  } // Returns source identifier string

  virtual bool isTitleListSupported() { return false; }
  virtual bool isAtomListSupported() { return false; }
  virtual bool isCoordinatesSupported() { return false; }
  virtual bool isBondListSupported() { return false; }

  // Pure virtual - derived classes must override

  // Reset source so that next time it is used, a new set of atom and bond
  // objects are created
  virtual void Reset() = 0;

  virtual int GetNumTitles() = 0;
  virtual int GetNumAtoms() = 0;
  virtual int GetNumBonds() = 0;
  virtual int GetNumSegments() = 0;

  virtual std::vector<std::string> GetTitleList() = 0;
  virtual RbtAtomList GetAtomList() = 0;
  virtual RbtBondList GetBondList() = 0;
  virtual RbtSegmentMap GetSegmentMap() = 0;

  // DM 12 May 1999 - support for data records (e.g. SD file)
  // Does source support data records (default=false)
  virtual bool isDataSupported() { return false; }
  // Get number of data fields
  virtual int GetNumData() = 0;
  // Get list of field names as string list
  virtual std::vector<std::string> GetDataFieldList() = 0;
  // Get all data as map of key=field name, value=variant (double,string or
  // string list)
  virtual RbtStringVariantMap GetDataMap() = 0;
  // Query as to whether a particular data field name is present
  virtual bool isDataFieldPresent(const std::string &strDataField) = 0;
  // Get a particular data value
  virtual RbtVariant GetDataValue(const std::string &strDataField) = 0;

private:
  ////////////////////////////////////////
  // Private methods
  /////////////////
  RbtBaseMolecularDataSource(); // Disable default constructor
  RbtBaseMolecularDataSource(const RbtBaseMolecularDataSource
                                 &); // Copy constructor disabled by default
  RbtBaseMolecularDataSource &
  operator=(const RbtBaseMolecularDataSource
                &); // Copy assignment disabled by default

private:
  ////////////////////////////////////////
  // Private data
  //////////////
  std::string m_strName; // Source identifier string
};

#endif //_RBTBASEMOLECULARDATASOURCE_H_
