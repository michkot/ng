#pragma once

#include <memory>
#include <functional>
#include <vector>
#include <queue>
#include <list>
#include <set>

#include "Exceptions.hh"

template<class T>
using uptr = ::std::unique_ptr<T>;
template<class T>
using ref_wr = ::std::reference_wrapper<T>;
template<class T>
using ref_vector = ::std::vector<::std::reference_wrapper<T>>;
template<class T>
using ref_queue = ::std::queue<::std::reference_wrapper<T>>;
template<class T>
using ref_deque = ::std::deque<::std::reference_wrapper<T>>;
template<class T>
using ref_list = ::std::list<::std::reference_wrapper<T>>;
template<class T>
using ref_set = ::std::set<::std::reference_wrapper<T>>;

#define STL_UNWRAP(stl_obj) decltype(stl_obj)::value_type::type
#define STL_ITEM_T(stl_obj) decltype(stl_obj)::value_type
