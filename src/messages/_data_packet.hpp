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
  private:
    std::int32_t entitlement_;
    std::string content_type_;
    std::vector<char> data_;

  public:
    DataPacket() noexcept
    {
    }

    DataPacket(
      const std::int32_t& entitlement,
      const std::string& content_type,
      const std::vector<char>& data) noexcept
      : entitlement_(entitlement),
        content_type_(content_type),
        data_(data)
    {
    }

    DataPacket(
      std::int32_t&& entitlement,
      std::string&& content_type,
      std::vector<char>&& data) noexcept
      : entitlement_(entitlement),
        content_type_(std::move(content_type)),
        data_(std::move(data))
    {
    }

    std::int32_t entitlement() const noexcept { return entitlement_; }
    const std::string& content_type() const noexcept { return content_type_; }
    const std::vector<char>& data() const noexcept { return data_; }

    bool is_authorized(const std::set<std::int32_t> &granted_entitlements) const noexcept
    {
      return granted_entitlements.contains(entitlement_);
    }

    bool operator==(const DataPacket& other) const noexcept
    {
      return
        entitlement_ == other.entitlement_ &&
        content_type_ == other.content_type_ &&
        data_ == other.data_;
    }

    FrameBuffer& serialize(FrameBuffer& frame) const
    {
      return
        frame
          << entitlement_
          << content_type_
          << data_;
    }

    FrameBuffer& deserialize(FrameBuffer& frame)
    {
      return
        frame
          >> entitlement_
          >> content_type_
          >> data_;
    }
  };

  inline std::ostream& operator << (std::ostream& os, const DataPacket& d)
  {
    return os
      << "DataPacket"
      << "(entitlement=" << d.entitlement()
      << ",content_type=" << d.content_type()
      << ",data=" << ::to_string(d.data())
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
