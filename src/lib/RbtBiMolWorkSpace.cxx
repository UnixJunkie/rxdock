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

#include "RbtBiMolWorkSpace.h"
#include "RbtAnnotationHandler.h"
#include "RbtBaseMolecularFileSource.h"
#include "RbtBaseSF.h"

////////////////////////////////////////
// Constructors/destructors

RbtBiMolWorkSpace::RbtBiMolWorkSpace() : RbtWorkSpace(2) {
  _RBTOBJECTCOUNTER_CONSTR_("RbtBiMolWorkSpace");
}

RbtBiMolWorkSpace::~RbtBiMolWorkSpace() {
  _RBTOBJECTCOUNTER_DESTR_("RbtBiMolWorkSpace");
}

////////////////////////////////////////
// Public methods
////////////////

// Model handling
RbtModelPtr RbtBiMolWorkSpace::GetReceptor() const {
  return GetModel(RECEPTOR);
}
RbtModelPtr RbtBiMolWorkSpace::GetLigand() const { return GetModel(LIGAND); }
RbtModelList RbtBiMolWorkSpace::GetSolvent() const {
  return GetModels(SOLVENT);
}
bool RbtBiMolWorkSpace::hasSolvent() const { return GetNumModels() > SOLVENT; }

void RbtBiMolWorkSpace::SetReceptor(RbtModelPtr spReceptor) {
  SetModel(RECEPTOR, spReceptor);
  // DM 30 Oct 2001 - no need to clear the ligand population, there may be
  // scenarios where we want to continue a ligand docking with a revised
  // receptor model
}

void RbtBiMolWorkSpace::SetLigand(RbtModelPtr spLigand) {
  // DM 30 Oct 2001 - invalidate the current population
  ClearPopulation();
  // DM 26 Aug 2002 - better to call SetModel AFTER defining the ligand as
  // flexible
  SetModel(LIGAND, spLigand);
}

void RbtBiMolWorkSpace::SetSolvent(RbtModelList solventList) {
  if (hasSolvent()) {
    SetModels(SOLVENT, solventList);
  } else {
    AddModels(solventList);
  }
}

void RbtBiMolWorkSpace::RemoveSolvent() {
  if (hasSolvent()) {
    RemoveModels(SOLVENT);
  }
}

void RbtBiMolWorkSpace::UpdateModelCoordsFromChromRecords(
    RbtBaseMolecularFileSource *pSource, int iTrace) {
  int nModels = GetNumModels();
  for (int iModel = 0; iModel < nModels; iModel++) {
    // if (iModel == LIGAND) continue;
    RbtModelPtr spModel = GetModel(iModel);
    if (spModel.Ptr()) {
      RbtChromElementPtr spChrom = spModel->GetChrom();
      if (spChrom.Ptr()) {
        unsigned int chromLength = spChrom->GetLength();
        if (chromLength > 0) {
          std::ostringstream ostr;
          ostr << "CHROM." << iModel;
          std::string chromField = ostr.str();
          if (pSource->isDataFieldPresent(chromField)) {
            // TODO: Move this code to RbtVariant class
            // Concatenate the multi-record value into a single string
            std::string chromRecord = Rbt::ConvertListToDelimitedString(
                pSource->GetDataValue(chromField), ",");
            // Now split into string values and convert to doubles
            std::vector<std::string> chromValues =
                Rbt::ConvertDelimitedStringToList(chromRecord, ",");
            if (chromLength == chromValues.size()) {
              std::vector<double> chromVec;
              for (std::vector<std::string>::const_iterator iter =
                       chromValues.begin();
                   iter != chromValues.end(); ++iter) {
                std::string chromValue(*iter);
                chromVec.push_back(std::atof(chromValue.c_str()));
              }
              spChrom->SetVector(chromVec);
              spChrom->SyncToModel();
              spModel->UpdatePseudoAtoms();
            } else if (iTrace > 0) {
              std::cout << "Mismatched chromosome sizes for model #" << iModel
                        << std::endl;
              std::cout << chromField << " record in " << pSource->GetFileName()
                        << " has " << chromValues.size() << " elements"
                        << std::endl;
              std::cout << "Expected number of elements is " << chromLength
                        << std::endl;
              std::cout << "Model chromosome not updated" << std::endl;
            }
          }
        }
      }
    }
  }
}

// Model I/O
// Saves ligand to file sink
void RbtBiMolWorkSpace::Save(bool bSaveScores) {
  SaveLigand(GetSink(), bSaveScores);
}

// Model I/O
// Saves ligand to history file sink
void RbtBiMolWorkSpace::SaveHistory(bool bSaveScores) {
  SaveLigand(GetHistorySink(), bSaveScores);
}

// Private method to save ligand to file sink, optionally with score attached
void RbtBiMolWorkSpace::SaveLigand(RbtMolecularFileSinkPtr spSink,
                                   bool bSaveScores) {
  if (spSink.Null()) // Check we have a valid sink
    return;
  RbtModelPtr spLigand(GetLigand());
  if (spLigand.Null()) // Check we have a ligand
    return;

  RbtBaseSF *pSF(GetSF());
  // If we have a scoring function, clear any current score data
  if (pSF) {
    std::string strSFName(pSF->GetFullName());
    spLigand->ClearAllDataFields(strSFName);
    spLigand->ClearAllDataFields(RbtAnnotationHandler::_ANNOTATION_FIELD);
  }
  // Save current score components as model data fields
  if (bSaveScores && pSF) {
    RbtStringVariantMap scoreMap;
    pSF->ScoreMap(scoreMap);
    for (RbtStringVariantMapConstIter vIter = scoreMap.begin();
         vIter != scoreMap.end(); vIter++) {
      spLigand->SetDataValue((*vIter).first, (*vIter).second);
    }
    // Save the chromosome values for all models for later retrieval
    spLigand->ClearAllDataFields("CHROM.");
    int nModels = GetNumModels();
    for (int iModel = 0; iModel < nModels; iModel++) {
      RbtModelPtr spModel = GetModel(iModel);
      if (spModel.Ptr()) {
        RbtChromElementPtr spChrom = spModel->GetChrom();
        if (spChrom.Ptr() && (spChrom->GetLength() > 0)) {
          spChrom->SyncFromModel();
          std::vector<double> chromVec;
          spChrom->GetVector(chromVec);
          std::ostringstream ostr;
          ostr << "CHROM." << iModel;
          spLigand->SetDataValue(ostr.str(), RbtVariant(chromVec, 72, 8));
        }
      }
    }
  }
  spSink->SetModel(spLigand);
  if (hasSolvent()) {
    spSink->SetSolvent(GetSolvent());
  } else {
    spSink->SetSolvent(RbtModelList());
  }
  spSink->Render();
}
