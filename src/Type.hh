#pragma once

#include <string>

namespace llvm
{
  class Type;
}

typedef llvm::Type* FrontendTypeId;    // unique GCC id (or LLVM representation)

class LlvmType;
typedef LlvmType Type;

//TODO@nobody: Udelat const type?? - co by to obnaselo, prosim komentovat vice

// Type -- reference to type representation (copyable, assignable)
// Might be later changed to interaface(fully abstraact class) when multiple frontends are in mind
class LlvmType {
private:
  FrontendTypeId frontedId;
  FrontendTypeId GetFrontendId() const { return frontedId; }

public:
  explicit /*ctr*/ LlvmType(FrontendTypeId frontedId) : frontedId{frontedId} {}

  static Type CreateVoidType();
  static Type CreateCharPointerType();

  bool operator==(const Type& rhs)
  {
    return GetFrontendId() == rhs.GetFrontendId();
  }

  void        ToString(std::string& str) const;
  std::string ToString() const { std::string s{}; ToString(s); return s; }

  size_t GetSizeOf() const; // Returns type's allocation size
  size_t GetBitWidth() const; // Returns type's bit size

  bool IsInteger() const;
  bool IsInteger(unsigned bitwidth) const;
  bool IsBool() const { return IsInteger(1u); }
  bool IsReal() const;
  bool IsPointer() const;
  bool IsFunction() const;
  bool IsSimdVector() const;
  bool IsAggregateType() const; // LLVM type group for Arrays and Structs
  bool IsArray() const;
  bool IsStruct() const;
  // bool IsUnion() const; // LLVM does not support Union type
  bool IsVoid() const;

  Type GetPointerToType() const;
  Type GetPointerElementType() const;
  Type GetStructElementType(unsigned index) const;
  size_t GetStructElementOffset(unsigned index) const;
};
