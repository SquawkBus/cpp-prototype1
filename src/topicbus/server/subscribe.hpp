#ifndef SQUAWKBUS_TOPIC_BUS_SERVER_SUBSCRIBE_HPP
#define SQUAWKBUS_TOPIC_BUS_SERVER_SUBSCRIBE_HPP

#include <map>
#include <memory>
#include <regex>
#include <string>
#include <vector>

#include "topicbus/messages/message.hpp"

namespace squawkbus::topicbus::server
{
  using squawkbus::topicbus::messages::SubscriptionRequest;

  class Interactor;

  class SubscriptionManager
  {
  private:
    std::map<std::string, std::vector<Interactor*>> subscriptions_;
    std::map<std::string, std::regex> regex_cache_;
    std::map<Interactor*, std::string> _interactor_subscriptions_;

  public:
    void on_subscription(
      Interactor* interactor,
      SubscriptionRequest* message);

  private:
    void add_subscription(
      Interactor* interactor,
      const std::string& topic
    );
    void remove_subscription(
      Interactor* interactor,
      const std::string& topic
    );
  };
}

#endif // SQUAWKBUS_TOPIC_BUS_SERVER_SUBSCRIBE_HPP
