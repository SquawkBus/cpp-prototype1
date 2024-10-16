#ifndef SQUAWKBUS_IO_LOGGER_HPP
#define SQUAWKBUS_IO_LOGGER_HPP

#include "logging/log.hpp"

namespace squawkbus::io
{
  static squawkbus::logging::Logger log = logging::logger("SQUAWKBUS_IO");
}

#endif // SQUAWKBUS_IO_LOGGER_HPP
