#ifndef SQUAWKBUS_TOPICBUS_SERVER_PUBLISH_HPP
#define SQUAWKBUS_TOPICBUS_SERVER_PUBLISH_HPP

#include <map>
#include <set>
#include <string>

#include "topicbus/messages/message.hpp"

#include "interactor.hpp"

namespace squawkbus::topicbus::server
{
  using squawkbus::topicbus::messages::MulticastData;
  using squawkbus::topicbus::messages::UnicastData;
  
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

#endif // SQUAWKBUS_TOPICBUS_SERVER_PUBLISH_HPP
