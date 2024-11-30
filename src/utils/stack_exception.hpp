#ifndef SQUAWKBUS_UTILS_STACK_EXCEPTION_HPP
#define SQUAWKBUS_UTILS_STACK_EXCEPTION_HPP

#include <source_location>
#include <stacktrace>
#include <stdexcept>
#include <string>

namespace squawkbus::utils
{
  
  class stack_exception : public std::exception
  {
  public:
      explicit stack_exception(
          std::string message,
          const std::source_location& location = std::source_location::current(),
          const std::stacktrace& stacktrace = std::stacktrace::current())
          :   _message(std::move(message)),
              _location(location),
              _stacktrace(stacktrace)
      {
      }
      virtual ~stack_exception() noexcept {}

      virtual const char* what() const noexcept { return _message.c_str(); }
      const std::source_location& where() const noexcept { return _location; }
      const std::stacktrace& stack() const noexcept { return _stacktrace; }

  private:
      std::string _message;
      const std::source_location _location;
      const std::stacktrace _stacktrace;
      
  };

}

#endif // SQUAWKBUS_UTILS_STACK_EXCEPTION_HPP
