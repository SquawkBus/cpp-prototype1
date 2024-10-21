#ifndef SQUAWKBUS_SERVER_PUBLISH_HPP
#define SQUAWKBUS_SERVER_PUBLISH_HPP

#include <map>
#include <set>
#include <string>

#include "messages/message.hpp"

#include "interactor.hpp"

namespace squawkbus::server
{
  using squawkbus::messages::MulticastData;
  using squawkbus::messages::UnicastData;
  
  class PublisherManager
  {
  private:
    std::map<std::string, std::set<Interactor*>> topic_publishers_;
    std::map<Interactor*, std::set<std::string>> publisher_topics_;

  public:
    void on_send_unicast(Interactor* publisher, UnicastData* message);
    void on_send_multicast(Interactor* publisher, MulticastData* message);
    void on_interactor_closed(Interactor* interactor);

  private:
    void add_publisher(Interactor* publisher, const std::string& topic);
  };
}

#endif // SQUAWKBUS_SERVER_PUBLISH_HPP
