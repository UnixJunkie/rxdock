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

// Abstract base class for molecular file-based data sources

#ifndef _RBTBASEMOLECULARFILESOURCE_H_
#define _RBTBASEMOLECULARFILESOURCE_H_

#include "RbtBaseFileSource.h"
#include "RbtBaseMolecularDataSource.h"
#include "RbtParameterFileSource.h"

class RbtBaseMolecularFileSource : public RbtBaseFileSource,
                                   public RbtBaseMolecularDataSource {
public:
  // Constructors
  // RbtBaseMolecularFileSource(const char* fileName, const char* sourceName);
  // Single-record version
  RBTDLL_EXPORT RbtBaseMolecularFileSource(const std::string &fileName,
                                           const std::string &sourceName);
  // Multi-record version, with record delimiter passed as an argument
  RbtBaseMolecularFileSource(const std::string &fileName,
                             const std::string &strRecDelim,
                             const std::string &sourceName);

  // Destructor
  virtual ~RbtBaseMolecularFileSource();

  // Pure virtual methods from RbtBaseMolecularDataSource
  void Reset();
  int GetNumTitles();
  int GetNumAtoms();
  int GetNumBonds();
  int GetNumSegments();

  std::vector<std::string> GetTitleList();
  RbtAtomList GetAtomList();
  RbtBondList GetBondList();
  RbtSegmentMap GetSegmentMap();

  // DM 12 May 1999 - support for data records (e.g. SD file)
  // Get number of data fields
  virtual int GetNumData();
  // Get list of field names as string list
  virtual std::vector<std::string> GetDataFieldList();
  // Get all data as map of key=field name, value=variant (double,string or
  // string list)
  virtual RbtStringVariantMap GetDataMap();
  // Query as to whether a particular data field name is present
  virtual bool isDataFieldPresent(const std::string &strDataField);
  // Get a particular data value
  virtual RbtVariant GetDataValue(const std::string &strDataField);

  // These methods allow filtering of the data source by segment name
  // So for example, we could just read the segment named TAR from the source
  // The filter modifies the behaviour of GetAtomList, GetBondList etc
  RbtSegmentMap GetSegmentFilterMap();
  RBTDLL_EXPORT void SetSegmentFilterMap(const RbtSegmentMap &segmentFilterMap);
  void ClearSegmentFilterMap();
  bool isSegmentFilterMapDefined();

protected:
  // Protected methods
  void ClearMolCache(); // Clear the atom, bond and title lists
  void
  RemoveAtom(RbtAtomPtr spAtom); // DM 16 Feb 2000 - moved from RbtMdlFileSource
  void
  RenumberAtomsAndBonds(); // DM 30 Oct 2000 - now independent from RemoveAtom
  // Moved from RbtMOL2FileSource - sets partial charges on a per-residue basis
  // Automatically determines protonation states of ASP/GLU/HIS using Mandatory
  // and Forbidden atoms in spParamSource
  void SetupPartialIonicGroups(RbtAtomList &atoms,
                               RbtParameterFileSourcePtr spParamSource);

private:
  // Private methods
  RbtBaseMolecularFileSource(); // Disable default constructor
  RbtBaseMolecularFileSource(const RbtBaseMolecularFileSource
                                 &); // Copy constructor disabled by default
  RbtBaseMolecularFileSource &
  operator=(const RbtBaseMolecularFileSource
                &); // Copy assignment disabled by default

  // Get methods with segment filter applied
  int GetNumAtomsWithFilter();
  int GetNumBondsWithFilter();
  RbtAtomList GetAtomListWithFilter();
  RbtBondList GetBondListWithFilter();

protected:
  // Protected data
  std::vector<std::string> m_titleList;
  RbtAtomList m_atomList;
  RbtBondList m_bondList;
  RbtSegmentMap m_segmentMap;
  RbtStringVariantMap m_dataMap; // DM 12 May 1999 - for storing data records

private:
  // Private data
  RbtSegmentMap m_segmentFilterMap;
};

// useful typedefs
typedef SmartPtr<RbtBaseMolecularFileSource>
    RbtMolecularFileSourcePtr; // Smart pointer

#endif //_RBTBASEMOLECULARFILESOURCE_H_
