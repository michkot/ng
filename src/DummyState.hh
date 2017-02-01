//#pragma once
//
//#include <map>
//#include "Definitions.hh"
//#include "General.hh"
//#include "IOperation.hh"
//#include "Values.hh"
//#include "IState.hh"
//
//typedef int DummyValueId;
//
//class DummyState : public IState {
//private:
//  std::map<FrontendValueId, ValueId> globalVarMapping;
//  std::map<FrontendValueId, ValueId> varMapping;
//
//public:
//  virtual ~DummyState() override {}
//
//  virtual void AddGlobalVar(OperArg var) override
//  {
//    globalVarMapping.insert_or_assign(var.id, ValueId{});
//  }
//
//  virtual void LinkLocalVar(OperArg var, ValueId value) override {}
//  virtual ValueId AddOrGetLocalVar(OperArg var) override
//  {
//    varMapping.insert_or_assign(var.id, ValueId{});
//    return ValueId{};
//  }
//
//  virtual void DelLocalVar(OperArg var) override
//  {
//    throw NotImplementedException{"DelLocalVar"};
//  }
//  virtual void PushFrame(FunctionCallInfo info) override
//  {
//    throw NotImplementedException{"PushFrame"};
//  }
//  virtual void PopFrame(OperArg retVar) override
//  {
//    throw NotImplementedException{"PopFrame"};
//  }
//
//  /*ctr*/ DummyState(ICfgNode& lastNode, ICfgNode& nextNode) :
//    IState(lastNode, nextNode),
//    globalVarMapping{},
//    varMapping{}
//  { }
//
//};
