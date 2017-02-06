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
#include <range/v3/all.hpp>

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
    stackCurrentAddr = GetVC().CreateVal(Type::CreateCharPointerType());
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
    stackCurrentAddr(state.stackCurrentAddr)
  {
  }
  
  //TODO: same algo as in method bellow
  //TODO: rename
  
  virtual ValueId GetOrCreateGlobalVar(OperArg var) override
  {
    return globalMapping.CreateOrGetValueId(var);
  }
  virtual ValueId GetOrCreateLocalVar(OperArg var) override
  {
    return localMapping.CreateOrGetValueId(var);
  }
  virtual ValueId GetAnyOrCreateLocalVar(OperArg var) override
  {
    try { return globalMapping.GetValueId(var.id); }
    catch(exception e) { return localMapping.CreateOrGetValueId(var); }
  }
  virtual void LinkGlobalVar(OperArg var, ValueId value) override
  {
    globalMapping.LinkToValueId(var.id, value);
  }
  virtual void LinkLocalVar(OperArg var, ValueId value) override
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
    ExecuteOnNewState(*succesor, args);
    originalState.nextCfgNode.GetStatesManager().InsertAndEnqueue(
      move(succesor)
    );

    
    originalState.SetExplored();
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
    originalState.nextCfgNode.GetStatesManager().InsertAndEnqueue(
      move(succesor)
    );

    succesor = CreateSuccesorFalse(originalState);
    ExecuteOnNewState(*succesor, args);
    originalState.nextCfgNode.GetStatesManager().InsertAndEnqueue(
      move(succesor)
    );

    originalState.SetExplored();
    return;
  }

};

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
  virtual void ExecuteOnNewState(IState& newState, const vector<OperArg>& args) override
  {
    return ExecuteOnNewState(static_cast<ForwardNullAnalysisState&>(newState), args);
  }
  virtual void ExecuteOnNewState(ForwardNullAnalysisState& newState, const vector<OperArg>& args) = 0;
};

class FnaOperationBinary : public BaseForwardNullAnalysisOperation {
  virtual void ExecuteOnNewState(ForwardNullAnalysisState& newState, const vector<OperArg>& args) override
  {
    //newstate
    ArithFlags flags = static_cast<ArithFlags>(static_cast<uint64_t>(args[1].id));
    auto lhs         = newState.GetOrCreateLocalVar(args[2]);
    auto rhs         = newState.GetOrCreateLocalVar(args[3]);
    auto type        = args[2].type;
    
    ValueId retVal = ApplyOnVC(newState.GetVC(), lhs, rhs, type, flags);
    newState.LinkLocalVar(args[0], retVal);
  }

  virtual ValueId ApplyOnVC(IValueContainer& vc, const ValueId& lhs, const ValueId& rhs, const Type& type, ArithFlags flags) = 0;
};

class FnaOperationAdd : public FnaOperationBinary {
  virtual ValueId ApplyOnVC(IValueContainer& vc, const ValueId& lhs, const ValueId& rhs, const Type& type, ArithFlags flags) override
  {
    return vc.Add(lhs, rhs, type, flags);
  }
};
class FnaOperationSub : public FnaOperationBinary {
  virtual ValueId ApplyOnVC(IValueContainer& vc, const ValueId& lhs, const ValueId& rhs, const Type& type, ArithFlags flags) override
  {
    return vc.Sub(lhs, rhs, type, flags);
  }
};
class FnaOperationAnd : public FnaOperationBinary {
  virtual ValueId ApplyOnVC(IValueContainer& vc, const ValueId& lhs, const ValueId& rhs, const Type& type, ArithFlags flags) override
  {
    return vc.BitAnd(lhs, rhs, type);
  }
};

class FnaOperationGetElementPtr : public BaseForwardNullAnalysisOperation {
  virtual void ExecuteOnNewState(ForwardNullAnalysisState& newState, const vector<OperArg>& args) override
  {
    //newstate
    throw NotImplementedException();
  }
};

#define PTR_TYPE Type::CreateCharPointerType()

class FnaOperationAlloca : public BaseForwardNullAnalysisOperation {
  virtual void ExecuteOnNewState(ForwardNullAnalysisState& newState, const vector<OperArg>& args) override
  {
    auto count = newState.GetAnyOrCreateLocalVar(args[2]);
    auto type  = args[0].type;

    ValueId elementSize64 = newState.GetVC().CreateConstIntVal(type.GetPointerElementType().GetSizeOf(), PTR_TYPE);
    ValueId count64       = newState.GetVC().ExtendInt(count, args[2].type, PTR_TYPE, ArithFlags::Default);
    ValueId size64        = newState.GetVC().Mul(elementSize64, count64, PTR_TYPE, ArithFlags::Default);
    ValueId retVal        = newState.GetVC().Add(newState.stackCurrentAddr, size64, PTR_TYPE, ArithFlags::Default);

    newState.stackCurrentAddr = retVal;
    newState.LinkLocalVar(args[0], retVal);
    newState.stackMemory.push_back("alokovano xy");
  }
};

class FnaOperationCall : public BaseForwardNullAnalysisOperation {
  virtual void ExecuteOnNewState(ForwardNullAnalysisState& newState, const vector<OperArg>& args) override
  {
    //newstate
    throw NotImplementedException();
  }
};

class FnaOperationLoad : public BaseForwardNullAnalysisOperation {
  virtual void ExecuteOnNewState(ForwardNullAnalysisState& newState, const vector<OperArg>& args) override
  {
    //newstate
    throw NotImplementedException();
  }
};

class FnaOperationStore : public BaseForwardNullAnalysisOperation {
  virtual void ExecuteOnNewState(ForwardNullAnalysisState& newState, const vector<OperArg>& args) override
  {
    //newstate
    throw NotImplementedException();
  }
};

class FnaOperationTrunc : public BaseForwardNullAnalysisOperation {
  virtual void ExecuteOnNewState(ForwardNullAnalysisState& newState, const vector<OperArg>& args) override
  {
    //newstate
    throw NotImplementedException();
  }
};





class FnaOperationFactory : public IOperationFactory {

  IOperation* notSupported = new OperationNotSupportedOperation();
  IOperation* add = new FnaOperationAdd();
  IOperation* sub = new FnaOperationSub();
  IOperation* gep = new FnaOperationGetElementPtr();
  IOperation* allocaop = new FnaOperationAlloca();
  IOperation* callop = new FnaOperationCall();
  IOperation* trunc = new FnaOperationTrunc();
  IOperation* and = new FnaOperationAnd();
  IOperation* load = new FnaOperationLoad();
  IOperation* store = new FnaOperationStore();
public:
  /*ctr*/ FnaOperationFactory()
  {
  }
  // Inherited via IOperationFactory
  virtual IOperation & Ret() override { return *notSupported; }
  virtual IOperation & Br()  override { return *notSupported; }
  virtual IOperation & Add() override { return *add; }
  virtual IOperation & Sub() override { return *sub; }
  virtual IOperation & Mul() override { return *notSupported; }
  virtual IOperation & Div() override { return *notSupported; }
  virtual IOperation & Rem() override { return *notSupported; }
  virtual IOperation & Shl() override { return *notSupported; }
  virtual IOperation & Shr() override { return *notSupported; }
  virtual IOperation & And() override { return *and; }
  virtual IOperation & Or()  override { return *notSupported; }
  virtual IOperation & Xor() override { return *notSupported; }

  virtual IOperation & Alloca() override { return *allocaop; }
  virtual IOperation & Load() override { return *load; }
  virtual IOperation & Store() override { return *store; }
  virtual IOperation & Call() override { return *callop; }
  virtual IOperation & Invoke() override { return *notSupported; }
  virtual IOperation & Cast() override { return *notSupported; }

  virtual IOperation & GetElementPtr() override { return *gep; }

  virtual IOperation & Cmp() override { return *notSupported; }

  virtual IOperation & NotSupportedInstr() override { return *notSupported; }

};