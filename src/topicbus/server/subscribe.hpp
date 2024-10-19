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
  public:
    typedef std::shared_ptr<Interactor> interactor_ptr;

  private:
    std::map<std::string, std::vector<interactor_ptr>> subscriptions_;
    std::map<std::string, std::regex> regex_cache_;
    std::map<interactor_ptr, std::string> _interactor_subscriptions_;

  public:
    void on_subscription(
      interactor_ptr interactor,
      std::shared_ptr<SubscriptionRequest> message);

  private:
    void add_subscription(
      std::shared_ptr<Interactor> interactor,
      const std::string& topic
    );
    void remove_subscription(
      std::shared_ptr<Interactor> interactor,
      const std::string& topic
    );
  };
}

#endif // SQUAWKBUS_TOPIC_BUS_SERVER_SUBSCRIBE_HPP
