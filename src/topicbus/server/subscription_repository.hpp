#ifndef SQUAWKBUS_TOPIC_BUS_SERVER_SUBSCRIPTION_REPOSITORY_HPP
#define SQUAWKBUS_TOPIC_BUS_SERVER_SUBSCRIPTION_REPOSITORY_HPP

#include <map>
#include <memory>
#include <set>
#include <regex>
#include <string>
#include <vector>

#include "topicbus/messages/message.hpp"

namespace squawkbus::topicbus::server
{
  class Interactor;

  class SubscriptionRepository
  {
  private:
    std::map<std::string, std::map<Interactor*, int>> subscriptions_;
    std::map<std::string, std::regex> regex_cache_;
    std::map<Interactor*, std::set<std::string>> subscriber_topic_patterns_;

  public:
    void add_subscription(Interactor* subscriber, const std::string& topic_pattern);
    void remove_subscription(Interactor* subscriber, const std::string& topic_pattern);
    void remove_interactor(Interactor*);
    std::set<Interactor*> find_subscribers(const std::string& topic) const;
  };
}

#endif // SQUAWKBUS_TOPIC_BUS_SERVER_SUBSCRIPTION_REPOSITORY_HPP
