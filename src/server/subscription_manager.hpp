#ifndef SQUAWKBUS_SERVER_SUBSCRIPTION_MANAGER_HPP
#define SQUAWKBUS_SERVER_SUBSCRIPTION_MANAGER_HPP

#include <map>
#include <memory>
#include <set>
#include <regex>
#include <string>
#include <vector>

#include "messages/messages.hpp"

namespace squawkbus::server
{
  using squawkbus::messages::SubscriptionRequest;

  class Interactor;
  class NotificationManager;

  class SubscriptionManager
  {
  private:
    std::map<std::string, std::map<Interactor*, int>> subscriptions_;
    std::map<std::string, std::regex> regex_cache_;
    std::map<Interactor*, std::set<std::string>> subscriber_topic_patterns_;

  public:
    void on_subscription(
      Interactor* subscriber,
      SubscriptionRequest* message,
      NotificationManager& notification_manager);

    void on_interactor_closed(Interactor* subscriber);

    std::vector<std::pair<std::string, std::vector<Interactor*>>> find_matching_subscriptions(const std::regex& regex) const;

  private:
    void add_subscription(Interactor* subscriber, const std::string& topic_pattern);
    void remove_subscription(Interactor* subscriber, const std::string& topic_pattern);
    void remove_interactor(Interactor*);
    std::set<Interactor*> find_subscribers(const std::string& topic) const;
  };
}

#endif // SQUAWKBUS_SERVER_SUBSCRIPTION_MANAGER_HPP
