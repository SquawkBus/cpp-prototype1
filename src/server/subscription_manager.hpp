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
    std::map<Interactor*, std::set<std::string>> subscriber_topics_;

  public:
    void on_subscription(
      Interactor* subscriber,
      const SubscriptionRequest& request,
      const NotificationManager& notification_manager);

    void on_interactor_closed(
      Interactor* subscriber,
      const NotificationManager& notification_manager);

    std::vector<std::pair<std::string, std::vector<Interactor*>>> find_matching_subscriptions(const std::regex& regex) const;
    std::set<Interactor*> find_subscribers(const std::string& topic) const;

  private:
    void add_subscription(
      Interactor* subscriber,
      const std::string& topic_pattern,
      const NotificationManager& notification_manager);
    void remove_subscription(
      Interactor* subscriber,
      const std::string& topic_pattern,
      const NotificationManager& notification_manager);
    void remove_interactor(
      Interactor*,
      const NotificationManager& notification_manager);
  };
}

#endif // SQUAWKBUS_SERVER_SUBSCRIPTION_MANAGER_HPP
