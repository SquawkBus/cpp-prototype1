#ifndef SQUAWKBUS_SERVER_PUBLISHER_MANAGER_HPP
#define SQUAWKBUS_SERVER_PUBLISHER_MANAGER_HPP

#include <map>
#include <set>
#include <string>

#include "messages/messages.hpp"

#include "interactor.hpp"

namespace squawkbus::server
{
  using squawkbus::messages::MulticastData;
  using squawkbus::messages::UnicastData;

  class Interactor;
  class SubscriptionManager;

  class PublisherManager
  {
  private:
    std::map<std::string, std::set<Interactor*>> topic_publishers_;
    std::map<Interactor*, std::set<std::string>> publisher_topics_;

  public:
    void on_send_unicast(Interactor* publisher, UnicastData* message, std::map<std::string, Interactor*> interactors);
    void on_send_multicast(Interactor* publisher, MulticastData* message, const SubscriptionManager& subscription_manager);
    void on_interactor_closed(Interactor* interactor);

  private:
    void add_publisher(Interactor* publisher, const std::string& topic);
  };
}

#endif // SQUAWKBUS_SERVER_PUBLISHER_MANAGER_HPP
