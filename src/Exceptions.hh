#pragma once

#include <stdexcept>

class NotImplementedException : public std::logic_error {
public:
  NotImplementedException() : logic_error("Not implemented") {}
  NotImplementedException(const char* name);
};


class NotSupportedException : public std::logic_error {
public:
  NotSupportedException() : logic_error("Not supported") {}
  NotSupportedException(const char* name, const char* type);
};

