#ifndef SQUAWKBUS_SERVER_NOTIFICATION_MANAGER_HPP
#define SQUAWKBUS_SERVER_NOTIFICATION_MANAGER_HPP

#include <map>
#include <regex>
#include <string>

#include "messages/messages.hpp"

namespace squawkbus::server
{
  using squawkbus::messages::NotificationRequest;

  class Interactor;
  class SubscriptionManager;

  class NotificationManager
  {
  private:
    std::map<std::string, std::map<Interactor*, int>> notifications_;
    std::map<std::string, std::regex> regex_cache_;
    std::map<Interactor*, std::set<std::string>> listener_topic_patterns_;

  public:
    void on_listen(Interactor* listener, NotificationRequest* message, const SubscriptionManager& subscription_manager);
    void on_interactor_closed(Interactor* listener);
    void notify(Interactor* subscriber, const std::string& topic_pattern, bool is_add);

  private:
    void add_listener(Interactor* listener, const std::string& topic_pattern, const SubscriptionManager& subscription_manager);
    void remove_listener(Interactor* listener, const std::string& topic_pattern, const SubscriptionManager& subscription_manager);
    void remove_interactor(Interactor*);
    std::set<Interactor*> find_listeners(const std::string& topic) const;
  };
}

#endif // SQUAWKBUS_SERVER_NOTIFICATION_MANAGER_HPP
