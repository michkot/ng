
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

