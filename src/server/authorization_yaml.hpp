#ifndef SQUAWKBUS_SERVER_AUTHORIZATION_YAML_HPP
#define SQUAWKBUS_SERVER_AUTHORIZATION_YAML_HPP

#include <yaml-cpp/yaml.h>

#include "role.hpp"

namespace squawkbus::server
{
  struct Authorization
  {
    std::vector<int> entitlements;
    Role role;
  };
}

namespace YAML
{
  using squawkbus::server::Role;
  using squawkbus::server::Authorization;
  
  template<>
  struct convert<Role>
  {
    static Node encode(const Role& role)
    {
      Node node;

      if ((role & Role::Subscriber) != Role::Subscriber)
        node.push_back("Subscriber");
      if ((role & Role::Notifier) != Role::Notifier)
        node.push_back("Notifier");
      if ((role & Role::Publisher) != Role::Publisher)
        node.push_back("Publisher");

      return node;
    }

    static bool decode(const Node& node, Role& role)
    {
      if (!node.IsSequence())
        return false;

      role = Role::None;
      for (auto& element : node)
      {
        if (element.as<std::string>() == "Subscriber")
          role = (role | Role::Subscriber);
        else if (element.as<std::string>() == "Publisher")
          role = (role | Role::Publisher);
        if (element.as<std::string>() == "Notifier")
          role = (role | Role::Notifier);
      }

      return true;
    }
  };

  template<>
  struct convert<Authorization>
  {
    static Node encode(const Authorization& authorization)
    {
      Node node;
      node["entitlements"] = authorization.entitlements;
      node["roles"] = authorization.role;
      return node;
    }

    static bool decode(const Node& node, Authorization& authorization)
    {
      if (!node.IsMap())
        return false;

      authorization.entitlements = node["entitlements"].as<std::vector<int>>();
      authorization.role = node["roles"].as<Role>();

      return true;
    }
  };

}

#endif // SQUAWKBUS_SERVER_AUTHORIZATION_YAML_HPP
