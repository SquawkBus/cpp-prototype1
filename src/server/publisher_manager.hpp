#ifndef SQUAWKBUS_SERVER_PUBLISHER_MANAGER_HPP
#define SQUAWKBUS_SERVER_PUBLISHER_MANAGER_HPP

#include <map>
#include <set>
#include <string>

#include "messages/messages.hpp"

#include "interactor.hpp"
#include "authorization.hpp"

namespace squawkbus::server
{
  using squawkbus::messages::MulticastData;
  using squawkbus::messages::UnicastData;
  using squawkbus::messages::DataPacket;

  class Interactor;
  class SubscriptionManager;

  class PublisherManager
  {
  private:
    std::map<std::string, std::set<Interactor*>> topic_publishers_;
    std::map<Interactor*, std::set<std::string>> publisher_topics_;

  public:
    void on_send_unicast(
      Interactor* publisher,
      const UnicastData& request,
      std::map<std::string, Interactor*> interactors,
      const AuthorizationManager& authorization_manager);
    void on_send_multicast(
      Interactor* publisher,
      const MulticastData& request,
      const SubscriptionManager& subscription_manager,
      const AuthorizationManager& authorization_manager);
    void on_interactor_closed(Interactor* interactor, const SubscriptionManager& subscription_manager);

  private:
    void add_publisher(Interactor* publisher, const std::string& topic);
  };
}

#endif // SQUAWKBUS_SERVER_PUBLISHER_MANAGER_HPP
