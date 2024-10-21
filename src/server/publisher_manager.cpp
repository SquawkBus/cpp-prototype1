#include <format>
#include <memory>
#include <utility>

#include "logging/log.hpp"

#include "publisher_manager.hpp"
#include "subscription_manager.hpp"

namespace logging = squawkbus::logging;

namespace squawkbus::server
{
  using squawkbus::messages::ForwardedMulticastData;
  using squawkbus::messages::ForwardedUnicastData;
  using squawkbus::messages::MulticastData;
  using squawkbus::messages::UnicastData;
  
  void PublisherManager::on_send_unicast(
    Interactor* publisher,
    const UnicastData& request,
    std::map<std::string, Interactor*> interactors)
  {
    auto i_subscriber = interactors.find(request.client_id());
    if (i_subscriber == interactors.end())
    {
      logging::info(std::format("no interactor for {}", request.client_id()));
      return;
    }
    auto client = i_subscriber->second;

    auto response = std::make_shared<ForwardedUnicastData>(
      publisher->user(),
      publisher->host(),
      request.client_id(),
      request.topic(),
      request.data_packets()
    );
    client->send(response);
  }

  void PublisherManager::on_send_multicast(
    Interactor* publisher,
    const MulticastData& request,
    const SubscriptionManager& subscription_manager)
  {
    auto subscribers = subscription_manager.find_subscribers(request.topic());
    auto response = std::make_shared<ForwardedMulticastData>(
      publisher->user(),
      publisher->host(),
      request.topic(),
      request.data_packets()
    );
    for (auto subscriber : subscribers)
    {
      subscriber->send(response);
    }
  }

  void PublisherManager::on_interactor_closed(Interactor* interactor)
  {
    // Find any topics this interactor has published.
    auto i_publisher_topics = publisher_topics_.find(interactor);
    if (i_publisher_topics != publisher_topics_.end())
    {
      // Remove the publisher, remembering topics left without publishers.
      auto stale_topics = std::set<std::string> {};
      for (auto& [topic, publishers] : topic_publishers_)
      {
        publishers.erase(interactor);
        if (publishers.empty())
        {
          stale_topics.insert(topic);
        }
      }

      publisher_topics_.erase(i_publisher_topics);

      for (auto& topic : stale_topics)
      {
        topic_publishers_.erase(topic);
      }
    }
  }

  void PublisherManager::add_publisher(Interactor* publisher, const std::string& topic)
  {
    auto i_topic_publishers = topic_publishers_.find(topic);
    if (i_topic_publishers != topic_publishers_.end())
    {
      i_topic_publishers->second.insert(publisher);
    }
    else
    {
      topic_publishers_.insert({ { topic, { publisher } } });
    }

    auto i_publisher_topics = publisher_topics_.find(publisher);
    if (i_publisher_topics != publisher_topics_.end())
    {
      i_publisher_topics->second.insert(topic);
    }
    else
    {
      publisher_topics_.insert({ { publisher, { topic } } });
    }
  }
}
