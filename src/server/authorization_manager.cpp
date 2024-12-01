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

#include "authorization_manager.hpp"

namespace squawkbus::server
{
  namespace
  {
    auto log = logging::logger("squawkbus");
  }

  void AuthorizationManager::load()
  {
    log.info("Configuring authorizations.");

    repository_ = AuthorizationRepository::make(path_, cmd_line_specs_);
  }
}
