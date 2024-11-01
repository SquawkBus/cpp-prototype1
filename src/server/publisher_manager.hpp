#ifndef SQUAWKBUS_SERVER_PUBLISHER_MANAGER_HPP
#define SQUAWKBUS_SERVER_PUBLISHER_MANAGER_HPP

#include <map>
#include <set>
#include <string>

#include "messages/messages.hpp"

// #include "interactor.hpp"
#include "authorization_manager.hpp"

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
    AuthorizationManager authorization_manager_;
    std::map<std::string, std::set<Interactor*>> topic_publishers_;
    std::map<Interactor*, std::set<std::string>> publisher_topics_;

  public:
    PublisherManager(AuthorizationManager&& authorization_manager)
      : authorization_manager_(std::move(authorization_manager))
    {
    }

    void reload_authorizations();

    void on_send_unicast(
      Interactor* publisher,
      const UnicastData& request,
      std::map<std::string, Interactor*> interactors);
    void on_send_multicast(
      Interactor* publisher,
      const MulticastData& request,
      const SubscriptionManager& subscription_manager);
    void on_interactor_closed(
      Interactor* interactor,
      const SubscriptionManager& subscription_manager);

  private:
    void add_publisher(Interactor* publisher, const std::string& topic);
  };
}

#endif // SQUAWKBUS_SERVER_PUBLISHER_MANAGER_HPP
