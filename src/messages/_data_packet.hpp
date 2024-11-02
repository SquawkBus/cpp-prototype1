#ifndef SQUAWKBUS_MESSAGES_DATA_PACKET_HPP
#define SQUAWKBUS_MESSAGES_DATA_PACKET_HPP

#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "utils/utils.hpp"

#include "serialization/frame_buffer.hpp"
#include "serialization/frame_buffer_io.hpp"

namespace squawkbus::messages
{
  using squawkbus::serialization::FrameBuffer;
  
  class DataPacket
  {
  public:
    std::int32_t entitlement;
    std::string content_type;
    std::vector<char> data;

  public:
    DataPacket() noexcept
    {
    }

    DataPacket(
      const std::int32_t& entitlement,
      const std::string& content_type,
      const std::vector<char>& data) noexcept
      : entitlement(entitlement),
        content_type(content_type),
        data(data)
    {
    }

    DataPacket(
      std::int32_t&& entitlement,
      std::string&& content_type,
      std::vector<char>&& data) noexcept
      : entitlement(entitlement),
        content_type(std::move(content_type)),
        data(std::move(data))
    {
    }

    bool is_authorized(const std::set<std::int32_t> &granted_entitlements) const noexcept
    {
      return granted_entitlements.contains(entitlement);
    }

    bool operator==(const DataPacket& other) const noexcept
    {
      return
        entitlement == other.entitlement &&
        content_type == other.content_type &&
        data == other.data;
    }

    FrameBuffer& serialize(FrameBuffer& frame) const
    {
      return
        frame
          << entitlement
          << content_type
          << data;
    }

    FrameBuffer& deserialize(FrameBuffer& frame)
    {
      return
        frame
          >> entitlement
          >> content_type
          >> data;
    }
  };

  inline std::ostream& operator << (std::ostream& os, const DataPacket& d)
  {
    return os
      << "DataPacket"
      << "(entitlement=" << d.entitlement
      << ",content_type=" << d.content_type
      << ",data=" << ::to_string(d.data)
      << ")" ;
  }

  inline FrameBuffer& operator<<(FrameBuffer& frame, const DataPacket& d)
  {
    return d.serialize(frame);
  }

  inline FrameBuffer& operator>>(FrameBuffer& frame, DataPacket& p)
  {
    return p.deserialize(frame);
  }
  
}

#endif // SQUAWKBUS_MESSAGES_DATA_PACKET_HPP
