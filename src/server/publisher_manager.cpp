#include "publisher_manager.hpp"

#include <algorithm>
#include <format>
#include <iterator>
#include <memory>
#include <utility>

#include "logging/log.hpp"

#include "subscription_manager.hpp"
#include "authorization.hpp"

namespace squawkbus::server
{
  namespace
  {
    auto log = logging::logger("squawkbus");
  }

  using squawkbus::messages::ForwardedMulticastData;
  using squawkbus::messages::ForwardedUnicastData;
  using squawkbus::messages::MulticastData;
  using squawkbus::messages::UnicastData;
  using squawkbus::messages::DataPacket;
  
  static std::vector<DataPacket> get_authorized_data(
    const std::vector<DataPacket>& data_packets,
    const std::set<std::int32_t>& entitlements)
  {
    std::vector<DataPacket> authorized_data_packets;
    for (auto &data_packet : data_packets)
    {
      if (data_packet.is_authorized(entitlements))
        authorized_data_packets.push_back(data_packet);
    }
    return authorized_data_packets;
  }

  template<typename T>
  static std::set<T> intersection(const std::set<T>& a, const std::set<T>& b)
  {
    auto c = std::set<T>();
    std::set_intersection(
      a.begin(), a.end(),
      b.begin(), b.end(),
      std::inserter(c, c.begin()));
    return c;
  }

  void PublisherManager::on_send_unicast(
    Interactor* publisher,
    const UnicastData& request,
    std::map<std::string, Interactor*> interactors)
  {
    auto i_subscriber = interactors.find(request.client_id());
    if (i_subscriber == interactors.end())
    {
      log.info(std::format("no interactor for {}", request.client_id()));
      return;
    }

    auto receiver = i_subscriber->second;

    auto publisher_entitlements = authorization_manager_.entitlements(
      publisher->user(),
      request.topic(),
      Role::Publisher
    );
    auto receiver_entitlements = authorization_manager_.entitlements(
      receiver->user(),
      request.topic(),
      Role::Subscriber
    );
    auto entitlements = intersection(publisher_entitlements, receiver_entitlements);

    if (!publisher_entitlements.empty() && entitlements.empty())
    {
      log.debug(
        std::format(
          "no entitlements from {} to {} for {}",
          publisher->user(),
          receiver->user(),
          request.topic()
        )
      );
      return;
    }

    auto authorized_data_packets = get_authorized_data(
      request.data_packets(),
      entitlements);

    add_publisher(publisher, request.topic());

    auto response = std::make_shared<ForwardedUnicastData>(
      publisher->user(),
      publisher->host(),
      request.client_id(),
      request.topic(),
      authorized_data_packets
    );
    receiver->send(response);
  }

  void PublisherManager::on_send_multicast(
    Interactor* publisher,
    const MulticastData& request,
    const SubscriptionManager& subscription_manager)
  {
    add_publisher(publisher, request.topic());

    auto subscribers = subscription_manager.find_subscribers(request.topic());
    if (subscribers.empty())
    {
      return;
    }

    auto publisher_entitlements = authorization_manager_.entitlements(
      publisher->user(),
      request.topic(),
      Role::Publisher
    );

    bool has_published = false;

    for (auto subscriber : subscribers)
    {
      auto subscriber_entitlements = authorization_manager_.entitlements(
        subscriber->user(),
        request.topic(),
        Role::Subscriber
      );
      auto entitlements = intersection(publisher_entitlements, subscriber_entitlements);

      if (!publisher_entitlements.empty() && entitlements.empty())
      {
        log.debug(
          std::format(
            "no entitlements from {} to {} for {}",
            publisher->user(),
            subscriber->user(),
            request.topic()
          )
        );
        continue;
      }

      auto authorized_data_packets = get_authorized_data(
        request.data_packets(),
        entitlements);

      auto response = std::make_shared<ForwardedMulticastData>(
        publisher->user(),
        publisher->host(),
        request.topic(),
        authorized_data_packets
      );

      subscriber->send(response);

      has_published = true;
    }

    if (has_published)
      add_publisher(publisher, request.topic());
  }

  void PublisherManager::on_interactor_closed(
    Interactor* interactor,
    const SubscriptionManager& subscription_manager)
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

        // Inform subscribers that the publisher has disconnected by sending
        // an empty message.
        auto subscribers = subscription_manager.find_subscribers(topic);
        if (subscribers.empty())
        {
          continue;
        }
        auto message = std::make_shared<ForwardedMulticastData>(
          interactor->user(),
          interactor->host(),
          topic,
          std::vector<DataPacket> {});
        for (auto subscriber: subscribers)
        {
          subscriber->send(message);
        }
      }

    }
  }

  void PublisherManager::reload_authorizations()
  {
      authorization_manager_.reload();
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
