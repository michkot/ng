#pragma once


#include "Definitions.hh"
#include "General.hh"
#include "IOperation.hh"
#include "ICfgNode.hh"
#include "IState.hh"
#include "StateStorage.hh"
#include "DummyState.hh"

#include <map>
#include <cassert>

#include <range/v3/all.hpp>


class Mapper {

private:

  IValueContainer* vc;
  std::map<FrontendValueId, ValueId> innerMap;

public:

  /*ctr*/ Mapper(IValueContainer& vc) : vc(&vc) {}

  /*ctr*/ Mapper(const Mapper&) = default;

  ValueId GetValueId(FrontendValueId id)
  {
    return innerMap.at(id);
  }
  
  auto GetFrontendIds(ValueId id)
  {
    return innerMap | ranges::view::remove_if([=](STL_ITEM_T(innerMap) pair){ return pair.second == id; });
  }

  void LinkToValueId(FrontendValueId id, ValueId value)
  {
    innerMap.insert({id, value});
  }

  ValueId CreateOrGetValueId(OperArg arg) 
  {
    // src: http://stackoverflow.com/a/101980
    typedef std::map<FrontendValueId, ValueId> MapType;   // Your innerMap type may vary, just change the typedef

    MapType::iterator lb = innerMap.lower_bound(arg.id);

    if (lb != innerMap.end() && !(innerMap.key_comp()(arg.id, lb->first)))
    {
      // key already exists
      // update lb->second if you care to
      return lb->second;
    }
    else
    {
      // the key does not exist in the innerMap
      // add it to the innerMap
      auto newValue = vc->CreateVal(arg.type);
      innerMap.insert(lb, MapType::value_type(arg.id, newValue));    // Use lb as a hint to insert,
                                                                     // so it can avoid another lookup
      return newValue;
    }
  }

};



class MapperDebug {
private:

  std::map<FrontendValueId, ValueId> map;

public:

  void LinkToValueId(FrontendValueId id, ValueId value)
  {
    map.insert({id, value});
  }

  ValueId CreateOrGetValueId(FrontendValueId id) 
  {
    // src: http://stackoverflow.com/a/101980
    typedef std::map<FrontendValueId, ValueId> MapType;   // Your innerMap type may vary, just change the typedef

    MapType::iterator lb = map.lower_bound(id);

    if (lb != map.end() && !(map.key_comp()(id, lb->first)))
    {
      // key already exists
      // update lb->second if you care to
      return lb->second;
    }
    else
    {
      // the key does not exist in the innerMap
      // add it to the innerMap
      ValueId newValue{};
      map.insert(lb, MapType::value_type(id, newValue));    // Use lb as a hint to insert,
                                                            // so it can avoid another lookup
      return newValue;
    }
  }

};