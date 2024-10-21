#ifndef SQUAWKBUS_TOPIC_BUS_SERVER_SUBSCRIPTION_MANAGER_HPP
#define SQUAWKBUS_TOPIC_BUS_SERVER_SUBSCRIPTION_MANAGER_HPP

#include <map>
#include <memory>
#include <set>
#include <regex>
#include <string>
#include <vector>

#include "topicbus/messages/message.hpp"

#include "subscription_repository.hpp"

namespace squawkbus::topicbus::server
{
  using squawkbus::topicbus::messages::SubscriptionRequest;

  class Interactor;
  class NotificationManager;

  class SubscriptionManager
  {
  private:
    SubscriptionRepository repository_;

  public:
    void on_subscription(
      Interactor* subscriber,
      SubscriptionRequest* message,
      NotificationManager& notification_manager);

    void on_interactor_closed(Interactor* subscriber);
    
  private:
    void add_subscription(
      Interactor* subscriber,
      const std::string& topic_pattern,
      NotificationManager& notification_manage);
    void remove_subscription(
      Interactor* subscriber,
      const std::string& topic_pattern,
      NotificationManager& notification_manage);
  };
}

#endif // SQUAWKBUS_TOPIC_BUS_SERVER_SUBSCRIPTION_MANAGER_HPP
