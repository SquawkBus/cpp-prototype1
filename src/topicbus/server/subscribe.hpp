#ifndef SQUAWKBUS_TOPIC_BUS_SERVER_SUBSCRIBE_HPP
#define SQUAWKBUS_TOPIC_BUS_SERVER_SUBSCRIBE_HPP

#include <memory>

#include "topicbus/messages/message.hpp"

namespace squawkbus::topicbus::server
{
  using squawkbus::topicbus::messages::SubscriptionRequest;

  class Interactor;

  class SubscriptionManager
  {
  public:
    typedef std::shared_ptr<Interactor> interactor_ptr;

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
