#include "stdafx.h"
#include "Type.hh"

#include "Exceptions.hh"
#include <set>

#define POINTER_SIZE  8
#define POINTER_WIDTH 64

enum class Info {
  isVoid,
  isInteger,
  //isBool,
  isReal,
  isPointer,
  isFunction,
  isSimdVector,
  //isAggregate,
  isArray,
  isStruct,
};

// Should be trivial class
class AngieTypeImpl {
public:
  Info   info;
  size_t allocSize;
  size_t bitwidth;
  const AngieTypeImpl* innerType;

  /*ctr*/ AngieTypeImpl() = default;

  /*ctr*/ AngieTypeImpl(Info info, size_t size, size_t width) :
    info{info},
    allocSize{size},
    bitwidth{bitwidth}
  {
  }
  
  /*ctr*/ AngieTypeImpl(Info info, size_t size, size_t bitwidth, const AngieTypeImpl* innerType) :
    info{info},
    allocSize{size},
    innerType{innerType}
  {
  }
  
  bool operator==(const AngieTypeImpl& rhs) const
  {
    return
      allocSize == rhs.allocSize &&
      bitwidth == rhs.bitwidth &&
      info == rhs.info;
  }
  bool operator<(const AngieTypeImpl& rhs) const
  {
    return
      allocSize < rhs.allocSize &&
      bitwidth < rhs.bitwidth &&
      info < rhs.info;
  }
};

static std::set<AngieTypeImpl> typeDb;

Type AngieType::CreateVoidType()
{
  //TODO: switch to emplace!
  auto res = typeDb.insert(AngieTypeImpl{Info::isVoid, 0, 0ull});
  return Type{&*res.first};
}

Type AngieType::CreateCharPointerType()
{
  //TODO: switch to emplace!
  auto res = typeDb.insert(AngieTypeImpl{Info::isPointer, POINTER_SIZE, POINTER_WIDTH});
  return Type{&*res.first};
}

Type AngieType::CreateIntegerType(unsigned bitwidth)
{
  //TODO@michkot: fix this hack
  size_t closestPowerOfTwoSize =
    (bitwidth % sizeof(char)) + 1;

  //TODO: switch to emplace!
  auto res = typeDb.insert(AngieTypeImpl{Info::isInteger, closestPowerOfTwoSize, bitwidth});
  return Type{&*res.first};
}

Type AngieType::CreatePointerTo(Type target)
{
  //TODO: switch to emplace!
  auto res = typeDb.insert(AngieTypeImpl{Info::isPointer, POINTER_SIZE, POINTER_WIDTH, target.frontedId});
  return Type{&*res.first};
}

void AngieType::ToString(std::string & str) const
{
}

size_t AngieType::GetSizeOf() const
{
  return frontedId->allocSize;
}

size_t AngieType::GetBitWidth() const
{
  return frontedId->bitwidth;
}

bool AngieType::IsInteger() const
{
  return frontedId->info == Info::isInteger;
}

bool AngieType::IsInteger(unsigned bitwidth) const
{
  return IsInteger() && frontedId->bitwidth == bitwidth;
}

bool AngieType::IsReal() const
{
  return frontedId->info == Info::isReal;
}

bool AngieType::IsPointer() const
{
  return frontedId->info == Info::isPointer;
}

bool AngieType::IsFunction() const
{
  return frontedId->info == Info::isReal;
}

bool AngieType::IsSimdVector() const
{
  return frontedId->info == Info::isSimdVector;
}

bool AngieType::IsAggregateType() const
{
  return frontedId->info == Info::isArray || frontedId->info == Info::isStruct;
}

bool AngieType::IsArray() const
{
  return frontedId->info == Info::isArray;
}

bool AngieType::IsStruct() const
{
  return frontedId->info == Info::isStruct;
}

bool AngieType::IsVoid() const
{
  return frontedId->info == Info::isVoid;
}

Type AngieType::GetPointerToType() const
{
  throw NotImplementedException();
}

Type AngieType::GetPointerElementType() const
{
  throw NotImplementedException();
}

Type AngieType::GetStructElementType(unsigned index) const
{
  throw NotImplementedException();
}

size_t AngieType::GetStructElementOffset(unsigned index) const
{
  throw NotImplementedException();
}