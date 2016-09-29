#pragma once

// Basic type hierarchy for SMG implementation
// (c) PP, 2016

#include <string>
#include <map>
#include <vector>
#include <memory>
#include <iostream>

typedef void* frontend_id;    // unique GCC id (or LLVM representation)

// Type -- reference to type representation (copyable, assignable)
class Type {
 private:
  virtual frontend_id FrontendId() const = 0;

 public:
  bool operator==(const Type& rhs) {
    return FrontendId() == rhs.FrontendId();
  }

  virtual size_t SizeOf() const = 0;
  virtual size_t Width() const = 0;
  virtual std::string ToString() const = 0;

  virtual bool IsInteger() const = 0;
  virtual bool IsPointer() const = 0;
  virtual bool IsStruct() const = 0;
  virtual bool IsUnion() const = 0;
  virtual bool IsArray() const = 0;
};
