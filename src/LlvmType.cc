#include "Type.hh"

#if TYPE_KIND == TYPE_KIND_LLVM

#include "LlvmGlobals.hh"

//#include <innerMap>
//#include <vector>
//#include <memory>
//#include <iostream>

#include <llvm/IR/Type.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/DataLayout.h>
#include <llvm/Support/raw_ostream.h>

Type LlvmType::CreateVoidType()
{
  return Type(nullptr);
}

Type LlvmType::CreateCharPointerType()
{
  static auto ptrTy = llvm::Type::getInt8PtrTy(llvmModule->getContext());
  return Type{ptrTy};
}

void LlvmType::ToString(std::string& str) const
{
  llvm::raw_string_ostream ost{str};
  GetFrontendId()->print(ost);
  return;
}

size_t LlvmType::GetSizeOf() const
{
  // assert on getting size of not-sized object is included in DataLayout method

  return llvmDataLayout.getTypeAllocSize(GetFrontendId());
}

size_t LlvmType::GetBitWidth() const
{
  // assert on getting size of not-sized object is included in DataLayout method

  return llvmDataLayout.getTypeSizeInBits(GetFrontendId());
}
bool LlvmType::IsInteger() const
{
  return GetFrontendId()->isIntegerTy();
}

bool LlvmType::IsInteger(unsigned bitwidth) const
{
  return GetFrontendId()->isIntegerTy(bitwidth);
}

bool LlvmType::IsReal() const
{
  return GetFrontendId()->isFloatingPointTy();
}

bool LlvmType::IsPointer() const
{
  return GetFrontendId()->isPointerTy();
}

bool LlvmType::IsFunction() const
{
  return GetFrontendId()->isFunctionTy();
}

bool LlvmType::IsSimdVector() const
{
  return GetFrontendId()->isVectorTy();
}

bool LlvmType::IsAggregateType() const
{
  return GetFrontendId()->isAggregateType();
}

bool LlvmType::IsArray() const
{
  return GetFrontendId()->isArrayTy();
}

bool LlvmType::IsStruct() const
{
  return GetFrontendId()->isStructTy();
}

bool LlvmType::IsVoid() const
{
  return GetFrontendId() == nullptr;
}

Type LlvmType::GetPointerToType() const
{
  return LlvmType{static_cast<const llvm::Type*>(GetFrontendId())->getPointerTo()};
}

LlvmType LlvmType::GetPointerElementType() const
{
  assert(GetFrontendId()->isPointerTy() && "Is not a pointer type");

  return LlvmType{static_cast<const llvm::PointerType*>(GetFrontendId())->getElementType()};
}

LlvmType LlvmType::GetStructElementType(unsigned index) const
{
  assert(GetFrontendId()->isStructTy() && "Is not a struct type");

  return LlvmType{static_cast<const llvm::StructType*>(GetFrontendId())->getElementType(index)};
}

size_t LlvmType::GetStructElementOffset(unsigned index) const
{
  assert(GetFrontendId()->isStructTy() && "Is not a struct type");
  auto structType = static_cast<llvm::StructType*>(GetFrontendId());
  auto layout = llvmDataLayout.getStructLayout(structType);

  return layout->getElementOffset(index);
}

#endif
