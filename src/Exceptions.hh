#pragma once

#include <cassert>
#include <stdexcept>

#if ! defined(NDEBUG)

#define NotImplementedException(arg) (assert(false),std::logic_error("Not implemented"))

#else

class NotImplementedException : public std::logic_error {
public:
  NotImplementedException() : logic_error("Not implemented") {}
  NotImplementedException(const char* name);
};

#endif

class NotSupportedException : public std::logic_error {
public:
  NotSupportedException() : logic_error("Not supported") {}
  NotSupportedException(const char* name, const char* type);
};