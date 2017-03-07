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
/** @file laboratory.cc */
#include "precompall.hh"

#include <vector>
#include <experimental/filesystem>
#include <range/v3/all.hpp>
#include <iostream>

using namespace ::std::experimental::filesystem;
using namespace ::ranges;

std::vector<std::string> GetExamples()
{
  auto examplesFiles = range<directory_iterator>{
    directory_iterator{current_path().append("examples")},
    directory_iterator{}
    };
  auto llFiles = examplesFiles | view::filter([](auto dir) { return dir.path().has_extension() && dir.path().extension() == ".ll"; });
  return llFiles | view::transform([](auto file) { return file.path().generic_string(); });
}

void lab_main()
{
  for (auto i : GetExamples())
  {
    std::cout << i << std::endl;
  }

  //for(auto dir : )

  exit(0);
}
