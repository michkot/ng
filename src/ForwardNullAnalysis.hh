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
/** @file ForwardNullAnalysis.hh */

#pragma once

#include "Definitions.hh"
#include "General.hh"
#include "IOperation.hh"
#include "ICfgNode.hh"
#include "IState.hh"
#include "StateStorage.hh"
#include "DummyState.hh"

#include <cassert>

#include "FrontedValueMapper.hh"
//#include <range/v3/all.hpp>

using namespace ::std;

class StackFrame {
public:
  int currentOffset{0};
  void AllocateMem(size_t size) { currentOffset += size; }
};

// ------------
// ForwardNullAnalysis
class ForwardNullAnalysisState : public IState {
private:

  Mapper globalMapping;
  Mapper localMapping;

public:

  /*ctr*/ ForwardNullAnalysisState(
      ICfgNode& lastCfgNode, 
      ICfgNode& nextCfgNode,
      IValueContainer& vc,
      Mapper& globalMapping
      ) :
    IState(lastCfgNode, nextCfgNode),
    vc(&vc),
    globalMapping(globalMapping),
    localMapping(vc)
  {
    //stack.push_back(StackFrame{});
    stackCurrentAddr = ValueId{0};//GetVC().CreateVal(Type::CreateCharPointerType());
    //stackBaseAddr = GetVC().CreateVal(Type::CreateCharPointerType());
    //heapBaseAddr = GetVC().CreateVal(Type::CreateCharPointerType());
  }

  // copy ctr ??? or what
  /*ctr*/ ForwardNullAnalysisState(
      const ForwardNullAnalysisState& state, 
      ICfgNode& lastCfgNode, ICfgNode& 
      nextCfgNode
      ) :
    IState(lastCfgNode, nextCfgNode),
    vc(state.vc),
    globalMapping(state.globalMapping),
    localMapping(state.localMapping),
    //stack(state.stack),
    //stackMemory(state.stackMemory),
    //heapMemory(state.heapMemory),
    stackCurrentAddr(state.stackCurrentAddr),
    hasValue(state.hasValue)
  {
  }
  
  //TODO: same algo as in method bellow
  //TODO: rename
  
  virtual ValueId GetAnyVar(FrontendIdTypePair var) override
  {
    try { return globalMapping.GetValueId(var.id); }
    catch (exception e) { return localMapping.GetValueId(var.id); }
  }
  // DO NOT USE - use GetAnyOrCreateLocalVar
  virtual ValueId GetOrCreateGlobalVar(FrontendIdTypePair var) override
  {
    return globalMapping.CreateOrGetValueId(var);
  }
  // DO NOT USE - use GetAnyOrCreateLocalVar
  virtual ValueId GetOrCreateLocalVar(FrontendIdTypePair var) override
  {
    return localMapping.CreateOrGetValueId(var);
  }

  virtual ValueId GetAnyOrCreateLocalVar(FrontendIdTypePair var) override
  {
    try { return globalMapping.GetValueId(var.id); }
    catch(exception e) { return localMapping.CreateOrGetValueId(var); }
  }
  virtual void LinkGlobalVar(FrontendIdTypePair var, ValueId value) override
  {
    globalMapping.LinkToValueId(var.id, value);
  }
  virtual void LinkLocalVar(FrontendIdTypePair var, ValueId value) override
  {
    localMapping.LinkToValueId(var.id, value);
  }

  IValueContainer* vc;
  IValueContainer& GetVC() { return *vc; }

  vector<string> stackMemory;
  //vector<string> heapMemory;
  //vector<StackFrame> stack;

  //ValueId stackBaseAddr;
  //ValueId heapBaseAddr;
  ValueId stackCurrentAddr;
  //ValueId heapCurrentAddr;

  std::map<ValueId, ValueId> hasValue;

  void Store(ValueId what, ValueId where)
  {
    hasValue[where] = what;
  }
  ValueId Load(ValueId where)
  {
    return hasValue.at(where);
  }


};

class AnalysisErrorException : public std::logic_error {
public:
  /*ctr*/ AnalysisErrorException()
    : logic_error("A fatal error was discoverd by the analysis. Abstract execution can not continue in this path.")
  {
  }
  /*ctr*/ AnalysisErrorException(const char* c)
    : logic_error(c)
  {
  }

};

class InvalidDereferenceException : public AnalysisErrorException {
public:
  /*ctr*/ InvalidDereferenceException()
    : AnalysisErrorException("Program tried to dereference unallocated or unintialized memmory.")
  {
  }
};

class PossibleNullDereferenceException : public AnalysisErrorException {
public:
  /*ctr*/ PossibleNullDereferenceException()
    : AnalysisErrorException("Possible null dereference exception occured - getElementPtr.")
  {
  }
};



/// <summary>
/// Base class for implementing Operation. 
/// </summary>
////template<bool isBranching>
class BaseOperation : public IOperation {
public:

  virtual uptr<IState> CreateSuccesor     (IState& initialState) = 0;
          uptr<IState> CreateSuccesorTrue (IState& initialState) { return CreateSuccesor(initialState); }
  virtual uptr<IState> CreateSuccesorFalse(IState& initialState) { throw NotImplementedException(); }

  virtual void         ExecuteOnNewState  (IState& newState, const vector<OperArg>& args) = 0;

  // Override this again to switch to branching implementation
  virtual void Execute(IState& originalState, const vector<OperArg>& args) override
  {
    ExecuteImplNonbranching(originalState, args);
  }

private:

  // non-branching variant
  ////template <bool T = isBranching, typename std::enable_if_t<!T>* = nullptr>
  void ExecuteImplNonbranching(IState& originalState, const vector<OperArg>& args)
  {
    assert(!originalState.nextCfgNode.HasTwoNext()); //TODO: comment

    uptr<IState> succesor;
    succesor = CreateSuccesor(originalState);    
    try
    {
      //TODO: moved into the try/catch/finally lbock, is it correct?
      originalState.SetExplored();
      ExecuteOnNewState(*succesor, args);

      // Handled in TerminalCfgNode::Execute()
      /*if (originalState.nextCfgNode.IsTerminalNode())
        succesor->SetExplored();*/

      originalState.nextCfgNode.GetStatesManager().InsertAndEnqueue(move(succesor));
    }
    catch (AnalysisErrorException e)
    {
      originalState.nextCfgNode.PrintLocation();
      printf("%s\n",e.what());
    }
    
    return;
  }

  // branching variant
  ////template <bool T = isBranching, typename std::enable_if_t<T>* = nullptr>
  void ExecuteImplBranching(IState& originalState, const vector<OperArg>& args)
  {
    assert(originalState.nextCfgNode.HasTwoNext()); //TODO: comment

    uptr<IState> succesor;
    succesor = CreateSuccesorTrue(originalState);
    ExecuteOnNewState(*succesor, args);
    originalState.nextCfgNode.GetStatesManager().InsertAndEnqueue(move(succesor));

    succesor = CreateSuccesorFalse(originalState);
    ExecuteOnNewState(*succesor, args);
    originalState.nextCfgNode.GetStatesManager().InsertAndEnqueue(move(succesor));

    originalState.SetExplored();
    return;
  }

};

#define PTR_TYPE Type::CreateCharPointerType()

////template<bool isBranching>
class BaseForwardNullAnalysisOperation : public BaseOperation/*<isBranching>*/ {
public:
  
  virtual uptr<IState> CreateSuccesor(IState& s) override
  {
    auto succesorPtr = make_unique<ForwardNullAnalysisState>(dynamic_cast<ForwardNullAnalysisState&>(s), s.nextCfgNode, s.nextCfgNode.GetNext());
    auto& succesor = *succesorPtr; 
    // FIXME: when compiler is in compliance with newer copy elision rules
    // http://www.open-std.org/jtc1/sc22/wg21/docs/cwg_defects.html#1579
    return move(succesorPtr);
  }
  virtual uptr<IState> CreateSuccesorFalse(IState& s) override
  {
    auto succesorPtr = make_unique<ForwardNullAnalysisState>(dynamic_cast<ForwardNullAnalysisState&>(s), s.nextCfgNode, s.nextCfgNode.GetNextFalse());
    auto& succesor = *succesorPtr; 
    
    // FIXME: when compiler is in compliance with newer copy elision rules
    // http://www.open-std.org/jtc1/sc22/wg21/docs/cwg_defects.html#1579
    return move(succesorPtr);
  }
  virtual void ExecuteOnNewState(IState& newState, const vector<OperArg>& args) override
  {
    return ExecuteOnNewState(static_cast<ForwardNullAnalysisState&>(newState), args);
  }
  virtual void ExecuteOnNewState(ForwardNullAnalysisState& newState, const vector<OperArg>& args) = 0;
};


class FnaOperationBranch : public IOperation {
public:

  // Override this again to switch to branching implementation
  virtual void Execute(IState& originalState, const vector<OperArg>& args) override
  {
    assert(originalState.nextCfgNode.HasTwoNext()); //TODO: comment

    uptr<IState> succesor;
    succesor = CreateSuccesorTrue(originalState);
    ExecuteOnNewState(static_cast<ForwardNullAnalysisState&>(*succesor), args, true);
    originalState.nextCfgNode.GetStatesManager().InsertAndEnqueue(move(succesor));

    succesor = CreateSuccesorFalse(originalState);
    ExecuteOnNewState(static_cast<ForwardNullAnalysisState&>(*succesor), args, false);
    originalState.nextCfgNode.GetStatesManager().InsertAndEnqueue(move(succesor));

    originalState.SetExplored();
    return;
  }

  uptr<IState> CreateSuccesorTrue(IState& s)
  {
    auto succesorPtr = make_unique<ForwardNullAnalysisState>(dynamic_cast<ForwardNullAnalysisState&>(s), s.nextCfgNode, s.nextCfgNode.GetNextTrue());
    auto& succesor = *succesorPtr; 
    // FIXME: when compiler is in compliance with newer copy elision rules
    // http://www.open-std.org/jtc1/sc22/wg21/docs/cwg_defects.html#1579
    return move(succesorPtr);
  }

  uptr<IState> CreateSuccesorFalse(IState& s)
  {
    auto succesorPtr = make_unique<ForwardNullAnalysisState>(dynamic_cast<ForwardNullAnalysisState&>(s), s.nextCfgNode, s.nextCfgNode.GetNextFalse());
    auto& succesor = *succesorPtr; 
    // FIXME: when compiler is in compliance with newer copy elision rules
    // http://www.open-std.org/jtc1/sc22/wg21/docs/cwg_defects.html#1579
    return move(succesorPtr);
  }

  void ExecuteOnNewState(ForwardNullAnalysisState& newState, const vector<OperArg>& args, bool br)
  {
    auto     lhs          = newState.GetAnyVar   (args[2].idTypePair);

    if (br)
      newState.GetVC().AssumeTrue(lhs);
    else
      newState.GetVC().AssumeFalse(lhs);
  }
};


class FnaOperationBinary : public BaseForwardNullAnalysisOperation {
  virtual void ExecuteOnNewState(ForwardNullAnalysisState& newState, const vector<OperArg>& args) override
  {
    auto opts = args[1].binOpOpts;

    auto lhs  = newState.GetAnyVar(args[2].idTypePair);
    auto rhs  = newState.GetAnyVar(args[3].idTypePair);
    auto type = args[2].idTypePair.type;
    
    ValueId retVal = newState.GetVC().BinOp(lhs, rhs, type, opts);
    newState.LinkLocalVar(args[0].idTypePair, retVal);
  }

};

class FnaOperationGetElementPtr : public BaseForwardNullAnalysisOperation {
  virtual void ExecuteOnNewState(ForwardNullAnalysisState& newState, const vector<OperArg>& args) override
  {
    // consider packing and alligment!!!

    //auto numOfIndexes = args.size() - 2;

    //auto lvl0Size = args[2].idTypePair.type.GetPointerElementType().GetSizeOf();

    //if (args[2].idTypePair.type.IsStruct())
    //{
    //  auto lvl1Size = args[2].idTypePair.type.GetPointerElementType().GetStructElementOffset(/*and here index*/);
    //}
    
    auto     lhs    = newState.GetAnyVar   (args[2].idTypePair);
    uint64_t offset = static_cast<uint64_t>(args[1].idTypePair.id); //HACK relaying on ValueId == constant value stored by that id    

    //! We assume, that getelementptr instruction is always generated as forerunner of load/store op.
    if (newState.GetVC().IsZero(lhs))
    {
      throw PossibleNullDereferenceException();
    }
    
    ValueId offsetVal     = newState.GetVC().CreateConstIntVal(offset, PTR_TYPE);
    ValueId retVal        = newState.GetVC().Add(lhs, offsetVal, PTR_TYPE, ArithFlags::Default);
    newState.LinkLocalVar(args[0].idTypePair, retVal);
  }
};

class FnaOperationAlloca : public BaseForwardNullAnalysisOperation {
  virtual void ExecuteOnNewState(ForwardNullAnalysisState& newState, const vector<OperArg>& args) override
  {
    auto count = newState.GetAnyVar(args[2].idTypePair);
    auto type  = args[0].idTypePair.type;

    ValueId elementSize64 = newState.GetVC().CreateConstIntVal(type.GetPointerElementType().GetSizeOf(), PTR_TYPE);
    ValueId count64       = newState.GetVC().ExtendInt(count, args[2].idTypePair.type, PTR_TYPE, ArithFlags::Default);
    ValueId size64        = newState.GetVC().Mul(elementSize64, count64, PTR_TYPE, ArithFlags::Default);
    ValueId retVal        = newState.GetVC().Add(newState.stackCurrentAddr, size64, PTR_TYPE, ArithFlags::Default);

    newState.stackCurrentAddr = retVal;
    newState.LinkLocalVar(args[0].idTypePair, retVal);
    newState.stackMemory.push_back("alokovano xy");
  }
};

class FnaOperationCall : public BaseForwardNullAnalysisOperation {
  virtual void ExecuteOnNewState(ForwardNullAnalysisState& newState, const vector<OperArg>& args) override
  {
    auto callTargetId = args[1].id;
    auto callTargetType = args[1].idTypePair.type;

    // ne need to do several thinks in here
    // A) prepare new stack frame 
    // B) bind argument values to formal parameters in called function
    // C) prepare "return info" which will bind the returned value to FrontendId of returned value in caller
    // D) plan execution of the first node in the function with state prepared according to previous points

    // for that, we need:
    // support for stack frames with "return info"
    // a system which binds FunctionPointers (of all kinds) to functions
    
    // for variable adressing of function, I propose the same idea I originaly had for stack:
    // that is, "base adress", here base address of function, beeing in unknown (abstract) value
    // and other addresses somehow based on this address
    
    
    return;
    throw NotImplementedException();
  }
};

class FnaOperationLoad : public BaseForwardNullAnalysisOperation {
  virtual void ExecuteOnNewState(ForwardNullAnalysisState& newState, const vector<OperArg>& args) override
  {

    // this operation should somehow load a value from memory to register
    // as registers are "direct values",
    // it in fact means just to bind an existing value to another FrontendValueId
    // which value is to be loaded is but entirely up to the specific analysis
    
    auto target = newState.GetAnyVar(args[2].idTypePair);
    ValueId value;
    try
    {
      value = newState.Load(target);
    }
    catch(out_of_range e)
    {
      //! We assume an invalid read if the address was not written to previously
      throw InvalidDereferenceException();
    }

    newState.LinkLocalVar(args[0].idTypePair, value);
  }
};

class FnaOperationStore : public BaseForwardNullAnalysisOperation {
  virtual void ExecuteOnNewState(ForwardNullAnalysisState& newState, const vector<OperArg>& args) override
  {

    // this operation should somehow Store a value in register to certain address in memory
    // the way for the operation to handle such a "write" is completely analysis specific
    
    //! HACK!!! this fixes the problem of main() arguments, which are defined at instr #0 without any instruction which would produce them
    auto value  = newState.GetAnyOrCreateLocalVar(args[2].idTypePair); //TODO fix this dircty hack kand maybe remove the GetAnyOrCreateLocalVar
    auto target = newState.GetAnyVar(args[3].idTypePair);

    newState.Store(value, target);
  }
};

class FnaOperationMemset : public BaseForwardNullAnalysisOperation {
  virtual void ExecuteOnNewState(ForwardNullAnalysisState& newState, const vector<OperArg>& args) override
  {

    // this operation should somehow Store a value in register to certain address in memory
    // the way for the operation to handle such a "write" is completely analysis specific
    
    auto target = newState.GetAnyVar(args[2].idTypePair);
    auto value  = newState.GetAnyVar(args[3].idTypePair);
    auto len    = newState.GetAnyVar(args[4].idTypePair);

    auto& vc = newState.GetVC();
    auto i = vc.CreateConstIntVal(0, PTR_TYPE);
    auto one = vc.CreateConstIntVal(1, PTR_TYPE);
    while (vc.IsCmp(len, i, PTR_TYPE, CmpFlags::UnsigGt))
    {
      newState.Store(value, target); 
      i      = vc.Add(i, one, PTR_TYPE, ArithFlags::Default);      
      target = vc.Add(target, one, PTR_TYPE, ArithFlags::Default);
    }
  }
};

class FnaOperationTrunc : public BaseForwardNullAnalysisOperation {
  virtual void ExecuteOnNewState(ForwardNullAnalysisState& newState, const vector<OperArg>& args) override
  {
    //newstate
    ArithFlags flags = args[1].arithFlags;
    auto lhs         = newState.GetAnyVar(args[2].idTypePair);
    auto tarType     = args[0].idTypePair.type;
    auto srcType     = args[2].idTypePair.type;
    
    ValueId retVal = newState.GetVC().TruncateInt(lhs, srcType, tarType);
    newState.LinkLocalVar(args[0].idTypePair, retVal);
  }
};

class FnaOperationExtend : public BaseForwardNullAnalysisOperation {
  virtual void ExecuteOnNewState(ForwardNullAnalysisState& newState, const vector<OperArg>& args) override
  {
    //newstate
    ArithFlags flags = args[1].arithFlags;
    auto lhs         = newState.GetAnyVar(args[2].idTypePair);
    auto tarType     = args[0].idTypePair.type;
    auto srcType     = args[2].idTypePair.type;
    
    ValueId retVal = newState.GetVC().TruncateInt(lhs, srcType, tarType);
    newState.LinkLocalVar(args[0].idTypePair, retVal);
  }
};

class FnaOperationCast : public BaseForwardNullAnalysisOperation {
  virtual void ExecuteOnNewState(ForwardNullAnalysisState& newState, const vector<OperArg>& args) override
  {
    auto lhs         = newState.GetAnyVar(args[2].idTypePair);
    auto opts = args[1].castOpOpts;
    //ArithFlags flags = static_cast<ArithFlags>(static_cast<uint64_t>(args[1].id) & 0xffff);
    //auto tarType     = args[0].idTypePair.type;
    //auto srcType     = args[2].idTypePair.type;
    
    if (opts.opKind == CastOpKind::BitCast)
      newState.LinkLocalVar(args[0].idTypePair, lhs);
    else if(opts.opKind == CastOpKind::Extend)      
      newState.LinkLocalVar(args[0].idTypePair, lhs); //TODO: hack!
    else
      throw NotImplementedException();
  }
};

class FnaOperationCmp : public BaseForwardNullAnalysisOperation {
  virtual void ExecuteOnNewState(ForwardNullAnalysisState& newState, const vector<OperArg>& args) override
  {
    //newstate
    auto lhs         = newState.GetAnyVar(args[2].idTypePair);
    auto rhs         = newState.GetAnyVar(args[3].idTypePair);
    auto srcType     = args[2].idTypePair.type;
    auto flags       = args[1].cmpFlags;
    
    ValueId retVal = newState.GetVC().Cmp(lhs, rhs, srcType, flags);
    newState.LinkLocalVar(args[0].idTypePair, retVal);
  }
};



class FnaOperationFactory : public IOperationFactory {

  IOperation* notSupported = new OperationNotSupportedOperation();
  IOperation* binop = new FnaOperationBinary();
  IOperation* gep = new FnaOperationGetElementPtr();
  IOperation* allocaop = new FnaOperationAlloca();
  IOperation* callop = new FnaOperationCall();
  IOperation* trunc = new FnaOperationTrunc();
  IOperation* extend = new FnaOperationExtend();
  IOperation* load = new FnaOperationLoad();
  IOperation* store = new FnaOperationStore();
  IOperation* cast = new FnaOperationCast();
  IOperation* memset = new FnaOperationMemset();
  IOperation* cmp = new FnaOperationCmp();
  IOperation* br = new FnaOperationBranch();
public:
  /*ctr*/ FnaOperationFactory()
  {
  }
  // Inherited via IOperationFactory
  virtual IOperation & Ret() override { return *notSupported; }
  virtual IOperation & Br()  override { return *br; }
  virtual IOperation & BinOp() override { return *binop; }

  virtual IOperation & Alloca() override { return *allocaop; }
  virtual IOperation & Load() override { return *load; }
  virtual IOperation & Store() override { return *store; }
  virtual IOperation & Call() override { return *callop; }
  virtual IOperation & Invoke() override { return *notSupported; }
  virtual IOperation & Cast() override { return *cast; }

  virtual IOperation & GetElementPtr() override { return *gep; }

  virtual IOperation & Cmp() override { return *cmp; }

  virtual IOperation& Memset() override { return *memset; }
  virtual IOperation& Memcpy() override { return *notSupported; }
  virtual IOperation& Memmove() override { return *notSupported; }
  virtual IOperation& Malloc() override { return *notSupported; }
  virtual IOperation& Free() override { return *notSupported; }

  virtual IOperation & NotSupportedInstr() override { return *notSupported; }

};