#ifndef SQUAWKBUS_SERVER_SUBSCRIPTION_MANAGER_HPP
#define SQUAWKBUS_SERVER_SUBSCRIPTION_MANAGER_HPP

#include <map>
#include <memory>
#include <set>
#include <regex>
#include <string>
#include <vector>

#include "messages/messages.hpp"

#include "subscription_repository.hpp"

namespace squawkbus::server
{
  using squawkbus::messages::SubscriptionRequest;

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

    std::vector<std::pair<std::string, std::vector<Interactor*>>> find_matching_subscriptions(const std::regex& regex) const;

  };
}

#endif // SQUAWKBUS_SERVER_SUBSCRIPTION_MANAGER_HPP
