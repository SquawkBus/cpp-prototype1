#include "authorization_manager.hpp"

#include <algorithm>
#include <cstdint>
#include <format>
#include <ranges>
#include <regex>
#include <set>
#include <string>
#include <stdexcept>
#include <vector>

#include "logging/log.hpp"

#include "constants.hpp"

namespace squawkbus::server
{
  namespace
  {
    auto log = logging::logger(LOGGER_NAME);
  }

  void AuthorizationManager::load()
  {
    log.debug("Configuring authorizations.");

    repository_ = AuthorizationRepository::make(path_, cmd_line_specs_);
  }
}
