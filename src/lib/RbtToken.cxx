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

#include "RbtToken.h"
#include "RbtCommands.h"
#include "RbtDebug.h"

static const RbtVble default_vble;

std::string RbtToken::_CT("RbtToken");

///////////////////
// Constructors
///////////////////
RbtToken::RbtToken(const RbtVble &v) : comm(-1), vble(v), isvble(true) {
  _RBTOBJECTCOUNTER_CONSTR_(_CT);
}

RbtToken::RbtToken(RbtCommands c) : comm(c), vble(default_vble), isvble(false) {
  _RBTOBJECTCOUNTER_CONSTR_(_CT);
}

RbtToken::RbtToken(const RbtToken &t)
    : comm(t.comm), vble(t.vble), isvble(t.isvble) {
  _RBTOBJECTCOUNTER_COPYCONSTR_(_CT);
}

///////////////////
// Destructor
//////////////////
RbtToken::~RbtToken() { _RBTOBJECTCOUNTER_DESTR_(_CT); }

/*void RbtToken::copy(const RbtToken &t)
{
    isvble = t.isvble;
    comm = t.comm;
    vble = t.vble;
}*/

const RbtVble &RbtToken::GetVble() const {
  if (!isvble)
    throw RbtError(_WHERE_, "the token is not a vble");
  return vble;
}
/* void RbtToken::SetVbleNumber(RbtInt vn)
{
    vble.SetValue(vn);
}*/

bool RbtToken::IsVble() { return (isvble); }
bool RbtToken::IsLog() { return (!isvble && comm.IsLog()); }
bool RbtToken::IsExp() { return (!isvble && comm.IsExp()); }
bool RbtToken::IsAdd() { return (!isvble && comm.IsAdd()); }
bool RbtToken::IsSub() { return (!isvble && comm.IsSub()); }
bool RbtToken::IsMul() { return (!isvble && comm.IsMul()); }
bool RbtToken::IsDiv() { return (!isvble && comm.IsDiv()); }
bool RbtToken::IsAnd() { return (!isvble && comm.IsAnd()); }
bool RbtToken::IsIf() { return (!isvble && comm.IsIf()); }
