#pragma once

#include <memory>
#include <functional>
#include <vector>
#include <queue>
#include <list>

template<class T>
using uptr = ::std::unique_ptr<T>;
template<class T>
using ref_wr = ::std::reference_wrapper<T>;
template<class T>
using ref_vector = ::std::vector<::std::reference_wrapper<T>>;
template<class T>
using ref_queue = ::std::queue<::std::reference_wrapper<T>>;
template<class T>
using ref_list = ::std::list<::std::reference_wrapper<T>>;

