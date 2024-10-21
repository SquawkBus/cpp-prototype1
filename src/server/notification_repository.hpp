#ifndef SQUAWKBUS_SERVER_NOTIFICATION_REPOSITORY_HPP
#define SQUAWKBUS_SERVER_NOTIFICATION_REPOSITORY_HPP

#include <map>
#include <memory>
#include <set>
#include <regex>
#include <string>
#include <vector>

#include "messages/messages.hpp"

namespace squawkbus::server
{
  class Interactor;

  class NotificationRepository
  {
  private:
    std::map<std::string, std::map<Interactor*, int>> notifications_;
    std::map<std::string, std::regex> regex_cache_;
    std::map<Interactor*, std::set<std::string>> listener_topic_patterns_;

  public:
    void add_listener(Interactor* listener, const std::string& topic_pattern);
    void remove_listener(Interactor* listener, const std::string& topic_pattern);
    void remove_interactor(Interactor*);
    std::set<Interactor*> find_listeners(const std::string& topic) const;
  };
}

#endif // SQUAWKBUS_SERVER_NOTIFICATION_REPOSITORY_HPP
