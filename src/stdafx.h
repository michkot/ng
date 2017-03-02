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
/** @file stdafx.h */

// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#if defined(_MSC_VER)

#include "Definitions.hh"
#include "Exceptions.hh"
//#include "Type.hh"
//#include "Values.hh"

//#include <gsl/gsl_algorithm>
#include <vector>
#include <map>
#include <algorithm>
#include <string>
#include <sstream>

#include <iostream>

#pragma warning(push, 3)
#include <range/v3/all.hpp>
#pragma warning(pop)

#endif