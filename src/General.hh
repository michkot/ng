/*******************************************************************************

Copyright (C) 2017 Michal Kotoun

This file is a part of Angie project.

Angie is free software: you can redistribute it and/or modify it under the
terms of the GNU Lesser General Public License as published by the Free
Software Foundation, either version 3 of the License, or (at your option)
any later version.

Angie is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
more details.

You should have received a copy of the GNU Lesser General Public License
along with Angie.  If not, see <http://www.gnu.org/licenses/>.

*******************************************************************************/
/** @file General.hh */

#pragma once

#include <cstdint>

#include "Type.hh"

class FrontendValueId {
  // Fields
private:
  uint64_t id;

  // Constructors
private:
  /**/     FrontendValueId()            : id{ 0 }  { }
public:
  explicit FrontendValueId(uint64_t id) : id{ id } { }

  // Conversion methods
  explicit operator uint64_t() const { return id; }

  // Methods
  FrontendValueId operator++()    { id++; return *this; } // prefix
  FrontendValueId operator++(int) { auto copy = *this; id++; return copy; } // postfix

  bool operator==(const FrontendValueId& other) const { return this->id == other.id; }
  bool operator!=(const FrontendValueId& other) const { return this->id != other.id; }
  bool operator< (const FrontendValueId& other) const { return this->id < other.id; }
  bool operator> (const FrontendValueId& other) const { return this->id > other.id; }
  bool operator<=(const FrontendValueId& other) const { return this->id <= other.id; }
  bool operator>=(const FrontendValueId& other) const { return this->id >= other.id; }

};

class OperArg {
public:
  FrontendValueId id;
  Type type;

  /*ctr*/ OperArg(FrontendValueId id, Type type) : id{id}, type{type} {}

};

//předávané argumenty, návratový typ, návratová lokace/instrukce
typedef int FunctionCallInfo;
