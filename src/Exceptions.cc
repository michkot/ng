#include "Exceptions.hh"

NotImplementedException::NotImplementedException(const char* name) :
  logic_error(
    std::string{"Method "}
    .append(name)
    .append("is not implemented.")
    )
  {}

NotSupportedException::NotSupportedException(const char* name, const char* type) :
  logic_error(
    std::string{"Method "}
      .append(name)
      .append("is not supported in class of type")
      .append(type)
      .append(".")
    ) 
  {}

// Here i used originaly operator+ for concat but I ran into this problem:
// C:/Apps/msys64/mingw64/lib/libLLVMAsmParser.a(LLParser.cpp.obj):(.text$_ZStplIcSt11char_traitsIcESaIcEENSt7__cxx1112basic_stringIT_T0_T1_EEOS8_PKS5_[_ZStplIcSt11char_traitsIcESaIcEENSt7__cxx1112basic_stringIT_T0_T1_EEOS8_PKS5_]+0x0): multiple definition of `std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > std::operator+<char, std::char_traits<char>, std::allocator<char> >(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >&&, char const*)'
// CMakeFiles/bordel1.dir/objects.a(Exceptions.cc.obj):(.text[_ZStplIcSt11char_traitsIcESaIcEENSt7__cxx1112basic_stringIT_T0_T1_EEOS8_PKS5_]+0x0): first defined here
